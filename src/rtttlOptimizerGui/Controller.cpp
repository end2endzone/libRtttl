/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

#include "Controller.h"
#include "Model.h"
#include "tone.h"
#include "beep.h"
#include "libMidi.h"
#include "anyrtttl.h"

//Undefine the following from arduino.h
//which generates the following issues:
//  1>c:\program files\microsoft sdks\windows\v7.0\include\winuser.h(5341) : warning C4091: 'typedef ' : ignored on left of 'tagINPUT' when no variable is declared
//  1>c:\program files\microsoft sdks\windows\v7.0\include\winuser.h(5341) : error C2143: syntax error : missing ';' before 'constant'
//  1>c:\program files\microsoft sdks\windows\v7.0\include\winuser.h(5341) : error C2059: syntax error : 'constant'
//  1>c:\program files\microsoft sdks\windows\v7.0\include\winuser.h(5348) : error C2061: syntax error : identifier 'LPINPUT'
#undef INPUT

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

void Controller::toMidi()
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
  TONE_CALL_INFO_LIST calls = playRTTTL(code);

  //Remove all delay() calls which matches a tone() call.
  removeExpectedDelays(calls);

  //got something to play
  if (calls.size() == 0)
  {
    r.result = UNKNOWN;
    r.errorDescription = "Nothing to play";

    //set result back to model
    m.setRequest(r);
    return;
  }

  //Configure a MIDI file output
  MidiFile midi;
  midi.setInstrument(0x50); //Lead 1 (square)
  midi.setMidiType(MidiFile::MIDI_TYPE_0);
  midi.setBeatsPerMinute(rtttl.defaults.bpm);
  midi.setName(rtttl.name);
  midi.setVolume(0x64);

  //forward each calls to the MidiFile module
  int numNotes = 0;
  for(size_t i=0; i<calls.size(); i++)
  {
    const TONE_CALL_INFO & c = calls[i];
    if (c._tone != INVALID_TONE_INFO)
    {
      midi.addNote(c._tone.freq, c._tone.duration);
    }
    if (c.delay != INVALID_TONE_DURATION)
    {
      midi.addDelay(c.delay);
    }
  }

  //adding an additional 250ms delay so that MIDI files does not abruptly end
  midi.addDelay(250); //adding a 250ms

  //build a temporary file path
  std::string outputFile;
  outputFile = getenv("TEMP");
#ifdef _WIN32
  outputFile.append("\\");
#else
  outputFile.append("/");
#endif
  outputFile.append("rtttlOptimizerGui.mid");

  //save
  bool saved = midi.save(outputFile.c_str());
  if (!saved)
  {
    r.result = NOT_SUPPORTED;
    r.errorDescription = std::string("Failed saving RTTTL to MIDI file ") + outputFile;

    //set result back to model
    m.setRequest(r);
    return;
  }

  //now play the file
  bool playSuccess = (playMidiFile(outputFile.c_str()) == 0);
  if (!playSuccess)
  {
    r.result = UNKNOWN;
    r.errorDescription = std::string("Failed play MIDI file ") + outputFile;

    //set result back to model
    m.setRequest(r);
    return;
  }

  //success ?
  if ( calls.size() > 0 )
  {
    r.result = SUCCESS;
    r.rtttlOutput = std::string("Playing MIDI file ") + outputFile;

    char info[1024];
    sprintf(info, "Playing %d notes.", calls.size());
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
    r.rtttlOutput.append("  noTone(pin);\n");
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

  //Convert song to stream
  STREAM stream;
  toStream10(rtttl, stream);

  //convert stream to c++ code
  std::string cppCode = toCppArray(stream);

  //lowercase the name of the song
  rtttl.name[0] = tolower( rtttl.name[0] );

  //success ?
  if ( stream.length > 0 )
  {
    r.result = SUCCESS;

    //append arduino code for decoding RTTTL data
    r.rtttlOutput = "";
    r.rtttlOutput.append("//Notes:\n");
    r.rtttlOutput.append("// 10 Bits RTTTL format requires AnyRtttl & BitReader libraries.\n");
    r.rtttlOutput.append("// The code & updates for AnyRtttl & BitReader libraries can be found on http://end2endzone.com\n");
    r.rtttlOutput.append("#include <anyrtttl.h>\n");
    r.rtttlOutput.append("#include <binrtttl.h>\n");
    r.rtttlOutput.append("#include <pitches.h>\n");
    r.rtttlOutput.append("\n");
    r.rtttlOutput.append("#include <bitreader.h>\n");
    r.rtttlOutput.append("\n");
    r.rtttlOutput.append("//#define ANY_RTTTL_INFO\n");
    r.rtttlOutput.append("//#define ANY_RTTTL_DEBUG\n");
    r.rtttlOutput.append("\n");

    //build a cpp friendly code for extracting data
    char buffer[10240];
    sprintf(buffer, "//RTTTL 10 bits binary format for the following: %s\n" \
                    "//Compatible with AnyRtttl library v%2.1f\n" \
                    "const unsigned char %s[] = %s;\n" \
                    "const int %s_length = %d;\n",
                    code, ANY_RTTTL_VERSION, rtttl.name, cppCode.c_str(), rtttl.name, rtttl.notes.size());
    r.rtttlOutput.append(buffer);
    r.rtttlOutput.append("\n");

    //add the BitReader required code.
    r.rtttlOutput.append("//BitReader support\n");
    r.rtttlOutput.append("#ifndef USE_BITADDRESS_READ_WRITE\n");
    r.rtttlOutput.append("BitReader bitreader;\n");
    r.rtttlOutput.append("#else\n");
    r.rtttlOutput.append("BitAddress bitreader;\n");
    r.rtttlOutput.append("#endif\n");
    r.rtttlOutput.append("\n");

    sprintf(buffer, "//the following function reads numBits from the %s buffer\n", rtttl.name);
    r.rtttlOutput.append(buffer);

    r.rtttlOutput.append("uint16_t readNextBits(uint8_t numBits)\n");
    r.rtttlOutput.append("{\n");
    r.rtttlOutput.append("  uint16_t bits = 0;\n");
    r.rtttlOutput.append("  bitreader.read(numBits, &bits);\n");
    r.rtttlOutput.append("  return bits;\n");
    r.rtttlOutput.append("}\n");
    r.rtttlOutput.append("\n");

    r.rtttlOutput.append("//Add the following in setup()\n");
    sprintf(buffer, "//bitreader.setBuffer(%s);\n", rtttl.name);
    r.rtttlOutput.append(buffer);
    r.rtttlOutput.append("\n");

    sprintf(buffer, "//Call the following for playing %s melody\n", rtttl.name);
    r.rtttlOutput.append(buffer);
    sprintf(buffer, "//anyrtttl::blocking::play10Bits(BUZZER_PIN, %s_length, &readNextBits);\n", rtttl.name);
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

  //Convert song to stream
  STREAM stream;
  toStream16(rtttl, stream);

  //convert stream to c++ code
  std::string cppCode = toCppArray(stream);

  //lowercase the name of the song
  rtttl.name[0] = tolower( rtttl.name[0] );

  //success ?
  if ( stream.length > 0 )
  {
    r.result = SUCCESS;

    //append arduino code for decoding RTTTL data
    r.rtttlOutput = "";
    r.rtttlOutput.append("//Notes:\n");
    r.rtttlOutput.append("// 16 Bits RTTTL format requires AnyRtttl library.\n");
    r.rtttlOutput.append("// The code & updates for AnyRtttl library can be found on http://end2endzone.com\n");
    r.rtttlOutput.append("#include <anyrtttl.h>\n");
    r.rtttlOutput.append("#include <binrtttl.h>\n");
    r.rtttlOutput.append("#include <pitches.h>\n");
    r.rtttlOutput.append("\n");
    r.rtttlOutput.append("//#define ANY_RTTTL_INFO\n");
    r.rtttlOutput.append("//#define ANY_RTTTL_DEBUG\n");
    r.rtttlOutput.append("\n");

    //build a cpp friendly code for extracting data
    char buffer[10240];
    sprintf(buffer, "//RTTTL 16 bits binary format for the following: %s\n" \
                    "//Compatible with AnyRtttl library v%2.1f\n" \
                    "const unsigned char %s[] = %s;\n" \
                    "const int %s_length = %d;\n",
                    code, ANY_RTTTL_VERSION, rtttl.name, cppCode.c_str(), rtttl.name, rtttl.notes.size());
    r.rtttlOutput.append(buffer);
    r.rtttlOutput.append("\n");

    sprintf(buffer, "//Call the following for playing %s melody\n", rtttl.name);
    r.rtttlOutput.append(buffer);
    sprintf(buffer, "//anyrtttl::blocking::play16Bits(BUZZER_PIN, %s, %s_length);\n", rtttl.name, rtttl.name);
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

void Controller::applyRtttlOptions()
{
  Model::MODEL_REQUEST r = Model::getInstance().getRequest();
  setRtttlSettings(r.rtttlSettings);
}