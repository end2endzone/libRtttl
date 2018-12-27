#pragma once

#include <string>
#include "libRTTTL.h"
#include "rtttl.h"

class Model
{
private:
  Model();
public:
  static Model & getInstance();
  virtual ~Model();

  void reset();

  struct MODEL_REQUEST
  {
    RTTTL_SETTINGS rtttlSettings;
    std::string rtttlInput;
    std::string rtttlOutput;
    RTTTL_RESULT result;
    std::string errorDescription;
    std::string info;
  };
  static void resetRequest(MODEL_REQUEST & oRequest);

  //Public methods
  const MODEL_REQUEST & getRequest() const;
  void setRequest(const MODEL_REQUEST & iRequest);

private:
  MODEL_REQUEST mRequest;
};
