#include <iostream>
#include "EnsembleWeatherManager.h"


EnsembleWeatherManager::EnsembleWeatherManager(wxWindow *parent)
  : GRIBUICtrlBarBase(parent)
{
}

void EnsembleWeatherManager::Reset(){
}

void EnsembleWeatherManager::Render(wrDC &dc, PlugIn_ViewPort &vp){
  // not ws
}
