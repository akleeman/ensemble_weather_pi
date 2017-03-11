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
	
	m_open_button = new wxButton( this, wxID_ANY, wxT("open"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_open_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_open_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnOpenFile ), NULL, this );
}

EnsembleWeatherUI::~EnsembleWeatherUI()
{
	// Disconnect Events
	m_open_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnOpenFile ), NULL, this );
	
}
