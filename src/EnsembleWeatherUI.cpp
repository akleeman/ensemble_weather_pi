///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "EnsembleWeatherUI.h"

///////////////////////////////////////////////////////////////////////////

EnsembleWeatherUI::EnsembleWeatherUI( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_SpotDisplay = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_SpotDisplay->SetScrollRate( 5, 5 );
	bSizer1->Add( m_SpotDisplay, 1, wxEXPAND | wxALL, 5 );
	
	m_open_button = new wxButton( this, wxID_ANY, wxT("open"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_open_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_SpotDisplay->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( EnsembleWeatherUI::OnSpotDoubleClick ), NULL, this );
	m_SpotDisplay->Connect( wxEVT_PAINT, wxPaintEventHandler( EnsembleWeatherUI::OnPaintSpot ), NULL, this );
	m_SpotDisplay->Connect( wxEVT_SIZE, wxSizeEventHandler( EnsembleWeatherUI::OnSizeSpot ), NULL, this );
	m_open_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnOpenFile ), NULL, this );
}

EnsembleWeatherUI::~EnsembleWeatherUI()
{
	// Disconnect Events
	m_SpotDisplay->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( EnsembleWeatherUI::OnSpotDoubleClick ), NULL, this );
	m_SpotDisplay->Disconnect( wxEVT_PAINT, wxPaintEventHandler( EnsembleWeatherUI::OnPaintSpot ), NULL, this );
	m_SpotDisplay->Disconnect( wxEVT_SIZE, wxSizeEventHandler( EnsembleWeatherUI::OnSizeSpot ), NULL, this );
	m_open_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnOpenFile ), NULL, this );
	
}
