///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ENSEMBLEWEATHERUI_H__
#define __ENSEMBLEWEATHERUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/scrolwin.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class EnsembleWeatherUI
///////////////////////////////////////////////////////////////////////////////
class EnsembleWeatherUI : public wxFrame 
{
	private:
	
	protected:
		wxScrolledWindow* m_SpotDisplay;
		wxButton* m_open_button;
		wxTextCtrl* m_forecast_file_text;
		wxButton* m_prev_time_button;
		wxButton* m_next_time_button;
		wxStaticText* m_time_text;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOpenFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPrevTimeClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNextTimeClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		EnsembleWeatherUI( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Ensemble Weather"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 456,358 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~EnsembleWeatherUI();
	
};

#endif //__ENSEMBLEWEATHERUI_H__
