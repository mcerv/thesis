#include "plots.h"

Plots::Plots(const Plots& obj) {
  histFwhm_v4 = new TH1D ();////// ("FWHM", "FWHM", NBINS,0,NBINS-1);
  histFwhm_acc_v4 = new TH1D ();// ("FWHM_acc", "FWHM_acc", NBINS,0,NBINS-1);
  histFwhm_rej_v4 = new TH1D ();// ("FWHM_rej", "FWHM_rej", NBINS,0,NBINS-1);
  histMaxampl_v4 = new TH1D ();// ("Amplitude", "Amplitude", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  histMaxampl_acc_v4 = new TH1D ();// ("Amplitude_acc", "Amplitude_acc", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  histMaxampl_rej_v4 = new TH1D ();// ("Amplitude_rej", "Amplitude_rej", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  histArea_v4 = new TH1D ();// ("Area", "Area", NBINS_AREA,0,NBINS_AREA-1);
  histArea_acc_v4 = new TH1D ();// ("Area_acc", "Area_acc", NBINS_AREA,0,NBINS_AREA-1);
  histArea_acc_fit_v4 = new TH1D ();// ("Area fit", "Area fit", NBINS_AREA,0,NBINS_AREA-1);
  histArea_rej_v4 = new TH1D ();// ("Area_rej", "Area_rej", NBINS_AREA,0,NBINS_AREA-1);
  extHistArea_v5 = new TH1D ();// ("Area_v5", "Area_v5", NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  extHistArea_acc_v5 = new TH1D ();// ("ExtArea_acc", "ExtArea_acc", NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  extHistArea_rej_v5 = new TH1D ();// ("ExtArea_rej", "ExtArea_rej", NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  extHistFwhmSlope_v6 = new TH2D ();// ("Width vs Slope", "Width vs Slope",
                              //       NBINS_FWHM_SLOPE_X, 0, NBINS_FWHM_SLOPE_X-1,
                              //       NBINS_FWHM_SLOPE_Y, -NBINS_FWHM_SLOPE_Y+1, 0);
  leg = new TLegend ();// (0.65,0.75,0.9,0.9);

  *histFwhm_v4 = *obj.histFwhm_v4;
  *histFwhm_acc_v4 = *obj.histFwhm_acc_v4;
  *histFwhm_rej_v4 = *obj.histFwhm_rej_v4;
  *histMaxampl_v4 = *obj.histMaxampl_v4;
  *histMaxampl_acc_v4 = *obj.histMaxampl_acc_v4;
  *histMaxampl_rej_v4 = *obj.histMaxampl_rej_v4;
  *histArea_v4 = *obj.histArea_v4;
  *histArea_acc_v4 = *obj.histArea_acc_v4;
  *histArea_rej_v4 = *obj.histArea_rej_v4;
  *histArea_acc_fit_v4 = *obj.histArea_acc_fit_v4;
  //*leg = *obj.leg;
  *extHistArea_v5 = *obj.extHistArea_v5;
  *extHistArea_acc_v5 = *obj.extHistArea_acc_v5;
  *extHistArea_rej_v5 = *obj.extHistArea_rej_v5;
  *extHistFwhmSlope_v6 = *obj.extHistFwhmSlope_v6;
}

Plots::Plots() {
  histFwhm_v4 = new TH1D ("FWHM", "FWHM", NBINS,0,NBINS-1);
  histFwhm_acc_v4 = new TH1D ("FWHM_acc", "FWHM_acc", NBINS,0,NBINS-1);
  histFwhm_rej_v4 = new TH1D ("FWHM_rej", "FWHM_rej", NBINS,0,NBINS-1);
  histMaxampl_v4 = new TH1D ("Amplitude", "Amplitude", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  histMaxampl_acc_v4 = new TH1D ("Amplitude_acc", "Amplitude_acc", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  histMaxampl_rej_v4 = new TH1D ("Amplitude_rej", "Amplitude_rej", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  histArea_v4 = new TH1D ("Area", "Area", NBINS_AREA,0,NBINS_AREA-1);
  histArea_acc_v4 = new TH1D ("Area_acc", "Area_acc", NBINS_AREA,0,NBINS_AREA-1);
  histArea_acc_fit_v4 = new TH1D ("Area fit", "Area fit", NBINS_AREA,0,NBINS_AREA-1);
  histArea_rej_v4 = new TH1D ("Area_rej", "Area_rej", NBINS_AREA,0,NBINS_AREA-1);
  extHistArea_v5 = new TH1D ("Area_v5", "Area_v5", NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  extHistArea_acc_v5 = new TH1D ("ExtArea_acc", "ExtArea_acc", NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  extHistArea_rej_v5 = new TH1D ("ExtArea_rej", "ExtArea_rej", NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  extHistFwhmSlope_v6 = new TH2D ("Width vs Slope", "Width vs Slope",
                                     NBINS_FWHM_SLOPE_X, 0, NBINS_FWHM_SLOPE_X-1,
                                     NBINS_FWHM_SLOPE_Y, -NBINS_FWHM_SLOPE_Y+1, 0);

  leg = new TLegend (0.65,0.75,0.9,0.9);

}


Plots::Plots(int32_t id) {
  std::stringstream ss;
  ss.str(""); ss << "FWHM_" << id;
  histFwhm_v4 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS,0,NBINS-1);
  ss.str(""); ss << "FWHM_acc_" << id;
  histFwhm_acc_v4 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS,0,NBINS-1);
  ss.str(""); ss << "FWHM_rej_" << id;
  histFwhm_rej_v4 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS,0,NBINS-1);
  ss.str(""); ss << "Amplitude_" << id;
  histMaxampl_v4 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  ss.str(""); ss << "Amplitude_acc_" << id;
  histMaxampl_acc_v4 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  ss.str(""); ss << "Amplitude_rej_" << id;
  histMaxampl_rej_v4 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  ss.str(""); ss << "Area_" << id;
  histArea_v4 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS_AREA,0,NBINS_AREA-1);
  ss.str(""); ss << "Area_acc_" << id;
  histArea_acc_v4 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS_AREA,0,NBINS_AREA-1);
  ss.str(""); ss << "Area_acc_fit_" << id;
  histArea_acc_fit_v4 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS_AREA,0,NBINS_AREA-1);
  ss.str(""); ss << "Area_rej_" << id;
  histArea_rej_v4 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS_AREA,0,NBINS_AREA-1);
  ss.str(""); ss << "extArea_" << id;
  extHistArea_v5 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  ss.str(""); ss << "extArea_acc_" << id;
  extHistArea_acc_v5 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  ss.str(""); ss << "extArea_rej_" << id;
  extHistArea_rej_v5 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  ss.str(""); ss << "fwhmSlope_" << id;
  extHistFwhmSlope_v6 = new TH2D (ss.str().c_str(), ss.str().c_str(),
                                     NBINS_FWHM_SLOPE_X, 0, NBINS_FWHM_SLOPE_X-1,
                                     NBINS_FWHM_SLOPE_Y, -NBINS_FWHM_SLOPE_Y+1, 0);
  leg = new TLegend (0.65,0.75,0.9,0.9);

}




Plots::~Plots() {
  if (histFwhm_v4) delete histFwhm_v4;
  if (histFwhm_acc_v4) delete histFwhm_acc_v4;
  if (histFwhm_rej_v4) delete histFwhm_rej_v4;
  if (histMaxampl_v4) delete histMaxampl_v4;
  if (histMaxampl_acc_v4) delete histMaxampl_acc_v4;
  if (histMaxampl_rej_v4) delete histMaxampl_rej_v4;
  if (histArea_v4) delete histArea_v4;
  if (histArea_acc_v4) delete histArea_acc_v4;
  if (histArea_acc_fit_v4) delete histArea_acc_fit_v4;
  if (histArea_rej_v4) delete histArea_rej_v4;
  if (extHistArea_v5) delete extHistArea_v5;
  if (extHistArea_acc_v5) delete extHistArea_acc_v5;
  if (extHistArea_rej_v5) delete extHistArea_rej_v5;
  if (extHistFwhmSlope_v6) delete extHistFwhmSlope_v6;
  if (leg) delete leg;
}
