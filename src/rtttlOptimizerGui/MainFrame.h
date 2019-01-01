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

#ifndef MAINFRAME_H
#define MAINFRAME_H

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
	virtual void onMidi( wxCommandEvent& event );
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

#endif //MAINFRAME_H
