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
	m_SpotDisplay->SetMinSize( wxSize( -1,270 ) );
	
	bSizer1->Add( m_SpotDisplay, 1, wxEXPAND | wxALL, 5 );
	
	wxGridSizer* m_lower_bar_sizer;
	m_lower_bar_sizer = new wxGridSizer( 0, 2, 0, 0 );
	
	m_lower_bar_sizer->SetMinSize( wxSize( -1,30 ) ); 
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	fgSizer3->SetMinSize( wxSize( -1,30 ) ); 
	m_open_button = new wxButton( this, wxID_ANY, wxT("open"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_open_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_forecast_file_text = new wxStaticText( this, wxID_ANY, wxT("Forecast File"), wxDefaultPosition, wxDefaultSize, 0 );
	m_forecast_file_text->Wrap( -1 );
	fgSizer3->Add( m_forecast_file_text, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	m_lower_bar_sizer->Add( fgSizer3, 1, wxALIGN_CENTER_VERTICAL, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	fgSizer2->SetMinSize( wxSize( -1,30 ) ); 
	m_time_text = new wxStaticText( this, wxID_ANY, wxT("Forecast Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_time_text->Wrap( -1 );
	fgSizer2->Add( m_time_text, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_prev_time_button = new wxButton( this, wxID_ANY, wxT("<"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer2->Add( m_prev_time_button, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	m_next_time_button = new wxButton( this, wxID_ANY, wxT(">"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer2->Add( m_next_time_button, 0, wxALL, 5 );
	
	
	m_lower_bar_sizer->Add( fgSizer2, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer1->Add( m_lower_bar_sizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( EnsembleWeatherUI::OnClose ) );
	m_SpotDisplay->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( EnsembleWeatherUI::OnSpotDoubleClick ), NULL, this );
	m_SpotDisplay->Connect( wxEVT_PAINT, wxPaintEventHandler( EnsembleWeatherUI::OnPaintSpot ), NULL, this );
	m_open_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnOpenFile ), NULL, this );
	m_prev_time_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnPrevTimeClick ), NULL, this );
	m_next_time_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnNextTimeClick ), NULL, this );
}

EnsembleWeatherUI::~EnsembleWeatherUI()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( EnsembleWeatherUI::OnClose ) );
	m_SpotDisplay->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( EnsembleWeatherUI::OnSpotDoubleClick ), NULL, this );
	m_SpotDisplay->Disconnect( wxEVT_PAINT, wxPaintEventHandler( EnsembleWeatherUI::OnPaintSpot ), NULL, this );
	m_open_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnOpenFile ), NULL, this );
	m_prev_time_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnPrevTimeClick ), NULL, this );
	m_next_time_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnsembleWeatherUI::OnNextTimeClick ), NULL, this );
	
}
