#include "Model.h"

Model::Model()
{
  reset();
}

Model & Model::getInstance()
{
  static Model model;
  return model;
}

Model::~Model()
{
  reset();
}

void Model::reset()
{
  resetRequest(mRequest);
}

void Model::resetRequest(MODEL_REQUEST & oRequest)
{
  MODEL_REQUEST & r = oRequest;
  r.rtttlSettings.forceDefaultSection = false;
  r.rtttlSettings.allowAnyBpm = false;
  r.rtttlSettings.allowToneFrequencyClamping = false;
  r.rtttlSettings.allowOctaveOffseting = false;
  r.rtttlInput = "";
  r.rtttlOutput = "";
  r.result = SUCCESS;
  r.errorDescription = "";
  r.info = "";
}

const Model::MODEL_REQUEST & Model::getRequest() const
{
  return mRequest;
}

void Model::setRequest(const MODEL_REQUEST & iRequest)
{
  mRequest = iRequest;
}
