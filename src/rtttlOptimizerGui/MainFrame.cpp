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

#include "MainFrame.h"
#include "Controller.h"

MainFrame::MainFrame(wxWindow *parent)
: MainFrameBase(parent)
{
  //Allow wxPanel to forward events up to wxFrame.
  this->connectKeyDownEvent(this);

  SetIcon(wxIcon(wxT("appicon"), wxBITMAP_TYPE_ICO_RESOURCE));

  Model & m = Model::getInstance();
  Controller & c = Controller::getInstance();

#ifdef _DEBUG
  //Force existing RTTTL data for debugging
  Model::MODEL_REQUEST r = m.getRequest();
  r.rtttlInput = "tetris:d=4,o=5,b=160:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6,8e6,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,a";
  m.setRequest(r);

  this->refreshControls();
#endif

  this->Centre( wxBOTH );
}

MainFrame::~MainFrame()
{
}

void MainFrame::connectKeyDownEvent(wxWindow* iComponent)
{
  if(iComponent)
  {
    iComponent->Connect(wxID_ANY,
                        wxEVT_KEY_DOWN,
                        wxKeyEventHandler(MainFrame::onKeyDown),
                        (wxObject*) NULL,
                        this);

    wxWindowListNode* wNode = iComponent->GetChildren().GetFirst();
    while(wNode)
    {
      wxWindow* wChild = wNode->GetData();
      this->connectKeyDownEvent(wChild);

      wNode = wNode->GetNext();
    }
  }
}

void MainFrame::processSelectAll(wxKeyEvent & iEvent)
{
  processTextCtrlSelectAll(iEvent, m_txtOutput);
  processTextCtrlSelectAll(iEvent, m_txtInput);
}

void MainFrame::processTextCtrlSelectAll(wxKeyEvent & iEvent, wxTextCtrl * iTextCtrl)
{
  if ( iEvent.ControlDown() && (iEvent.GetKeyCode() == 'A' || iEvent.GetKeyCode() == 'a') )
  {
    wxObject * source = iEvent.GetEventObject();
    if (source == iTextCtrl)
    {
      iTextCtrl->SelectAll();
      iEvent.Skip();
    }
  }
}

void MainFrame::onKeyDown(wxKeyEvent& event)
{
  // insert your code here
  if (event.GetKeyCode() == 27) //ESC
  {
    Destroy();
  }
  else
  {
    event.Skip();
  }

  //Allow CTRL+A on textboxes
  processSelectAll(event);
}

void MainFrame::onClose( wxCloseEvent& event )
{
  Model & m = Model::getInstance();
  Controller & c = Controller::getInstance();

  //Continue with normal close
  event.Skip();
}

void MainFrame::onPlay( wxCommandEvent& event )
{
  Model & m = Model::getInstance();
  Controller & c = Controller::getInstance();

  //Update model for request
  updateModel();

  //ask controller to update
  c.play();

  //and update
  refreshControls();

  event.Skip();
}

void MainFrame::onMidi( wxCommandEvent& event )
{
  Model & m = Model::getInstance();
  Controller & c = Controller::getInstance();

  //Update model for request
  updateModel();

  //ask controller to update
  c.toMidi();

  //and update
  refreshControls();

  event.Skip();
}

void MainFrame::onOptimize( wxCommandEvent& event )
{
  Model & m = Model::getInstance();
  Controller & c = Controller::getInstance();

  //Update model for request
  updateModel();

  //ask controller to update
  c.optimize();

  //and update
  refreshControls();

  event.Skip();
}

void MainFrame::onToTone( wxCommandEvent& event )
{
  Model & m = Model::getInstance();
  Controller & c = Controller::getInstance();

  //Update model for request
  updateModel();

  //ask controller to update
  c.toTone();

  //and update
  refreshControls();

  event.Skip();
}

void MainFrame::onFromTone( wxCommandEvent& event )
{
  Model & m = Model::getInstance();
  Controller & c = Controller::getInstance();

  //Update model for request
  updateModel();

  //ask controller to update
  c.fromTone();

  //and update
  refreshControls();

  event.Skip();
}

void MainFrame::onBinary10( wxCommandEvent& event )
{
  Model & m = Model::getInstance();
  Controller & c = Controller::getInstance();

  //Update model for request
  updateModel();

  //ask controller to update
  c.toBinary10();

  //and update
  refreshControls();

  event.Skip();
}

void MainFrame::onBinary16( wxCommandEvent& event )
{
  Model & m = Model::getInstance();
  Controller & c = Controller::getInstance();

  //Update model for request
  updateModel();

  //ask controller to update
  c.toBinary16();

  //and update
  refreshControls();

  event.Skip();
}

void MainFrame::updateModel()
{
  Model & m = Model::getInstance();

  //Update model for request
  m.reset();
  Model::MODEL_REQUEST r = m.getRequest();

  r.rtttlSettings.forceDefaultSection = m_chkForceDefaultSection->GetValue();
  r.rtttlSettings.allowAnyBpm = m_chkAllowAnyBpm->GetValue();
  r.rtttlSettings.allowToneFrequencyClamping = m_chkToneClamping->GetValue();
  r.rtttlSettings.allowOctaveOffseting = m_chkOctaveOffseting->GetValue();

  r.rtttlInput = m_txtInput->GetValue().wx_str();

  m.setRequest(r);
}

void MainFrame::refreshControls()
{
  Model & m = Model::getInstance();
  Model::MODEL_REQUEST r = m.getRequest();

  m_txtInput->SetValue( r.rtttlInput.c_str() );
  
  if (r.result == SUCCESS)
  {
    m_txtOutput->SetValue( r.rtttlOutput.c_str() );
  }
  else
  {
    m_txtOutput->SetValue( r.errorDescription.c_str() );
  }
  
  m_statusBar->SetLabel( r.info.c_str() );
}
