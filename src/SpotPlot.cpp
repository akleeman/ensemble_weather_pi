#include "SpotPlot.h"


bool SpotPlot::is_initialized() {
    return m_window == nullptr;
}


void SpotPlot::create_grid(wrDC &wrdc, int n_x, int n_y){
    int width, height;
    wrdc.GetSize(&width, &height);

    std::vector<double> xs;

    double padded_width = width - 1 - m_y_label_width - m_color_bar_width;
    m_grid_width = padded_width / (float) n_x;

    m_grid_x.clear();
    for (int i=0; i<n_x+1; i++){
      m_grid_x.push_back(m_y_label_width + m_grid_width * i);
    }

    m_grid_y.clear();
    m_grid_height = (height - 1 - m_x_label_height - m_header_height) / (float) n_y;
    for (int i=0; i<n_y+1; i++){
      m_grid_y.push_back(m_x_label_height + m_grid_height * (n_y - i));
    }
}


int SpotPlot::get_time_index(const wxPoint &point) {
    return bisect(m_grid_x, point.x) - 1;
}


void SpotPlot::draw_grid(wrDC &wrdc, SpotForecast *fcst){
    int width, height;
    wrdc.GetSize(&width, &height);
    wxBrush bg_brush(m_bg_color, wxALPHA_OPAQUE);
    wrdc.SetBrush(bg_brush);

    wrdc.DrawRectangle(0., 0., width, height);

    wxColour grey(200, 200, 200);
    wxPen pen(grey);
    wrdc.SetPen(pen);

    for (uint i=0; i<m_grid_x.size(); i++){
        wrdc.DrawLine(m_grid_x[i], m_grid_y[0],
                      m_grid_x[i], m_grid_y.back());
    }

    for (uint i=0; i<m_grid_y.size(); i++){
        wrdc.DrawLine(m_grid_x[0], m_grid_y[i],
                      m_grid_x.back() + m_grid_width, m_grid_y[i]);
    }

    std::vector<time_t> times = *(fcst->get_times());

    int w, h, decent, external;
    wxFont font = wrdc.GetFont();
    char s[3];
    wxDateTime first(times[0]);
    int last_text_right=0;
    int x_pos;
    for (uint i = 0; i < times.size(); i++) {
        int hours = (wxDateTime(times[i]) - first).GetHours();
        sprintf(s, "%3d", hours);
        wxString label(s);
        wrdc.GetTextExtent(label, &w, &h, &decent, &external, &font);
        x_pos = m_grid_x[i] - w / 2.;
        if (x_pos > last_text_right) {
          wrdc.DrawText(label, x_pos,
                        m_grid_y.front() + (m_x_label_height - h) / 2. + 2);
          last_text_right = x_pos + w;
        }
    }
}


void SpotPlot::draw_color_bar(wrDC &wrdc){

    for (uint i=0; i<m_grid_y.size() - 1; i++){
        wxBrush brush(wind_colors[i], wxALPHA_OPAQUE);
        wrdc.SetBrush(brush);
        wrdc.DrawRectangle(m_grid_x.back(), m_grid_y[i] - m_grid_height,
                           m_color_bar_width, m_grid_height);
        wxCoord w, h, decent, external;
        wxFont font = wrdc.GetFont();
        char s[3];
        sprintf(s, "%2.0f", wind_bins[i + 1]);
        wxString label(s);
        wrdc.GetTextExtent(label, &w, &h, &decent, &external, &font);
        wrdc.DrawText(label, m_y_label_width - w - 3., m_grid_y[i] - m_grid_height - h / 2.);

    }

}


void SpotPlot::draw_probs(wrDC &wrdc, Matrix<double> probs) {

    for (int i = 0; i < probs.shape()[0]; i++) {
        for (int j = 0; j < probs.shape()[1]; j++) {
            double a = probs.get(i, j);
            // For some reason the alpha transparency doesn't work here so
            // we manually create the effect of transparency
            wxColour color(66 * a + (1 - a) * m_bg_color.Red(),
                           119 * a + (1 - a) * m_bg_color.Green(),
                           229 * a + (1 - a) * m_bg_color.Blue());
            wxBrush brush(color, wxALPHA_OPAQUE);
            wrdc.SetBrush(brush);

            wrdc.DrawRectangle(m_grid_x[i], m_grid_y[j] - m_grid_height,
                               m_grid_width, m_grid_height);
        }
    }
}


void SpotPlot::draw_time_marker(wrDC &wrdc, int time_index) {
    wxPen pen(wxColour("red"));
    wrdc.SetPen(pen);

    wrdc.SetBrush(wxNullBrush);

    wrdc.DrawRectangle(m_grid_x[time_index], m_grid_y.back(),
                       m_grid_width, m_grid_y.front() - m_grid_y.back());
}


void SpotPlot::draw_current_time(wrDC &wrdc, SpotForecast *fcst) {

    std::vector<time_t> times = *fcst->get_times();
    int i;
    for (i = 0; i < (int) times.size(); i++) {
        if (wxDateTime(times[i]) > wxDateTime::Now()) break;
    }
    if (i == (int) times.size() || i <= 0) {
      return;
    }

    wxTimeSpan dt = wxDateTime::Now() - wxDateTime(times[i - 1]);
    wxTimeSpan grid_time_span = wxDateTime(times[i]) - wxDateTime(times[i - 1]);

    double ratio = dt.GetMinutes() / (double) grid_time_span.GetMinutes();
    double x_now = m_grid_x[i - 1] + ratio * (m_grid_x[i] - m_grid_x[i - 1]);

    wxPen pen(wxColour("blue"));
    wrdc.SetPen(pen);
    wrdc.DrawLine(x_now, m_grid_y.front(), x_now, m_grid_y.back());

}


void SpotPlot::plot(wxWindow *window, SpotForecast *fcst, int time_index){

    m_window = window;
    wxPaintDC dc(window);
    dc.SetBackgroundMode(wxTRANSPARENT);
    wrDC wrdc(dc);

    auto wind_speed = fcst->get_variable(WIND_SPEED_ID);
    auto wind_direction = fcst->get_variable(WIND_DIRECTION_ID);

    Matrix<double> *speed = wind_speed.get_data();
    Matrix<double> probs = binned_probabilities(*speed, wind_bins);
    create_grid(wrdc, probs.shape()[0], probs.shape()[1] + 1);
    draw_grid(wrdc, fcst);
    draw_color_bar(wrdc);
    draw_probs(wrdc, probs);
    draw_time_marker(wrdc, time_index);
    draw_current_time(wrdc, fcst);
}
