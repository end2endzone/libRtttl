#include "Controller.h"
#include "Model.h"
#include "tone.h"
#include "beep.h"
#include "arduinoBinRtttl.h"
#include "arduinoPlay10BitsRtttl.h"
#include "arduinoPlay16BitsRtttl.h"

#ifndef WX_PRECOMP
  #include <wx/wx.h>
  #include <wx/frame.h>
#else
  #include <wx/wxprec.h>
#endif

Controller::Controller()
{
}

Controller & Controller::getInstance()
{
  static Controller controller;
  return controller;
}

Controller::~Controller()
{
}

void Controller::play()
{
  //Update libRTTTL settings
  applyRtttlOptions();

  Model & m = Model::getInstance();

  //Update model for request
  Model::MODEL_REQUEST r = m.getRequest();

  //play the song to extract tone() & delay() calls
  const char * rtttl = r.rtttlInput.c_str();
  TONE_CALL_INFO_LIST calls = playRTTTL(rtttl);

  //Remove all delay() calls which matches a tone() call.
  removeExpectedDelays(calls);

  //forward each calls to the beep.dll module
  int numNotes = 0;
  for(size_t i=0; i<calls.size(); i++)
  {
    const TONE_CALL_INFO & c = calls[i];
    if (c._tone != INVALID_TONE_INFO)
    {
      beep(c._tone.freq, c._tone.duration);
    }
    if (c.delay != INVALID_TONE_DURATION)
    {
      sleep(c.delay);
    }
  }

  //success ?
  if ( calls.size() > 0 )
  {
    r.result = SUCCESS;

    char info[1024];
    sprintf(info, "Played %d notes.", calls.size());
    r.info = info;
  }
  else
  {
    r.result = NOT_SUPPORTED;
  }

  //set result back to model
  m.setRequest(r);
}

void Controller::optimize()
{
  //Update libRTTTL settings
  applyRtttlOptions();

  Model & m = Model::getInstance();

  //Update model for request
  Model::MODEL_REQUEST r = m.getRequest();

  //Optmize RTTTL string
  const char * code = r.rtttlInput.c_str();
  std::string optimizedRtttl;
  r.result = optimizeEncoding(code, optimizedRtttl);

  //success ?
  if ( r.result == SUCCESS )
  {
    r.rtttlOutput = optimizedRtttl.c_str();

    char info[1024];
    int newSize = (int)r.rtttlOutput.size();
    int oldSize = (int)r.rtttlInput.size();
    int diffSize = oldSize - newSize;
    sprintf(info, "Saved %d bytes. Now using %d bytes instead of %d bytes.", diffSize, newSize, oldSize);
    r.info = info;
  }
  else
  {
    r.errorDescription = getErrorDescription(r.result).c_str();
  }

  //set result back to model
  m.setRequest(r);
}

void Controller::toTone()
{
  //Update libRTTTL settings
  applyRtttlOptions();

  Model & m = Model::getInstance();

  //Update model for request
  Model::MODEL_REQUEST r = m.getRequest();

  //Convert string to RTTTL_SONG
  RTTTL_SONG rtttl;
  reset(rtttl);
  const char * code = r.rtttlInput.c_str();
  RTTTL_RESULT result = parseRtttlString(code, rtttl);
  if (result != SUCCESS)
  {
    r.result = result;
    r.errorDescription = getErrorDescription(r.result);

    //set result back to model
    m.setRequest(r);
    return;
  }

  //play the song to extract tone() & delay() calls
  const TONE_CALL_INFO_LIST calls = playRTTTL(rtttl);

  //output calls to the output string
  char functionCall[1024];
  r.rtttlOutput = "";
  for(size_t i=0; i<calls.size(); i++)
  {
    const TONE_CALL_INFO & c = calls[i];
    if (c._tone != INVALID_TONE_INFO)
    {
      sprintf(functionCall, "  tone(pin, %d, %d);\n", c._tone.freq, c._tone.duration);
      r.rtttlOutput.append( functionCall );
    }
    if (c.delay != INVALID_TONE_DURATION)
    {
      sprintf(functionCall, "  delay(%d);\n", c.delay);
      r.rtttlOutput.append( functionCall );
    }
  }

  //success ?
  if ( calls.size() > 0 )
  {
    r.result = SUCCESS;

    //make the output a function call
    char functionDeclaration[1024];
    rtttl.name[0] = toupper( rtttl.name[0] );
    sprintf(functionDeclaration, "void play%s(int pin) {\n", rtttl.name);
    r.rtttlOutput.insert(0, functionDeclaration);
    r.rtttlOutput.append("}\n");

    char info[1024];
    sprintf(info, "%d function calls required.", calls.size());
    r.info = info;
  }
  else
  {
    r.result = NOT_SUPPORTED;
  }

  //set result back to model
  m.setRequest(r);
}

void Controller::fromTone()
{
  //Update libRTTTL settings
  applyRtttlOptions();

  Model & m = Model::getInstance();

  //Update model for request
  Model::MODEL_REQUEST r = m.getRequest();

  //Convert string tone() calls to actual calls
  TONE_CALL_INFO_LIST calls;
  if ( !parseToneCallsString(r.rtttlInput.c_str(), calls) )
  {
    //failed parsing tone() calls
    r.result = NOT_SUPPORTED;
    r.errorDescription = getErrorDescription(r.result);

    //set result back to model
    m.setRequest(r);
    return;
  }

  //Convert tone() calls to rtttl code
  std::string code;
  RTTTL_RESULT result = calls2Rtttl( calls, "unknown", code );
  if (result != SUCCESS)
  {
    r.result = result;
    r.errorDescription = getErrorDescription(r.result);

    //set result back to model
    m.setRequest(r);
    return;
  }

  //success ?
  if ( code.size() > 0 )
  {
    r.result = SUCCESS;

    //build the output rtttl code
    r.rtttlOutput = code;

    char info[1024];
    sprintf(info, "Using %d bytes from %d function calls.", code.size(), calls.size());
    r.info = info;
  }
  else
  {
    r.result = NOT_SUPPORTED;
  }

  //set result back to model
  m.setRequest(r);
}

void Controller::toBinary10()
{
  //Update libRTTTL settings
  applyRtttlOptions();

  Model & m = Model::getInstance();

  //Update model for request
  Model::MODEL_REQUEST r = m.getRequest();

  //Convert string to RTTTL_SONG
  RTTTL_SONG rtttl;
  reset(rtttl);
  const char * code = r.rtttlInput.c_str();
  RTTTL_RESULT result = parseRtttlString(code, rtttl);
  if (result != SUCCESS)
  {
    r.result = result;
    r.errorDescription = getErrorDescription(r.result);

    //set result back to model
    m.setRequest(r);
    return;
  }

  //Force song to use anyBpm instead of bpmIdx
  //since arduino only supports anyBpm
  if (!rtttl.defaults.hasAnyBpm)
  {
    rtttl.defaults.anyBpm = getOfficialBpmFromIndex(rtttl.defaults.bpmIdx);
    rtttl.defaults.hasAnyBpm = true;
  }

  //Convert song to stream
  STREAM stream;
  toStream10(rtttl, stream);

  //convert stream to c++ code
  std::string cppCode = toCppArray(stream);

  //capitalize the name of the song
  rtttl.name[0] = toupper( rtttl.name[0] );

  //success ?
  if ( stream.length > 0 )
  {
    r.result = SUCCESS;

    //append arduino code for decoding RTTTL data
    r.rtttlOutput = "";
    r.rtttlOutput.append("// Note:\n//   10 Bits RTTTL format requires the BitReader library.\n//   The code & updates for the BitReader library can be found on http://end2endzone.com\n#include <bitreader.h>\n\n");
    r.rtttlOutput.append("//#define RTTTL_SERIAL_DEBUG\n\n");
    appendArduinoBinRtttlFile(r.rtttlOutput);
    r.rtttlOutput.append("\n");
    appendArduinoPlay10BitsRtttlFile(r.rtttlOutput);
    r.rtttlOutput.append("\n");

    //build a cpp friendly code for extracting data
    char buffer[10240];
    sprintf(buffer, "//rtttl 10 bits binary format for the following: %s\n" \
                    "const unsigned char %s[] = %s;\n" \
                    "const int %s_note_length = %d;\n",
                    code, rtttl.name, cppCode.c_str(), rtttl.name, rtttl.notes.size());
    r.rtttlOutput.append(buffer);

    //build information message
    char info[1024];
    int newSize = (int)stream.length;
    int oldSize = (int)r.rtttlInput.size();
    int diffSize = oldSize - newSize;
    sprintf(info, "Saved %d bytes. Now using %d bytes instead of %d bytes.", diffSize, newSize, oldSize);
    r.info = info;
  }
  else
  {
    r.result = NOT_SUPPORTED;
  }

  //set result back to model
  m.setRequest(r);
}

void Controller::toBinary16()
{
  //Update libRTTTL settings
  applyRtttlOptions();

  Model & m = Model::getInstance();

  //Update model for request
  Model::MODEL_REQUEST r = m.getRequest();

  //Convert string to RTTTL_SONG
  RTTTL_SONG rtttl;
  reset(rtttl);
  const char * code = r.rtttlInput.c_str();
  RTTTL_RESULT result = parseRtttlString(code, rtttl);
  if (result != SUCCESS)
  {
    r.result = result;
    r.errorDescription = getErrorDescription(r.result);

    //set result back to model
    m.setRequest(r);
    return;
  }

  //Force song to use anyBpm instead of bpmIdx
  //since arduino only supports anyBpm
  if (!rtttl.defaults.hasAnyBpm)
  {
    rtttl.defaults.anyBpm = getOfficialBpmFromIndex(rtttl.defaults.bpmIdx);
    rtttl.defaults.hasAnyBpm = true;
  }

  //Convert song to stream
  STREAM stream;
  toStream16(rtttl, stream);

  //convert stream to c++ code
  std::string cppCode = toCppArray(stream);

  //capitalize the name of the song
  rtttl.name[0] = toupper( rtttl.name[0] );

  //success ?
  if ( stream.length > 0 )
  {
    r.result = SUCCESS;

    //append arduino code for decoding RTTTL data
    r.rtttlOutput = "//#define RTTTL_SERIAL_DEBUG\n\n";
    appendArduinoBinRtttlFile(r.rtttlOutput);
    r.rtttlOutput.append("\n");
    appendArduinoPlay16BitsRtttlFile(r.rtttlOutput);
    r.rtttlOutput.append("\n");

    //build a cpp friendly code for extracting data
    char buffer[10240];
    sprintf(buffer, "//rtttl 16 bits binary format for the following: %s\n" \
                    "const unsigned char %s[] = %s;\n" \
                    "const int %s_note_length = %d;\n",
                    code, rtttl.name, cppCode.c_str(), rtttl.name, rtttl.notes.size());
    r.rtttlOutput.append(buffer);

    //build information message
    char info[1024];
    int newSize = (int)stream.length;
    int oldSize = (int)r.rtttlInput.size();
    int diffSize = oldSize - newSize;
    sprintf(info, "Saved %d bytes. Now using %d bytes instead of %d bytes.", diffSize, newSize, oldSize);
    r.info = info;
  }
  else
  {
    r.result = NOT_SUPPORTED;
  }

  //set result back to model
  m.setRequest(r);
}

std::string Controller::getErrorDescription(RTTTL_RESULT iResult)
{
  switch(iResult)
  {
  case SUCCESS:
    return "Operation completed successfully.";
    break;
  case NOT_SUPPORTED:
    return "Operation not supported.";
    break;
  case FAILED_OCTAVE_OFFSETTING:
    return "Unable to increase/decrease octave to get all notes within RTTTL accepted range (octave 4 to 7).";
    break;
  case FAILED_TONE_FREQUENCIES_CLAMPING:
    return "Unable to clamp all notes within RTTTL accepted frequencies (262Hz to 3951Hz).";
    break;
  case TONE_FREQUENCIES_OUT_OF_BOUNDS:
    return "Notes frequencies outside of accepted RTTTL frequency range (262Hz to 3951Hz). Try again with Octave Offsetting or Frequency Clamping enabled.";
    break;
  case BPM_OUT_OF_BOUNDS:
    return "BPM (Beats per minutes) outside of accepted RTTTL range (1 to 900).";
    break;
  case UNKNOWN_BPM:
    return "BPM (Beats per minutes) values not within official BPM. Try again with AnyBPM enabled. " \
           "Official BPM values are 25, 28, 31, 35, 40, 45, 50, 56, 63, 70, 80, 90, 100, 112, 125, 140, 160, 180, 200, 225, 250, 285, 320, 355, 400, 450, 500, 565, 635, 715, 800, 900";
    break;
  default:
    return "Unknown error.";
    break;
  };
}

void Controller::appendArduinoBinRtttlFile(std::string & ioBuffer)
{
  bin2cpp::File & file = bin2cpp::getBinRtttlFile();
  char * content = file.newBuffer();
  size_t contentSize = file.getSize();
  if (content)
  {
    //make sure the buffer is '\0' terminated
    char tmp = content[contentSize-1];
    content[contentSize-1] = '\0';

    //insert content into buffer
    ioBuffer.append(content);

    //insert last character
    ioBuffer.append(1, tmp);

    delete content;
  }
}

void Controller::appendArduinoPlay10BitsRtttlFile(std::string & ioBuffer)
{
  bin2cpp::File & file = bin2cpp::getPlay10BitsRtttlFile();
  char * content = file.newBuffer();
  size_t contentSize = file.getSize();
  if (content)
  {
    //make sure the buffer is '\0' terminated
    char tmp = content[contentSize-1];
    content[contentSize-1] = '\0';

    //insert content into buffer
    ioBuffer.append(content);

    //insert last character
    ioBuffer.append(1, tmp);

    delete content;
  }
}

void Controller::appendArduinoPlay16BitsRtttlFile(std::string & ioBuffer)
{
  bin2cpp::File & file = bin2cpp::getPlay16BitsRtttlFile();
  char * content = file.newBuffer();
  size_t contentSize = file.getSize();
  if (content)
  {
    //make sure the buffer is '\0' terminated
    char tmp = content[contentSize-1];
    content[contentSize-1] = '\0';

    //insert content into buffer
    ioBuffer.append(content);

    //insert last character
    ioBuffer.append(1, tmp);

    delete content;
  }
}

void Controller::applyRtttlOptions()
{
  Model::MODEL_REQUEST r = Model::getInstance().getRequest();
  setRtttlSettings(r.rtttlSettings);
}