#pragma once

#include "Model.h"
#include "rtttl.h"

class Controller
{
private:
  Controller();
public:
  static Controller & getInstance();
  virtual ~Controller();

  //Public methods
  void play();
  void optimize();
  void toTone();
  void fromTone();
  void toBinary10();
  void toBinary16();

  std::string getErrorDescription(RTTTL_RESULT iResult);

private:
  void applyRtttlOptions();
  void appendArduinoBinRtttlFile(std::string & ioValue);
  void appendArduinoPlay10BitsRtttlFile(std::string & ioValue);
  void appendArduinoPlay16BitsRtttlFile(std::string & ioValue);
};
