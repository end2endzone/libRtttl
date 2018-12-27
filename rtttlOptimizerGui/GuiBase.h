///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUIBASE_H__
#define __GUIBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panel2;
		wxPanel* m_panel81;
		wxCheckBox* m_chkForceDefaultSection;
		wxCheckBox* m_chkAllowAnyBpm;
		wxCheckBox* m_chkToneClamping;
		wxCheckBox* m_chkOctaveOffseting;
		wxPanel* m_panel22;
		wxTextCtrl* m_txtInput;
		wxPanel* m_panel8;
		wxButton* m_btnOptimize;
		wxButton* m_btnPlay;
		wxButton* m_btnMidi;
		wxButton* m_btnFromTone;
		wxButton* m_btnDecode;
		wxButton* m_btnToBinary10Bits;
		wxButton* m_btnToBinary16Bits;
		wxPanel* m_panel221;
		wxTextCtrl* m_txtOutput;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void onOptimize( wxCommandEvent& event ) { event.Skip(); }
		virtual void onPlay( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMidi( wxCommandEvent& event ) { event.Skip(); }
		virtual void onFromTone( wxCommandEvent& event ) { event.Skip(); }
		virtual void onToTone( wxCommandEvent& event ) { event.Skip(); }
		virtual void onBinary10( wxCommandEvent& event ) { event.Skip(); }
		virtual void onBinary16( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("RTTTL Optimizer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 746,451 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrameBase();
	
};

#endif //__GUIBASE_H__
