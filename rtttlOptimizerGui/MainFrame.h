#pragma once

#ifndef WX_PRECOMP
  #include <wx/wx.h>
  #include <wx/frame.h>
#else
  #include <wx/wxprec.h>
#endif

#include "GuiBase.h"
#include "Model.h"
#include "Controller.h"

class MainFrame :  public MainFrameBase
{
public:
  //Constructor/Destructor
  MainFrame(wxWindow *parent);
  virtual ~MainFrame();

  //Public methods
  // N/A

  //Event handlers
	virtual void onClose( wxCloseEvent& event );
	virtual void onPlay( wxCommandEvent& event );
	virtual void onOptimize( wxCommandEvent& event );
	virtual void onToTone( wxCommandEvent& event );
	virtual void onFromTone( wxCommandEvent& event );
	virtual void onBinary10( wxCommandEvent& event );
	virtual void onBinary16( wxCommandEvent& event );

private:
  void connectKeyDownEvent(wxWindow* iComponent);
  void processSelectAll(wxKeyEvent & iEvent);
  void processTextCtrlSelectAll(wxKeyEvent & iEvent, wxTextCtrl * iTextCtrl);
  void onKeyDown(wxKeyEvent& event);

private:
  void updateModel();
  void refreshControls();
};
