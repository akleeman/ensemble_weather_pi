///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "EnsembleWeatherUI.h"

///////////////////////////////////////////////////////////////////////////

EnsembleWeatherUIBase::EnsembleWeatherUIBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_open_button = new wxButton( this, wxID_ANY, wxT("open"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_open_button, 0, wxALL, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

EnsembleWeatherUIBase::~EnsembleWeatherUIBase()
{
}
