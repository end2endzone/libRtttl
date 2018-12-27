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
