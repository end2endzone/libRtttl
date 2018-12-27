///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GuiBase.h"

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 230,260 ), wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_panel81 = new wxPanel( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1411;
	bSizer1411 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer31;
	sbSizer31 = new wxStaticBoxSizer( new wxStaticBox( m_panel81, wxID_ANY, wxT("Options") ), wxVERTICAL );
	
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer41->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_chkForceDefaultSection = new wxCheckBox( m_panel81, wxID_ANY, wxT("Always insert default section"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkForceDefaultSection->SetValue(true); 
	bSizer41->Add( m_chkForceDefaultSection, 0, wxALL, 5 );
	
	
	bSizer41->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_chkAllowAnyBpm = new wxCheckBox( m_panel81, wxID_ANY, wxT("Allow any BPM"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( m_chkAllowAnyBpm, 0, wxALL, 5 );
	
	
	bSizer41->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_chkToneClamping = new wxCheckBox( m_panel81, wxID_ANY, wxT("Enable tone() frequency clamping"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( m_chkToneClamping, 0, wxALL, 5 );
	
	
	bSizer41->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_chkOctaveOffseting = new wxCheckBox( m_panel81, wxID_ANY, wxT("Enable octave offseting"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( m_chkOctaveOffseting, 0, wxALL, 5 );
	
	
	bSizer41->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	sbSizer31->Add( bSizer41, 1, wxEXPAND, 5 );
	
	
	bSizer1411->Add( sbSizer31, 0, wxEXPAND, 5 );
	
	
	m_panel81->SetSizer( bSizer1411 );
	m_panel81->Layout();
	bSizer1411->Fit( m_panel81 );
	bSizer3->Add( m_panel81, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	m_panel22 = new wxPanel( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_panel22, wxID_ANY, wxT("RTTTL Input") ), wxVERTICAL );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_txtInput = new wxTextCtrl( m_panel22, wxID_ANY, wxT("Zelda1:d=4,o=5,b=125:a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,16a#.6,16g#6,16f#.6,8g#.6,16f#.6,2f6,f6,8d#6,16d#6,16f6,2f#6,8f6,8d#6,8c#6,16c#6,16d#6,2f6,8d#6,8c#6,8c6,16c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f,a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,c#7,c7,2a6,f6,2f#.6,a#6,a6,2f6,f6,2f#.6,a#6,a6,2f6,d6,2d#.6,f#6,f6,2c#6,a#,c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_WORDWRAP|wxVSCROLL );
	bSizer15->Add( m_txtInput, 1, wxALL|wxEXPAND, 5 );
	
	
	sbSizer6->Add( bSizer15, 1, wxEXPAND, 5 );
	
	
	bSizer14->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	
	m_panel22->SetSizer( bSizer14 );
	m_panel22->Layout();
	bSizer14->Fit( m_panel22 );
	bSizer3->Add( m_panel22, 1, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_panel8 = new wxPanel( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panel8, wxID_ANY, wxT("Commands") ), wxVERTICAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnPlay = new wxButton( m_panel8, wxID_ANY, wxT("Play"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer4->Add( m_btnPlay, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnOptimize = new wxButton( m_panel8, wxID_ANY, wxT("Optimize"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer4->Add( m_btnOptimize, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnDecode = new wxButton( m_panel8, wxID_ANY, wxT("To tone()"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer4->Add( m_btnDecode, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnFromTone = new wxButton( m_panel8, wxID_ANY, wxT("From tone()"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer4->Add( m_btnFromTone, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnToBinary10Bits = new wxButton( m_panel8, wxID_ANY, wxT("To binary (10 bits)"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer4->Add( m_btnToBinary10Bits, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnToBinary16Bits = new wxButton( m_panel8, wxID_ANY, wxT("To binary (16 bits)"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer4->Add( m_btnToBinary16Bits, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	sbSizer3->Add( bSizer4, 1, wxEXPAND, 5 );
	
	
	bSizer141->Add( sbSizer3, 0, wxEXPAND, 5 );
	
	
	m_panel8->SetSizer( bSizer141 );
	m_panel8->Layout();
	bSizer141->Fit( m_panel8 );
	bSizer3->Add( m_panel8, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_panel221 = new wxPanel( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer142;
	bSizer142 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer61;
	sbSizer61 = new wxStaticBoxSizer( new wxStaticBox( m_panel221, wxID_ANY, wxT("RTTTL Output") ), wxVERTICAL );
	
	wxBoxSizer* bSizer151;
	bSizer151 = new wxBoxSizer( wxVERTICAL );
	
	m_txtOutput = new wxTextCtrl( m_panel221, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_WORDWRAP|wxVSCROLL );
	bSizer151->Add( m_txtOutput, 1, wxALL|wxEXPAND, 5 );
	
	
	sbSizer61->Add( bSizer151, 1, wxEXPAND, 5 );
	
	
	bSizer142->Add( sbSizer61, 1, wxEXPAND, 5 );
	
	
	m_panel221->SetSizer( bSizer142 );
	m_panel221->Layout();
	bSizer142->Fit( m_panel221 );
	bSizer3->Add( m_panel221, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	
	m_panel2->SetSizer( bSizer3 );
	m_panel2->Layout();
	bSizer3->Fit( m_panel2 );
	bSizer2->Add( m_panel2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer2 );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::onClose ) );
	m_btnPlay->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::onPlay ), NULL, this );
	m_btnOptimize->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::onOptimize ), NULL, this );
	m_btnDecode->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::onToTone ), NULL, this );
	m_btnFromTone->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::onFromTone ), NULL, this );
	m_btnToBinary10Bits->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::onBinary10 ), NULL, this );
	m_btnToBinary16Bits->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::onBinary16 ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::onClose ) );
	m_btnPlay->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::onPlay ), NULL, this );
	m_btnOptimize->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::onOptimize ), NULL, this );
	m_btnDecode->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::onToTone ), NULL, this );
	m_btnFromTone->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::onFromTone ), NULL, this );
	m_btnToBinary10Bits->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::onBinary10 ), NULL, this );
	m_btnToBinary16Bits->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::onBinary16 ), NULL, this );
	
}
