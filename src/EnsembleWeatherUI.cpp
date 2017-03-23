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
	this->SetSizeHints( wxSize( 600,300 ), wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_SpotDisplay = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer1->Add( m_SpotDisplay, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer31;
	fgSizer31 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer31->SetFlexibleDirection( wxBOTH );
	fgSizer31->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_open_button = new wxButton( this, wxID_ANY, wxT("open"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_open_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_forecast_file_text = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_forecast_file_text->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer3->Add( m_forecast_file_text, 1, wxALL, 5 );
	
	
	fgSizer31->Add( fgSizer3, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_prev_time_button = new wxButton( this, wxID_ANY, wxT("<"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer2->Add( m_prev_time_button, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	m_next_time_button = new wxButton( this, wxID_ANY, wxT(">"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer2->Add( m_next_time_button, 0, wxALL, 5 );
	
	m_time_text = new wxStaticText( this, wxID_ANY, wxT("Forecast Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_time_text->Wrap( -1 );
	fgSizer2->Add( m_time_text, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	
	fgSizer31->Add( fgSizer2, 0, wxALIGN_RIGHT, 5 );
	
	
	bSizer1->Add( fgSizer31, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_SpotDisplay->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( EnsembleWeatherUI::OnSpotDoubleClick ), NULL, this );
	m_SpotDisplay->Connect( wxEVT_PAINT, wxPaintEventHandler( EnsembleWeatherUI::OnPaintSpot ), NULL, this );
	m_open_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnOpenFile ), NULL, this );
	m_prev_time_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnPrevTimeClick ), NULL, this );
	m_next_time_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnNextTimeClick ), NULL, this );
}

EnsembleWeatherUI::~EnsembleWeatherUI()
{
	// Disconnect Events
	m_SpotDisplay->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( EnsembleWeatherUI::OnSpotDoubleClick ), NULL, this );
	m_SpotDisplay->Disconnect( wxEVT_PAINT, wxPaintEventHandler( EnsembleWeatherUI::OnPaintSpot ), NULL, this );
	m_open_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnOpenFile ), NULL, this );
	m_prev_time_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnPrevTimeClick ), NULL, this );
	m_next_time_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnNextTimeClick ), NULL, this );
	
}
