//
//  libRTTTL Library - v1.2 - 05/27/2016
//  Copyright (C) 2016 Antoine Beauchamp
//  The code & updates for the library can be found on http://end2endzone.com
//
// AUTHOR/LICENSE:
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3.0 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License (LGPL-3.0) for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// DISCLAIMER:
//  This software is furnished "as is", without technical support, and with no 
//  warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
//  The library allows one to convert an RTTTL melody to other formats like:
//  - Smaller RTTTL code
//  - Arduino code (tone() and delay() function calls)
//  - Binary RTTTL. See AnyRtttl library at http://end2endzone.com
//
//  The library also support converting arduino code (tones) to RTTTL.
//  
// HISTORY:
// 05/27/2016 v1.2 - First public release.
//

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