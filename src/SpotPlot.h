#include <wx/wx.h>
#include "wrdc.h"
#include "Slocum.h"
#include "WindCircle.h"
#include "VectorUtilities.h"


class SpotPlot
{
    public:

        void create_grid(wrDC &wrdc, int n_x, int n_y);

        void draw_grid(wrDC &wrdc, SpotForecast *fcst);

        void draw_color_bar(wrDC &wrdc);

        void draw_probs(wrDC &wrdc, Matrix<double> probs);

        void get_wind_probs(SpotForecast *fcst, Matrix<double> *probs);

        void draw_time_marker(wrDC &wrdc, int time_index);

        void draw_current_time(wrDC &wrdc, SpotForecast *fcst);

        void plot(wxWindow *window, SpotForecast *fcst, int time_index);

        wxWindow *get_window() { return m_window; };

        int get_time_index(const wxPoint &point);

        bool is_initialized();

    private:

        wxWindow *m_window;
        wrDC m_wrdc;
        double m_grid_height;
        double m_grid_width;
        double m_color_bar_width = 30;
        double m_header_height = 30;
        double m_x_label_height = 30;
        double m_y_label_width = 30;
        wxColour m_bg_color = wxColor(230, 230, 230);
        std::vector<double> m_grid_x, m_grid_y;
};
