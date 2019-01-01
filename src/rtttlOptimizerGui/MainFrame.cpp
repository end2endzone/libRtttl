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
