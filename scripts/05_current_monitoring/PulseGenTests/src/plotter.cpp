#include "plotter.h"

Plotter::Plotter () :
  _wave(0),
  _can(0),
  _app(0),
  _histFwhm_v4(0),
  _histFwhm_acc_v4(0),
  _histFwhm_rej_v4(0),
  _histMaxampl_v4(0),
  _histMaxampl_acc_v4(0),
  _histMaxampl_rej_v4(0),
  _histArea_v4(0),
  _histArea_acc_v4(0),
  _histArea_rej_v4(0),
  _histArea_acc_fit_v4(0),
  _extHistArea_v5(0),
  _extHistArea_acc_v5(0),
  _extHistArea_rej_v5(0),
  _leg(0)
{

  //cout<<" Plotter initialised."<<endl;
  _app = new TApplication ("app",NULL,NULL);
}

Plotter::~Plotter()
{
  if (_histFwhm_v4) delete _histFwhm_v4;
  if (_histFwhm_acc_v4) delete _histFwhm_acc_v4;
  if (_histFwhm_rej_v4) delete _histFwhm_rej_v4;
  if (_histArea_v4) delete _histArea_v4;
  if (_histArea_acc_v4) delete _histArea_acc_v4;
  if (_histArea_acc_fit_v4) delete _histArea_acc_fit_v4;
  if (_histArea_rej_v4) delete _histArea_rej_v4;
  if (_extHistArea_v5) delete _extHistArea_v5;
  if (_extHistArea_acc_v5) delete _extHistArea_acc_v5;
  if (_extHistArea_rej_v5) delete _extHistArea_rej_v5;
  if (_histMaxampl_v4) delete _histMaxampl_v4;
  if (_histMaxampl_acc_v4) delete _histMaxampl_acc_v4;
  if (_histMaxampl_rej_v4) delete _histMaxampl_rej_v4;
  if (_leg) delete _leg;
  if (_can) delete _can;

}


void Plotter::initHist_v4()
{
  if (_histFwhm_v4) delete _histFwhm_v4;
  _histFwhm_v4 = 0;
  if (_histFwhm_acc_v4) delete _histFwhm_acc_v4;
  _histFwhm_acc_v4 = 0;
  if (_histFwhm_rej_v4) delete _histFwhm_rej_v4;
  _histFwhm_rej_v4 = 0;
  if (_histMaxampl_v4) delete _histMaxampl_v4;
  _histMaxampl_v4 = 0;
  if (_histMaxampl_acc_v4) delete _histMaxampl_acc_v4;
  _histMaxampl_acc_v4 = 0;
  if (_histMaxampl_rej_v4) delete _histMaxampl_rej_v4;
  _histMaxampl_rej_v4 = 0;
  if (_histArea_v4) delete _histArea_v4;
  _histArea_v4 = 0;
  if (_histArea_acc_v4) delete _histArea_acc_v4;
  _histArea_acc_v4 = 0;
  if (_histArea_acc_fit_v4) delete _histArea_acc_fit_v4;
  _histArea_acc_fit_v4 = 0;
  if (_histArea_rej_v4) delete _histArea_rej_v4;
  _histArea_rej_v4 = 0;
  if (_leg) delete _leg;
  _leg = 0;
  _histFwhm_v4 = new TH1D ("FWHM", "FWHM", NBINS,0,NBINS-1);
  _histFwhm_acc_v4 = new TH1D ("FWHM_acc", "FWHM_acc", NBINS,0,NBINS-1);
  _histFwhm_rej_v4 = new TH1D ("FWHM_rej", "FWHM_rej", NBINS,0,NBINS-1);
  _histMaxampl_v4 = new TH1D ("Amplitude", "Amplitude", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  _histMaxampl_acc_v4 = new TH1D ("Amplitude_acc", "Amplitude_acc", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  _histMaxampl_rej_v4 = new TH1D ("Amplitude_rej", "Amplitude_rej", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  _histArea_v4 = new TH1D ("Area", "Area", NBINS_AREA,0,NBINS_AREA-1);
  _histArea_acc_v4 = new TH1D ("PulseArea_acc", "PulseArea_acc", NBINS_AREA,0,NBINS_AREA-1);
  _histArea_acc_fit_v4 = new TH1D ("Area fit", "Area fit", NBINS_AREA,0,NBINS_AREA-1);
  _histArea_rej_v4 = new TH1D ("PulseArea_rej", "PulseArea_rej", NBINS_AREA,0,NBINS_AREA-1);
  _leg = new TLegend (0.65,0.75,0.9,0.9);
}


void Plotter::fillHist_v4(int32_t USBdatalen, uint8_t USBdata[262000]) {

  //just in case
  //_histArea_rej_v4=0;

  //print the buffer
  // for (int32_t i = 0; i < USBdatalen; i = i + 32)
  // {
  //   cout<<i<<" bytes"<<endl;
  //   for (int32_t j = 0; j < 32; j++)
  //     cout<<hex<<(uint32_t)USBdata[i+j]<<" ";
  //   cout<<dec<<endl;
  // }
  //hist buffers
  uint32_t histfwhm[256];
  uint32_t histmaxampl[256];
  uint32_t histalphamaxampl[256];
  uint32_t histarea[256];
  uint32_t histalphaarea[256];
  double histfwhm_v4[256];
  double histmaxampl_v4[256];
  double histarea_v4[256];
  double histalphaarea_v4[256];
  int32_t  bin = 0;
  for (int32_t i = 0; i < 256; i++) {
    histfwhm[i]=0;
    histmaxampl[i]=0;
    histalphamaxampl[i]=0;
    histarea[i]=0;
    histalphaarea[i]=0;
    histfwhm_v4[i]=0;
    histmaxampl_v4[i]=0;
    histarea_v4[i]=0;
    histalphaarea_v4[i]=0;
  }

  for (int32_t i = 0; i < USBdatalen-32; i = i + 32) {//read in 256 bits at a time.
    //256bit = 32 * 8bit

    //find header(64 bit 0xAA)
    int32_t headcnt = 0;
    for (int32_t j = 0; j < 8; j++) {
      if (USBdata[i+j] == 0xAA)
        headcnt++;
      else
        headcnt = 0;
    }
    if (headcnt != 8) {
      continue; // jump to the next 64 bit
    }

    //if it's header, fill the histograms with the next values.
    //first read the bin number.
    bin = (int32_t)USBdata[i+11];
    //then get the values.
    //but not for bin 0, because it is corrupted.
    if (bin==0) continue;

    histfwhm[bin] = (uint32_t)(USBdata[i+12+3] << 0 ) |
                    (uint32_t)(USBdata[i+12+2] << 8 ) |
                    (uint32_t)(USBdata[i+12+1] << 16) |
                    (uint32_t)(USBdata[i+12+0] << 24);
    histmaxampl[bin] =  (uint32_t)(USBdata[i+12+7] << 0 ) |
                        (uint32_t)(USBdata[i+12+6] << 8 ) |
                        (uint32_t)(USBdata[i+12+5] << 16) |
                        (uint32_t)(USBdata[i+12+4] << 24);
    histalphamaxampl[bin] =   (uint32_t)(USBdata[i+12+11] << 0 ) |
                              (uint32_t)(USBdata[i+12+10] << 8 ) |
                              (uint32_t)(USBdata[i+12+9 ] << 16) |
                              (uint32_t)(USBdata[i+12+8 ] << 24);
    histarea[bin] = (uint32_t)(USBdata[i+12+15] << 0 ) |
                    (uint32_t)(USBdata[i+12+14] << 8 ) |
                    (uint32_t)(USBdata[i+12+13] << 16) |
                    (uint32_t)(USBdata[i+12+12] << 24);
    histalphaarea[bin] =  (uint32_t)(USBdata[i+12+19] << 0 ) |
                          (uint32_t)(USBdata[i+12+18] << 8 ) |
                          (uint32_t)(USBdata[i+12+17] << 16) |
                          (uint32_t)(USBdata[i+12+16] << 24);


    //
    // cout<<" bin "<<bin
    //     <<" fwhm "<<histfwhm[bin]
    //     <<" mamp "<<histmaxampl[bin]
    //     <<" area "<<histarea[bin]
    //     <<" alar "<<histalphaarea[bin]
    //     <<endl;
  }
  //reset all histograms
  initHist_v4();


  //fill all histograms
  for (int32_t i = 0; i < NBINS; i++) {
    _histFwhm_v4->Fill(i, histfwhm[i]);
    if (i>=_fwhmThres)
      _histFwhm_acc_v4->Fill(i, histfwhm[i]);
    else
      _histFwhm_rej_v4->Fill(i, histfwhm[i]);
  }
  for (int32_t i = 0; i < NBINS_MAXAMPL; i++) {
    _histMaxampl_v4->Fill(i, histmaxampl[i]);
    _histMaxampl_acc_v4->Fill(i, histalphamaxampl[i]);
    _histMaxampl_rej_v4->Fill(i, histmaxampl[i]-histalphamaxampl[i]);
  }
  for (int32_t i = 0; i < NBINS_AREA; i++) {
    _histArea_v4->Fill(i, histarea[i]);
    _histArea_acc_v4->Fill(i, histalphaarea[i]);
    _histArea_acc_fit_v4->Fill(i, histalphaarea[i]);
    _histArea_rej_v4->Fill(i, histarea[i]-histalphaarea[i]);
  }
  cout<<" filled histograms v4"<<endl;

  showHist_v4();
  //getchar();
}



void Plotter::showHist_v4()
{

  //calculate calibration values---------------------------------
  //for 100mV input, AREA_TO_MEV = 1/128*100*2^4*0.2*5.115/535.0 = 0.005975467
  // double ADC_mV = 1.0/128.0*(double)_inputRange;
  // double ADC_pVs = ADC_mV*SAMPLING_RATE*pow(2,(double)_areaHistScale);
  // double Area_MeV = ADC_pVs*ENERGY_SCALING_FACTOR_PVS_TO_MEV;
  double ADC_mV = _inputRange/128; //in mV
  double ADC_pVs = ADC_mV*SAMPLING_RATE*pow(2,(double)_areaHistScale);
  double Area_MeV = ADC_pVs*ENERGY_SCALING_FACTOR_PVS_TO_MEV;
  cout<<" _inputRange = "<<_inputRange;
  cout<<" ADC_mV      = "<<ADC_mV;
  cout<<" ADC_pVs     = "<<ADC_pVs;
  cout<<" Area_MeV    = "<<Area_MeV;
  //------------------------------------------------------------

  //-------------- Gaussian fit of the amplitude --------------
  TF1 *fit1 =  new TF1("gaussian1", "gaus", 0, 256);
  fit1->SetNpx(10000);
  fit1->SetLineWidth(1);
  fit1->SetLineColor(kRed);
  //------------------------------------------------------------

  //draw to canvas
  if (!_can) {
    _can = new TCanvas("Real time Processing","Real time Processing",1280,800);
    _can->Divide(2,2);
  }
  gStyle->SetOptStat(0);
  _can->cd(1);
  _histFwhm_v4->Draw();
  _histFwhm_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);
  _histFwhm_v4->GetXaxis()->SetTitle("FWHM [ns]");
  _histFwhm_v4->GetYaxis()->SetTitle("Entries");
  _histFwhm_rej_v4->SetFillColor(kBlue-8);
  _histFwhm_rej_v4->SetLineColor(kBlue-8);
  _histFwhm_rej_v4->SetFillStyle(3004);
  _histFwhm_rej_v4->Draw("same");
  _histFwhm_rej_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);
  _histFwhm_acc_v4->SetFillColor(kRed+1);
  _histFwhm_acc_v4->SetLineColor(kRed+1);
  _histFwhm_acc_v4->Draw("same");
  _histFwhm_acc_v4->SetFillStyle(3005);
  _histFwhm_acc_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);

  _can->cd(2);
  gPad->SetLogy(true);
  _histMaxampl_v4->Draw();
  _histMaxampl_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);
  _histMaxampl_v4->GetXaxis()->SetTitle("Amplitude [mV]");
  _histMaxampl_v4->GetYaxis()->SetTitle("Entries");
  _histMaxampl_rej_v4->SetFillColor(kBlue-8);
  _histMaxampl_rej_v4->SetLineColor(kBlue-8);
  _histMaxampl_rej_v4->SetFillStyle(3004);
  _histMaxampl_rej_v4->Draw("same");
  _histMaxampl_rej_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);
  _histMaxampl_acc_v4->SetFillColor(kRed+1);
  _histMaxampl_acc_v4->SetLineColor(kRed+1);
  _histMaxampl_acc_v4->Draw("same");
  _histMaxampl_acc_v4->SetFillStyle(3005);
  _histMaxampl_acc_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);

  //-------------- Gaussian fit of the amplitude --------------
  // _histMaxampl_acc_v4->Fit(fit1, "RQ");
  // cout<<" Fit results: "<<endl
  //     <<"  MEAN(Y)  "<<fit1->GetParameter(0)
  //     <<"  MEAN(X)  "<<fit1->GetParameter(1)
  //     <<"  SIGMA(X) "<<fit1->GetParameter(2)
  //     <<endl;
  //------------------------------------------------------------



  _can->cd(3);
  gPad->SetLogy(true);
  //_histArea_v4->Rebin(2);
  _histArea_v4->Draw();
  _histArea_v4->GetXaxis()->SetLimits(0,NBINS_AREA*Area_MeV);
  _histArea_v4->GetXaxis()->SetTitle("Energy [MeV]");
  _histArea_v4->GetYaxis()->SetTitle("Entries");
  //_histArea_rej_v4->Rebin(2);
  _histArea_rej_v4->SetFillColor(kBlue-8);
  _histArea_rej_v4->SetLineColor(kBlue-8);
  _histArea_rej_v4->SetLineWidth(1);
  _histArea_rej_v4->SetFillStyle(3004);
  _histArea_rej_v4->Draw("same");
  _histArea_rej_v4->GetXaxis()->SetLimits(0,NBINS_AREA*Area_MeV);
  //_histArea_acc_v4->Rebin(2);
  _histArea_acc_v4->SetFillColor(kRed+1);
  _histArea_acc_v4->SetLineColor(kRed+1);
  _histArea_acc_v4->SetLineWidth(1);
  _histArea_acc_v4->SetFillStyle(3005);
  _histArea_acc_v4->Draw("same");
  _histArea_acc_v4->GetXaxis()->SetLimits(0,NBINS_AREA*Area_MeV);

    // _leg->SetBorderSize(0);
    // _leg->SetFillStyle(4000);
    // _leg->SetShadowColor(0);
    // _leg->SetLineColor(0);
    // _leg->SetTextSize(0.03);
    // _leg->SetFillColor(kWhite);
    cout.precision(2);
    stringstream ss;
    ss<< "Overall count:  "<<_nPulses;
    ss<< " at "<<(double)((int32_t)(_freq*100))/100.0<<"/s";
    _leg->AddEntry(_histArea_v4,ss.str().c_str(),"F");
    ss.str("");
    ss<< "Alpha count:    "<<_nPulses_acc;
    ss<< " at "<<(double)((int32_t)(_freq_acc*100))/100.0<<"/s";
    _leg->AddEntry(_histArea_acc_fit_v4,ss.str().c_str(),"F");
    ss.str("");
    _leg->Draw("same");

  //-------------- Gaussian fit of the amplitude --------------
  // _histArea_acc_v4->Fit(fit1, "RQ");
  // cout<<" Fit results: "<<endl
  //     <<"  MEAN(Y)  "<<fit1->GetParameter(0)
  //     <<"  MEAN(X)  "<<fit1->GetParameter(1)
  //     <<"  SIGMA(X) "<<fit1->GetParameter(2)
  //     <<endl;
  //------------------------------------------------------------

  _can->cd(4);
  _histArea_acc_fit_v4->SetFillColor(kRed+1);
  _histArea_acc_fit_v4->SetLineColor(kRed+1);
  _histArea_acc_fit_v4->SetLineWidth(1);
  _histArea_acc_fit_v4->SetFillStyle(3005);
  _histArea_acc_fit_v4->Draw();
  _histArea_acc_fit_v4->GetXaxis()->SetLimits(0,NBINS_AREA*Area_MeV);
  _histArea_acc_fit_v4->GetXaxis()->SetTitle("Energy [MeV]");
  _histArea_acc_fit_v4->GetYaxis()->SetTitle("Entries");


  _can->Modified();
  _can->Update();
  //_can->WaitPrimitive();


  //plotsCalibration(ADC_mV, ADC_pVs);
  plotsNeutronMeas(ADC_mV, ADC_pVs, Area_MeV);


  // TH1D *fwhm = new TH1D ("FWHM", "FWHM", NBINS,0,NBINS-1);
  // fwhm->GetXaxis()->SetLimits(0,NBINS_AREA*Area_MeV);
  // fwhm->GetXaxis()->SetTitle("Energy [MeV]");
  // fwhm->GetYaxis()->SetTitle("Entries");
  // fwhm->

  // _histFwhm_acc_v4 = new TH1D ("FWHM_acc", "FWHM_acc", NBINS,0,NBINS-1);
  // _histFwhm_rej_v4 = new TH1D ("FWHM_rej", "FWHM_rej", NBINS,0,NBINS-1);
  // _histMaxampl_v4 = new TH1D ("Amplitude", "Amplitude", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  // _histMaxampl_acc_v4 = new TH1D ("Amplitude_acc", "Amplitude_acc", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  // _histMaxampl_rej_v4 = new TH1D ("Amplitude_rej", "Amplitude_rej", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  // _histArea_v4 = new TH1D ("Area", "Area", NBINS_AREA,0,NBINS_AREA-1);
  // _histArea_acc_v4 = new TH1D ("PulseArea_acc", "PulseArea_acc", NBINS_AREA,0,NBINS_AREA-1);
  // _histArea_acc_fit_v4 = new TH1D ("Area fit", "Area fit", NBINS_AREA,0,NBINS_AREA-1);
  // _histArea_rej_v4 = new TH1D ("PulseArea_rej", "PulseArea_rej", NBINS_AREA,0,NBINS_AREA-1);
  // _leg = new TLegend (0.65,0.75,0.9,0.9);




  //---------------------------------------------------------------------




}


void Plotter::plotsNeutronMeas(double ADC_mV, double ADC_pVs, double Area_MeV) {
    //-------------------  Additional plots for the report ----------------
    gStyle->SetOptStat(0);
    gStyle->SetStatFont(132);
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    gStyle->SetStatW(0.2);
    gStyle->SetStatH(0.2);
    cout.precision(3);
    //--------------------------------
    TCanvas *c1 = new TCanvas();
    _histFwhm_v4->Draw();
    _histFwhm_v4->SetFillColor(kRed+1);
    _histFwhm_v4->SetLineColor(kRed+1);
    _histFwhm_v4->Draw();
    _histFwhm_v4->SetFillStyle(3005);
    _histFwhm_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);
    _histFwhm_v4->GetXaxis()->SetRangeUser(0,11);
    _histFwhm_v4->SetTitle("");
    _histFwhm_v4->GetXaxis()->SetTitle("FWHM [ns]");
    _histFwhm_v4->GetYaxis()->SetTitle("Entries");
    _histFwhm_v4->GetYaxis()->SetLabelFont(132);
    _histFwhm_v4->GetXaxis()->SetLabelFont(132);
    _histFwhm_v4->GetYaxis()->SetTitleFont(132);
    _histFwhm_v4->GetXaxis()->SetTitleFont(132);
    _histFwhm_v4->GetYaxis()->SetTitleSize(0.05);
    _histFwhm_v4->GetXaxis()->SetTitleSize(0.05);
    _histFwhm_v4->GetYaxis()->SetLabelSize(0.05);
    _histFwhm_v4->GetXaxis()->SetLabelSize(0.05);
    _histFwhm_v4->GetXaxis()->SetNdivisions(9);
    _histFwhm_v4->Draw();
    c1->Modified();
    c1->Update();
    c1->WaitPrimitive();
    c1->SaveAs("plots/fwhm2.pdf");
    _histFwhm_rej_v4->Draw();
    _histFwhm_rej_v4->SetFillColor(kBlue-8);
    _histFwhm_rej_v4->SetLineColor(kBlue-8);
    _histFwhm_rej_v4->SetFillStyle(3004);
    _histFwhm_rej_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);
    _histFwhm_rej_v4->GetXaxis()->SetRangeUser(0,11);
    _histFwhm_rej_v4->SetTitle("");
    _histFwhm_rej_v4->GetXaxis()->SetTitle("FWHM [ns]");
    _histFwhm_rej_v4->GetYaxis()->SetTitle("Entries");
    _histFwhm_rej_v4->GetYaxis()->SetLabelFont(132);
    _histFwhm_rej_v4->GetXaxis()->SetLabelFont(132);
    _histFwhm_rej_v4->GetYaxis()->SetTitleFont(132);
    _histFwhm_rej_v4->GetXaxis()->SetTitleFont(132);
    _histFwhm_rej_v4->GetYaxis()->SetTitleSize(0.05);
    _histFwhm_rej_v4->GetXaxis()->SetTitleSize(0.05);
    _histFwhm_rej_v4->GetYaxis()->SetLabelSize(0.05);
    _histFwhm_rej_v4->GetXaxis()->SetLabelSize(0.05);
    _histFwhm_rej_v4->GetXaxis()->SetNdivisions(9);
    _histFwhm_acc_v4->Draw("same");
    _histFwhm_acc_v4->SetFillColor(kRed+1);
    _histFwhm_acc_v4->SetLineColor(kRed+1);
    _histFwhm_acc_v4->SetFillStyle(3005);
    _histFwhm_acc_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);
    _histFwhm_acc_v4->GetXaxis()->SetRangeUser(0,11);
    _histFwhm_acc_v4->SetTitle("");
    _histFwhm_acc_v4->GetYaxis()->SetLabelFont(132);
    _histFwhm_acc_v4->GetXaxis()->SetLabelFont(132);
    _histFwhm_acc_v4->GetYaxis()->SetTitleFont(132);
    _histFwhm_acc_v4->GetXaxis()->SetTitleFont(132);
    _histFwhm_acc_v4->GetYaxis()->SetTitleSize(0.05);
    _histFwhm_acc_v4->GetXaxis()->SetTitleSize(0.05);
    _histFwhm_acc_v4->GetYaxis()->SetLabelSize(0.05);
    _histFwhm_acc_v4->GetXaxis()->SetLabelSize(0.05);
    _histFwhm_acc_v4->GetXaxis()->SetNdivisions(9);
    c1->Modified();
    c1->Update();
    c1->WaitPrimitive();
    c1->SaveAs("plots/fwhm2class.pdf");


    //--------------------------------
    TCanvas *c2 = new TCanvas();
    _histMaxampl_v4->Draw();
    _histMaxampl_v4->SetFillColor(kRed+1);
    _histMaxampl_v4->SetLineColor(kRed+1);
    _histMaxampl_v4->SetFillStyle(3005);
    _histMaxampl_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);
    _histMaxampl_v4->GetXaxis()->SetRangeUser(0,60);
    _histMaxampl_v4->GetXaxis()->SetTitle("Amplitude [mV]");
    _histMaxampl_v4->GetYaxis()->SetTitle("Entries");
    _histMaxampl_v4->SetTitle("");
    _histMaxampl_v4->GetYaxis()->SetLabelFont(132);
    _histMaxampl_v4->GetXaxis()->SetLabelFont(132);
    _histMaxampl_v4->GetYaxis()->SetTitleFont(132);
    _histMaxampl_v4->GetXaxis()->SetTitleFont(132);
    _histMaxampl_v4->GetYaxis()->SetTitleSize(0.05);
    _histMaxampl_v4->GetXaxis()->SetTitleSize(0.05);
    _histMaxampl_v4->GetYaxis()->SetLabelSize(0.05);
    _histMaxampl_v4->GetXaxis()->SetLabelSize(0.05);
    _histMaxampl_v4->GetXaxis()->SetNdivisions(9);
    c2->Modified();
    c2->Update();
    c2->WaitPrimitive();
    c2->SaveAs("plots/maxampl2.pdf");
    _histMaxampl_rej_v4->Draw();
    _histMaxampl_rej_v4->SetFillColor(kBlue-8);
    _histMaxampl_rej_v4->SetLineColor(kBlue-8);
    _histMaxampl_rej_v4->SetFillStyle(3004);
    _histMaxampl_rej_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);
    _histMaxampl_rej_v4->GetXaxis()->SetRangeUser(0,60);
    _histMaxampl_rej_v4->GetXaxis()->SetTitle("Amplitude [mV]");
    _histMaxampl_rej_v4->GetYaxis()->SetTitle("Entries");
    _histMaxampl_rej_v4->SetTitle("");
    _histMaxampl_rej_v4->GetYaxis()->SetLabelFont(132);
    _histMaxampl_rej_v4->GetXaxis()->SetLabelFont(132);
    _histMaxampl_rej_v4->GetYaxis()->SetTitleFont(132);
    _histMaxampl_rej_v4->GetXaxis()->SetTitleFont(132);
    _histMaxampl_rej_v4->GetYaxis()->SetTitleSize(0.05);
    _histMaxampl_rej_v4->GetXaxis()->SetTitleSize(0.05);
    _histMaxampl_rej_v4->GetYaxis()->SetLabelSize(0.05);
    _histMaxampl_rej_v4->GetXaxis()->SetLabelSize(0.05);
    _histMaxampl_rej_v4->GetXaxis()->SetNdivisions(9);
    _histMaxampl_acc_v4->Draw("same");
    _histMaxampl_acc_v4->SetFillColor(kRed+1);
    _histMaxampl_acc_v4->SetLineColor(kRed+1);
    _histMaxampl_acc_v4->SetFillStyle(3005);
    _histMaxampl_acc_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);
    _histMaxampl_acc_v4->GetXaxis()->SetRangeUser(0,60);
    _histMaxampl_acc_v4->GetXaxis()->SetTitle("Amplitude [mV]");
    _histMaxampl_acc_v4->GetYaxis()->SetTitle("Entries");
    _histMaxampl_acc_v4->SetTitle("");
    _histMaxampl_acc_v4->GetYaxis()->SetLabelFont(132);
    _histMaxampl_acc_v4->GetXaxis()->SetLabelFont(132);
    _histMaxampl_acc_v4->GetYaxis()->SetTitleFont(132);
    _histMaxampl_acc_v4->GetXaxis()->SetTitleFont(132);
    _histMaxampl_acc_v4->GetYaxis()->SetTitleSize(0.05);
    _histMaxampl_acc_v4->GetXaxis()->SetTitleSize(0.05);
    _histMaxampl_acc_v4->GetYaxis()->SetLabelSize(0.05);
    _histMaxampl_acc_v4->GetXaxis()->SetLabelSize(0.05);
    _histMaxampl_acc_v4->GetXaxis()->SetNdivisions(9);
    c2->Modified();
    c2->Update();
    c2->WaitPrimitive();
    c2->SaveAs("plots/maxampl2class.pdf");
    TPad *pad2 = new TPad("pad2","pad2",0.33,0.33,0.963,0.963);
    pad2->Draw();
    pad2->SetFillStyle(4000);
    pad2->cd();
    pad2->SetLogy();
    _histMaxampl_rej_v4->Draw();
    _histMaxampl_acc_v4->Draw("same");
    pad2->Modified();
    pad2->Update();
    c2->Modified();
    c2->Update();
    c2->WaitPrimitive();
    c2->SaveAs("plots/maxampl2classlog.pdf");



    //--------------------------------
    TCanvas *c3 = new TCanvas();
    _histArea_v4->Draw();
    _histArea_v4->SetFillColor(kRed+1);
    _histArea_v4->SetLineColor(kRed+1);
    _histArea_v4->SetFillStyle(3005);
    _histArea_v4->GetXaxis()->SetLimits(0,NBINS_AREA*Area_MeV);
    _histArea_v4->GetXaxis()->SetRangeUser(0,3.5);
    _histArea_v4->GetXaxis()->SetTitle("Energy [MeV]");
    _histArea_v4->GetYaxis()->SetTitle("Entries");
    _histArea_v4->SetTitle("");
    //_histArea_v4->GetYaxis()->SetRangeUser(0,1050);
    _histArea_v4->GetYaxis()->SetLabelFont(132);
    _histArea_v4->GetXaxis()->SetLabelFont(132);
    _histArea_v4->GetYaxis()->SetTitleFont(132);
    _histArea_v4->GetXaxis()->SetTitleFont(132);
    _histArea_v4->GetYaxis()->SetTitleSize(0.05);
    _histArea_v4->GetXaxis()->SetTitleSize(0.05);
    _histArea_v4->GetYaxis()->SetLabelSize(0.05);
    _histArea_v4->GetXaxis()->SetLabelSize(0.05);
    _histArea_v4->GetXaxis()->SetNdivisions(9);
    c3->Modified();
    c3->Update();
    c3->WaitPrimitive();
    c3->SaveAs("plots/area2.pdf");
    _histArea_rej_v4->Draw();
    _histArea_rej_v4->SetFillColor(kBlue-8);
    _histArea_rej_v4->SetLineColor(kBlue-8);
    _histArea_rej_v4->SetFillStyle(3005);
    _histArea_rej_v4->GetXaxis()->SetLimits(0,NBINS_AREA*Area_MeV);
    _histArea_rej_v4->GetXaxis()->SetRangeUser(0,3.5);
    _histArea_rej_v4->GetXaxis()->SetTitle("Energy [MeV]");
    _histArea_rej_v4->GetYaxis()->SetTitle("Entries");
    _histArea_rej_v4->SetTitle("");
    //_histArea_rej_v4->GetYaxis()->SetRangeUser(0,1050);
    _histArea_rej_v4->GetYaxis()->SetLabelFont(132);
    _histArea_rej_v4->GetXaxis()->SetLabelFont(132);
    _histArea_rej_v4->GetYaxis()->SetTitleFont(132);
    _histArea_rej_v4->GetXaxis()->SetTitleFont(132);
    _histArea_rej_v4->GetYaxis()->SetTitleSize(0.05);
    _histArea_rej_v4->GetXaxis()->SetTitleSize(0.05);
    _histArea_rej_v4->GetYaxis()->SetLabelSize(0.05);
    _histArea_rej_v4->GetXaxis()->SetLabelSize(0.05);
    _histArea_rej_v4->GetXaxis()->SetNdivisions(9);
    _histArea_acc_v4->Draw("same");
    _histArea_acc_v4->SetFillColor(kRed+1);
    _histArea_acc_v4->SetLineColor(kRed+1);
    _histArea_acc_v4->SetFillStyle(3005);
    _histArea_acc_v4->GetXaxis()->SetLimits(0,NBINS_AREA*Area_MeV);
    _histArea_acc_v4->GetXaxis()->SetRangeUser(0,3.5);
    _histArea_acc_v4->GetXaxis()->SetTitle("Energy [MeV]");
    _histArea_acc_v4->GetYaxis()->SetTitle("Entries");
    _histArea_acc_v4->SetTitle("");
    //_histArea_acc_v4->GetYaxis()->SetRangeUser(0,1050);
    _histArea_acc_v4->GetYaxis()->SetLabelFont(132);
    _histArea_acc_v4->GetXaxis()->SetLabelFont(132);
    _histArea_acc_v4->GetYaxis()->SetTitleFont(132);
    _histArea_acc_v4->GetXaxis()->SetTitleFont(132);
    _histArea_acc_v4->GetYaxis()->SetTitleSize(0.05);
    _histArea_acc_v4->GetXaxis()->SetTitleSize(0.05);
    _histArea_acc_v4->GetYaxis()->SetLabelSize(0.05);
    _histArea_acc_v4->GetXaxis()->SetLabelSize(0.05);
    _histArea_acc_v4->GetXaxis()->SetNdivisions(9);
    c3->Modified();
    c3->Update();
    c3->WaitPrimitive();
    c3->SaveAs("plots/area2class.pdf");
    TPad *pad3 = new TPad("pad1","pad1",0.33,0.33,0.963,0.963);
    pad3->Draw();
    pad3->SetFillStyle(4000);
    pad3->cd();
    pad3->SetLogy();
    _histArea_rej_v4->Draw();
    _histArea_acc_v4->Draw("same");
    TF1 *fitAlpha = new TF1("alpha","gaus",0.5,2);
    TF1 *fitTriton = new TF1("triton","gaus",2,3.5);
    _histArea_acc_v4->Fit(fitAlpha, "R0");
    cout<<" Alpha peak: "
        <<fitAlpha->GetParameter(1)<<" +- "
        <<fitAlpha->GetParameter(2)<<" MeV "<<endl;
    _histArea_acc_v4->Fit(fitTriton, "R0");
    cout<<" Triton peak: "
        <<fitTriton->GetParameter(1)<<" +- "
        <<fitTriton->GetParameter(2)<<" MeV "<<endl;
    //    gStyle->SetOptStat();
    //TPaveStats *ps = (TPaveStats*)c3->GetPrimitive("stats");
    //ps->SetName("mystats");
    //TList *list = ps->GetListOfLines();
    //TLatex *myt = new TLatex(0,0,"Test = 10");
    //myt ->SetTextFont(42);
    //myt ->SetTextSize(0.04);
    //myt ->SetTextColor(kRed);
    //list->Add(myt);


    pad3->Modified();
    pad3->Update();
    c3->Modified();
    c3->Update();
    c3->WaitPrimitive();
    c3->SaveAs("plots/area2classlog.pdf");

}

void Plotter::plotsCalibration(double ADC_mV, double ADC_pVs) {
    //-------------------  Additional plots for the report ----------------
    gStyle->SetOptStat(1101);
    gStyle->SetStatFont(132);
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    gStyle->SetStatW(0.2);
    gStyle->SetStatH(0.2);
    //--------------------------------
    TCanvas *c1 = new TCanvas();
    _histFwhm_v4->Draw();
    _histFwhm_v4->SetFillColor(kRed+1);
    _histFwhm_v4->SetLineColor(kRed+1);
    _histFwhm_v4->SetFillStyle(3005);
    _histFwhm_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);
    _histFwhm_v4->GetXaxis()->SetRangeUser(6.5,11);
    _histFwhm_v4->SetTitle("");
    _histFwhm_v4->GetXaxis()->SetTitle("FWHM [ns]");
    _histFwhm_v4->GetYaxis()->SetTitle("Entries");
    _histFwhm_v4->GetYaxis()->SetLabelFont(132);
    _histFwhm_v4->GetXaxis()->SetLabelFont(132);
    _histFwhm_v4->GetYaxis()->SetTitleFont(132);
    _histFwhm_v4->GetXaxis()->SetTitleFont(132);
    _histFwhm_v4->GetYaxis()->SetTitleSize(0.05);
    _histFwhm_v4->GetXaxis()->SetTitleSize(0.05);
    _histFwhm_v4->GetYaxis()->SetLabelSize(0.05);
    _histFwhm_v4->GetXaxis()->SetLabelSize(0.05);
    _histFwhm_v4->GetXaxis()->SetNdivisions(9);


    TF1 *gaus1 = new TF1("t1","gaus",0,20);
    gaus1->SetParameter(1, _histFwhm_v4->GetMaximumBin() );
    _histFwhm_v4->Fit(gaus1,"Q0");
    //TLegend *leg1 = new TLegend (0.65,0.75,0.9,0.9);
    stringstream ss1;
    ss1             <<"FWHM = "
                    << float((int32_t)(gaus1->GetParameter(1)*10))/10
                    << " #pm "
                    << float((int32_t)(gaus1->GetParameter(2)*10))/10 << "ns";
    cout<<ss1.str()<<endl;

    //leg1->AddEntry((TObject*)0, ss1.str().c_str() );
    //leg1->Draw("same");
    // TPad *pad = new TPad("pad1","pad1",0.7,0.7,0.9,0.9);
    // // pad->cd();
    // pad->SetFillColor(0);
    // pad->SetBorderMode(0);
    // pad->SetBorderSize(1);
    // pad->SetBorderColor(kBlack);
    // // pad->SetFrameBorderMode(1);
    // pad->Draw();
    // TLatex *Tl = new TLatex();
    // Tl->SetTextSize(0.06);
    // Tl->SetTextAlign(12);
    // Tl->SetTextFont(132);
    // //Tl->DrawLatex(6.2, 6200, ss1.str().c_str());//Latex( 0.5, 0.5, " asdfasf ");
    // Tl->DrawLatex(0.2, 0.2, ss1.str().c_str());
    // pad->Modified();
    // pad->Update();
    c1->Modified();
    c1->Update();
    c1->WaitPrimitive();
    c1->SaveAs("plots/fwhm1.pdf");

    //--------------------------------
    TCanvas *c2 = new TCanvas();
    _histMaxampl_v4->Draw();
    _histMaxampl_v4->SetFillColor(kRed+1);
    _histMaxampl_v4->SetLineColor(kRed+1);
    _histMaxampl_v4->SetFillStyle(3005);
    _histMaxampl_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);
    _histMaxampl_v4->GetXaxis()->SetRangeUser(55,85);
    _histMaxampl_v4->GetXaxis()->SetTitle("Amplitude [mV]");
    _histMaxampl_v4->GetYaxis()->SetTitle("Entries");
    _histMaxampl_v4->SetTitle("");
    _histMaxampl_v4->GetYaxis()->SetLabelFont(132);
    _histMaxampl_v4->GetXaxis()->SetLabelFont(132);
    _histMaxampl_v4->GetYaxis()->SetTitleFont(132);
    _histMaxampl_v4->GetXaxis()->SetTitleFont(132);
    _histMaxampl_v4->GetYaxis()->SetTitleSize(0.05);
    _histMaxampl_v4->GetXaxis()->SetTitleSize(0.05);
    _histMaxampl_v4->GetYaxis()->SetLabelSize(0.05);
    _histMaxampl_v4->GetXaxis()->SetLabelSize(0.05);
    _histMaxampl_v4->GetXaxis()->SetNdivisions(9);


    //_histFwhm_v4->GetXaxis()->SetRangeUser(6,10);
    TF1 *gaus2 = new TF1("t2","gaus",60,85);
    gaus2->SetParameter(1, _histMaxampl_v4->GetMaximumBin() );
    _histMaxampl_v4->Fit(gaus2,"Q0");
    //TLegend *leg1 = new TLegend (0.65,0.75,0.9,0.9);
    stringstream ss2;
    ss2             <<"Ampl = "
                    << float((int32_t)(gaus2->GetParameter(1)*1))/1
                    << " #pm "
                    << float((int32_t)(gaus2->GetParameter(2)*1))/1 << "mV";
    cout<<ss2.str()<<endl;
    //leg1->AddEntry((TObject*)0, ss1.str().c_str() );
    //leg1->Draw("same");
    TLatex *Tl2 = new TLatex( );
    Tl2->SetTextSize(0.06);
    Tl2->SetTextAlign(12);
    Tl2->SetTextFont(132);
    //Tl2->DrawLatex(52, 2500, ss2.str().c_str());//Latex( 0.5, 0.5, " asdfasf ");
    c2->Modified();
    c2->Update();
    c2->WaitPrimitive();
    c2->SaveAs("plots/maxampl1.pdf");

    //--------------------------------
    TCanvas *c3 = new TCanvas();
    _histArea_v4->Draw();
    _histArea_v4->SetFillColor(kRed+1);
    _histArea_v4->SetLineColor(kRed+1);
    _histArea_v4->SetFillStyle(3005);
    _histArea_v4->GetXaxis()->SetLimits(0,NBINS_AREA*ADC_pVs);
    _histArea_v4->GetXaxis()->SetRangeUser(420,640);
    _histArea_v4->GetXaxis()->SetTitle("Area [pVs]");
    _histArea_v4->GetYaxis()->SetTitle("Entries");
    _histArea_v4->SetTitle("");
    _histArea_v4->GetYaxis()->SetRangeUser(0,1050);
    _histArea_v4->GetYaxis()->SetLabelFont(132);
    _histArea_v4->GetXaxis()->SetLabelFont(132);
    _histArea_v4->GetYaxis()->SetTitleFont(132);
    _histArea_v4->GetXaxis()->SetTitleFont(132);
    _histArea_v4->GetYaxis()->SetTitleSize(0.05);
    _histArea_v4->GetXaxis()->SetTitleSize(0.05);
    _histArea_v4->GetYaxis()->SetLabelSize(0.05);
    _histArea_v4->GetXaxis()->SetLabelSize(0.05);
    _histArea_v4->GetXaxis()->SetNdivisions(9);

    //_histFwhm_v4->GetXaxis()->SetRangeUser(6,10);
    TF1 *gaus3 = new TF1("t3","gaus",530,700);
    gaus3->SetParameter(1, _histArea_v4->GetMaximumBin() );
    _histArea_v4->Fit(gaus3,"RQ0");
    _histArea_v4->Draw();
    //TLegend *leg1 = new TLegend (0.65,0.75,0.9,0.9);
    stringstream ss3;
    ss3             <<"Area = "
                    << float((int32_t)(gaus3->GetParameter(1)*1))/1
                    << " #pm "
                    << float((int32_t)(gaus3->GetParameter(2)*1))/1 << " pVs";
    cout<<ss3.str()<<endl;
    //leg1->AddEntry((TObject*)0, ss1.str().c_str() );
    //leg1->Draw("same");
    TLatex *Tl3 = new TLatex( );
    Tl3->SetTextSize(0.06);
    Tl3->SetTextAlign(12);
    Tl3->SetTextFont(132);
    //Tl3->DrawLatex(410, 900, ss3.str().c_str());
    TGaxis *axis3 = new TGaxis (420,1050,640,1050,420/gaus3->GetParameter(1)*5.12,640/gaus3->GetParameter(1)*5.12,510,"-");
    axis3->SetName("Energy");
    axis3->SetTitle("Energy [MeV]");
    axis3->SetLineColor(kRed);
    axis3->SetLabelSize(0.03);
    axis3->SetTextFont(132);
    axis3->SetTextColor(kRed+1);
    axis3->SetTitleSize(0.045);
    axis3->SetLabelSize(0.05);
    axis3->SetLabelFont(132);
    axis3->SetLabelColor(kRed+1);
    //axis3->SetLabelOffset(0.025);
    axis3->Draw("same");
    gStyle->SetOptStat(1101);
    c3->Modified();
    c3->Update();
    c3->WaitPrimitive();
    c3->SaveAs("plots/area1.pdf");
}








//-------------------------------------------------------------------
//                        HISTOGRAMMING V5
//-------------------------------------------------------------------
void Plotter::initHist_v5()
{
  if (_histFwhm_v4) delete _histFwhm_v4;
  _histFwhm_v4 = 0;
  if (_histFwhm_acc_v4) delete _histFwhm_acc_v4;
  _histFwhm_acc_v4 = 0;
  if (_histFwhm_rej_v4) delete _histFwhm_rej_v4;
  _histFwhm_rej_v4 = 0;
  if (_histMaxampl_v4) delete _histMaxampl_v4;
  _histMaxampl_v4 = 0;
  if (_histMaxampl_acc_v4) delete _histMaxampl_acc_v4;
  _histMaxampl_acc_v4 = 0;
  if (_histMaxampl_rej_v4) delete _histMaxampl_rej_v4;
  _histMaxampl_rej_v4 = 0;
  if (_histArea_v4) delete _histArea_v4;
  _histArea_v4 = 0;
  if (_histArea_acc_v4) delete _histArea_acc_v4;
  _histArea_acc_v4 = 0;
  if (_histArea_acc_fit_v4) delete _histArea_acc_fit_v4;
  _histArea_acc_fit_v4 = 0;
  if (_histArea_rej_v4) delete _histArea_rej_v4;
  _histArea_rej_v4 = 0;
  if (_extHistArea_v5) delete _extHistArea_v5;
  _extHistArea_v5 = 0;
  if (_extHistArea_acc_v5) delete _extHistArea_acc_v5;
  _extHistArea_acc_v5 = 0;
  if (_extHistArea_rej_v5) delete _extHistArea_rej_v5;
  _extHistArea_rej_v5 = 0;
  if (_leg) delete _leg;
  _leg = 0;
  _histFwhm_v4 = new TH1D ("FWHM", "FWHM", NBINS,0,NBINS-1);
  _histFwhm_acc_v4 = new TH1D ("FWHM_acc", "FWHM_acc", NBINS,0,NBINS-1);
  _histFwhm_rej_v4 = new TH1D ("FWHM_rej", "FWHM_rej", NBINS,0,NBINS-1);
  _histMaxampl_v4 = new TH1D ("Amplitude", "Amplitude", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  _histMaxampl_acc_v4 = new TH1D ("Amplitude_acc", "Amplitude_acc", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  _histMaxampl_rej_v4 = new TH1D ("Amplitude_rej", "Amplitude_rej", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  _histArea_v4 = new TH1D ("Area", "Area", NBINS_AREA,0,NBINS_AREA-1);
  _histArea_acc_v4 = new TH1D ("Area_acc", "Area_acc", NBINS_AREA,0,NBINS_AREA-1);
  _histArea_acc_fit_v4 = new TH1D ("Area fit", "Area fit", NBINS_AREA,0,NBINS_AREA-1);
  _histArea_rej_v4 = new TH1D ("Area_rej", "Area_rej", NBINS_AREA,0,NBINS_AREA-1);
  _extHistArea_v5 = new TH1D ("Area", "Area", NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  _extHistArea_acc_v5 = new TH1D ("ExtArea_acc", "ExtArea_acc", NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  _extHistArea_rej_v5 = new TH1D ("ExtArea_rej", "ExtArea_rej", NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  _leg = new TLegend (0.65,0.75,0.9,0.9);
}


void Plotter::fillHist_v5(int32_t USBdatalen, uint8_t USBdata[262000]) {

  //just in case
  //_histArea_rej_v4=0;

  //print the buffer
  // for (int32_t i = 0; i < USBdatalen; i = i + 32)
  // {
  //   cout<<i<<" bytes"<<endl;
  //   for (int32_t j = 0; j < 32; j++)
  //     cout<<hex<<(uint32_t)USBdata[i+j]<<" ";
  //   cout<<dec<<endl;
  //
  //   if (!(i%100)) getchar();
  // }
  //hist buffers
  uint32_t histfwhm[256];
  uint32_t histmaxampl[256];
  uint32_t histalphamaxampl[256];
  uint32_t histarea[256];
  uint32_t histalphaarea[256];
  double histfwhm_v4[256];
  double histmaxampl_v4[256];
  double histarea_v4[256];
  double histalphaarea_v4[256];
  double exthistarea_v5[NBINS_EXTAREA];
  double exthistalphaarea_v5[NBINS_EXTAREA];
  int32_t  bin = 0;
  for (int32_t i = 0; i < 256; i++) {
    histfwhm[i]=0;
    histmaxampl[i]=0;
    histalphamaxampl[i]=0;
    histarea[i]=0;
    histalphaarea[i]=0;
    histfwhm_v4[i]=0;
    histmaxampl_v4[i]=0;
    histarea_v4[i]=0;
    histalphaarea_v4[i]=0;
  }
  for (int32_t i = 0; i < NBINS_EXTAREA; i++) {
    exthistarea_v5[i]=0;
    exthistalphaarea_v5[i]=0;
  }

  for (int32_t i = 0; i < USBdatalen-32; i = i + 32) {//read in 256 bits at a time.
    //256bit = 32 * 8bit

    //find header(64 bit 0xAA)
    int32_t headcnt = 0;
    for (int32_t j = 0; j < 2; j++) {
      if (USBdata[i+j] == 0xAA)
        headcnt++;
      else
        headcnt = 0;
    }
    if (headcnt != 2) {
      continue; // jump to the next 64 bit
    }

    //if it's header, fill the histograms with the next values.
    //first read the bin number. 16bit.
    bin = (uint32_t)(USBdata[i+2] << 8) |
          (uint32_t)(USBdata[i+3]);
    //bin value is actually one less - we get too high of a value from Fpga
    bin -= 1;

    //then get the values.
    //but not for bin 0, because it is corrupted.
    //if (bin==0) continue;

    exthistarea_v5[bin] = (uint32_t)(USBdata[i+4+27] << 0 ) |
                          (uint32_t)(USBdata[i+4+26] << 8 ) |
                          (uint32_t)(USBdata[i+4+25] << 16) |
                          (uint32_t)(USBdata[i+4+24] << 24);
    exthistalphaarea_v5[bin] =  (uint32_t)(USBdata[i+4+23] << 0 ) |
                                (uint32_t)(USBdata[i+4+22] << 8 ) |
                                (uint32_t)(USBdata[i+4+21] << 16) |
                                (uint32_t)(USBdata[i+4+20] << 24);

      // cout<<" bin "<<bin
      //   <<" extarea "<<exthistarea_v5[bin]
      //   <<" extalar "<<exthistalphaarea_v5[bin];
      //


    if (bin>255) continue;
    histfwhm[bin] = (uint32_t)(USBdata[i+4+3] << 0 ) |
                    (uint32_t)(USBdata[i+4+2] << 8 ) |
                    (uint32_t)(USBdata[i+4+1] << 16) |
                    (uint32_t)(USBdata[i+4+0] << 24);
    histmaxampl[bin] =  (uint32_t)(USBdata[i+4+7] << 0 ) |
                        (uint32_t)(USBdata[i+4+6] << 8 ) |
                        (uint32_t)(USBdata[i+4+5] << 16) |
                        (uint32_t)(USBdata[i+4+4] << 24);
    histalphamaxampl[bin] =   (uint32_t)(USBdata[i+4+11] << 0 ) |
                              (uint32_t)(USBdata[i+4+10] << 8 ) |
                              (uint32_t)(USBdata[i+4+9 ] << 16) |
                              (uint32_t)(USBdata[i+4+8 ] << 24);
    histarea[bin] = (uint32_t)(USBdata[i+4+15] << 0 ) |
                    (uint32_t)(USBdata[i+4+14] << 8 ) |
                    (uint32_t)(USBdata[i+4+13] << 16) |
                    (uint32_t)(USBdata[i+4+12] << 24);
    histalphaarea[bin] =  (uint32_t)(USBdata[i+4+19] << 0 ) |
                          (uint32_t)(USBdata[i+4+18] << 8 ) |
                          (uint32_t)(USBdata[i+4+17] << 16) |
                          (uint32_t)(USBdata[i+4+16] << 24);


    //
    // cout<<" fwhm "<<histfwhm[bin]
    //     <<" mamp "<<histmaxampl[bin]
    //     <<" area "<<histarea[bin]
    //     <<" alar "<<histalphaarea[bin]
    //    <<endl;
  }
  //reset all histograms
  // cout<<" Resetting histos"<<endl;
  initHist_v5();


  //fill all histograms
  for (int32_t i = 0; i < NBINS; i++) {
    _histFwhm_v4->Fill(i, histfwhm[i]);
    if (i>=_fwhmThres)
      _histFwhm_acc_v4->Fill(i, histfwhm[i]);
    else
      _histFwhm_rej_v4->Fill(i, histfwhm[i]);
  }
  // cout<<".";
  for (int32_t i = 0; i < NBINS_MAXAMPL; i++) {
    _histMaxampl_v4->Fill(i, histmaxampl[i]);
    _histMaxampl_acc_v4->Fill(i, histalphamaxampl[i]);
    _histMaxampl_rej_v4->Fill(i, histmaxampl[i]-histalphamaxampl[i]);
  }
  // cout<<".";
  for (int32_t i = 0; i < NBINS_AREA; i++) {
    _histArea_v4->Fill(i, histarea[i]);
    _histArea_acc_v4->Fill(i, histalphaarea[i]);
    _histArea_acc_fit_v4->Fill(i, histalphaarea[i]);
    _histArea_rej_v4->Fill(i, histarea[i]-histalphaarea[i]);
  }
  // cout<<".";
  for (int32_t i = 0; i < NBINS_EXTAREA; i++) {
    _extHistArea_v5->Fill(i, exthistarea_v5[i]);
    _extHistArea_acc_v5->Fill(i, exthistalphaarea_v5[i]);
    _extHistArea_rej_v5->Fill(i, exthistarea_v5[i]-exthistalphaarea_v5[i]);
  }
  // cout<<"done."<<endl;

  showHist_v5();
  //getchar();
}



void Plotter::showHist_v5()
{

  //calculate calibration values---------------------------------
  //for 100mV input, AREA_TO_MEV = 1/128*100*2^4*0.2*5.115/535.0 = 0.005975467
  // double ADC_mV = 1.0/128.0*(double)_inputRange;
  // double ADC_pVs = ADC_mV*SAMPLING_RATE*pow(2,(double)_areaHistScale);
  // double Area_MeV = ADC_pVs*ENERGY_SCALING_FACTOR_PVS_TO_MEV;
  double ADC_mV = _inputRange/128; //in mV
  double ADC_pVs = ADC_mV*SAMPLING_RATE*pow(2,(double)_areaHistScale);
  double extADC_pVs = ADC_mV*SAMPLING_RATE;
  double Area_MeV = ADC_pVs*ENERGY_SCALING_FACTOR_PVS_TO_MEV;
  double extArea_MeV = extADC_pVs*ENERGY_SCALING_FACTOR_PVS_TO_MEV;
  //cout<<" _inputRange = "<<_inputRange;
  //cout<<" ADC_mV      = "<<ADC_mV;
  //cout<<" ADC_pVs     = "<<ADC_pVs;
  //cout<<" Area_MeV    = "<<Area_MeV;
  //------------------------------------------------------------

  //-------------- Gaussian fit of the amplitude --------------
  TF1 *fit1 =  new TF1("gaussian1", "gaus", 0, 4095);
  fit1->SetNpx(10000);
  fit1->SetLineWidth(1);
  fit1->SetLineColor(kRed);
  //------------------------------------------------------------

  //draw to canvas
  if (!_can) {
    _can = new TCanvas("Real time Processing","Real time Processing",1280,800);
    _can->Divide(2,2);
  }
  gStyle->SetOptStat(0);
  _can->cd(1);
  gPad->SetLogy(true);
  _histFwhm_v4->Draw();
  _histFwhm_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);
  _histFwhm_v4->GetXaxis()->SetTitle("FWHM [ns]");
  _histFwhm_v4->GetYaxis()->SetTitle("Entries");
  _histFwhm_rej_v4->SetFillColor(kBlue-8);
  _histFwhm_rej_v4->SetLineColor(kBlue-8);
  _histFwhm_rej_v4->SetFillStyle(3004);
  _histFwhm_rej_v4->Draw("same");
  _histFwhm_rej_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);
  _histFwhm_acc_v4->SetFillColor(kRed+1);
  _histFwhm_acc_v4->SetLineColor(kRed+1);
  _histFwhm_acc_v4->Draw("same");
  _histFwhm_acc_v4->SetFillStyle(3005);
  _histFwhm_acc_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);

  //-------------- Gaussian fit of the width --------------
  fit1->SetRange(0.41,70);
  _histFwhm_acc_v4->Fit(fit1, "RQ");
  cout<<" FWHM results: "<<endl
      <<"  MEAN(Y)  "<<fit1->GetParameter(0)
      <<"  MEAN(X)  "<<fit1->GetParameter(1)
      <<"  SIGMA(X) "<<fit1->GetParameter(2)
      <<endl;
  //------------------------------------------------------------


  _can->cd(2);
  gPad->SetLogy(true);
  _histMaxampl_v4->Draw();
  _histMaxampl_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);
  _histMaxampl_v4->GetXaxis()->SetTitle("Amplitude [mV]");
  _histMaxampl_v4->GetYaxis()->SetTitle("Entries");
  _histMaxampl_rej_v4->SetFillColor(kBlue-8);
  _histMaxampl_rej_v4->SetLineColor(kBlue-8);
  _histMaxampl_rej_v4->SetFillStyle(3004);
  _histMaxampl_rej_v4->Draw("same");
  _histMaxampl_rej_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);
  _histMaxampl_acc_v4->SetFillColor(kRed+1);
  _histMaxampl_acc_v4->SetLineColor(kRed+1);
  _histMaxampl_acc_v4->Draw("same");
  _histMaxampl_acc_v4->SetFillStyle(3005);
  _histMaxampl_acc_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);

  //-------------- Gaussian fit of the amplitude --------------
  fit1->SetRange(10,70);
  _histMaxampl_acc_v4->Fit(fit1, "RQ");
  cout<<" AMPLITUDE results: "<<endl
      <<"  MEAN(Y)  "<<fit1->GetParameter(0)
      <<"  MEAN(X)  "<<fit1->GetParameter(1)
      <<"  SIGMA(X) "<<fit1->GetParameter(2)
      <<endl;
  //------------------------------------------------------------



  _can->cd(3);
  gPad->SetLogy(true);
  //_histArea_v4->Rebin(2);
  _histArea_v4->Draw();
  _histArea_v4->GetXaxis()->SetLimits(0,NBINS_AREA*ADC_pVs);
  _histArea_v4->GetXaxis()->SetTitle("Energy [MeV]");
  _histArea_v4->GetYaxis()->SetTitle("Entries");
  //_histArea_rej_v4->Rebin(2);
  _histArea_rej_v4->SetFillColor(kBlue-8);
  _histArea_rej_v4->SetLineColor(kBlue-8);
  _histArea_rej_v4->SetLineWidth(1);
  _histArea_rej_v4->SetFillStyle(3004);
  _histArea_rej_v4->Draw("same");
  _histArea_rej_v4->GetXaxis()->SetLimits(0,NBINS_AREA*ADC_pVs);
  //_histArea_acc_v4->Rebin(2);
  _histArea_acc_v4->SetFillColor(kRed+1);
  _histArea_acc_v4->SetLineColor(kRed+1);
  _histArea_acc_v4->SetLineWidth(1);
  _histArea_acc_v4->SetFillStyle(3005);
  _histArea_acc_v4->Draw("same");
  _histArea_acc_v4->GetXaxis()->SetLimits(0,NBINS_AREA*ADC_pVs);

    // _leg->SetBorderSize(0);
    // _leg->SetFillStyle(4000);
    // _leg->SetShadowColor(0);
    // _leg->SetLineColor(0);
    // _leg->SetTextSize(0.03);
    // _leg->SetFillColor(kWhite);
    cout.precision(2);
    stringstream ss;
    ss<< "Overall count:  "<<_nPulses;
    ss<< " at "<<(double)((int32_t)(_freq*100))/100.0<<"/s";
    _leg->AddEntry(_histArea_v4,ss.str().c_str(),"F");
    ss.str("");
    ss<< "Alpha count:    "<<_nPulses_acc;
    ss<< " at "<<(double)((int32_t)(_freq_acc*100))/100.0<<"/s";
    _leg->AddEntry(_histArea_acc_v4,ss.str().c_str(),"F");
    ss.str("");
    _leg->Draw("same");

  //-------------- Gaussian fit of the area --------------
  fit1->SetRange(100,6000);
  _histArea_acc_v4->Fit(fit1, "RQ");
  cout<<" AREA results: "<<endl
      <<"  MEAN(Y)  "<<fit1->GetParameter(0)
      <<"  MEAN(X)  "<<(int32_t)fit1->GetParameter(1)
      <<"  SIGMA(X) "<<fit1->GetParameter(2)
      <<endl;
  //------------------------------------------------------------


  _can->cd(4);
  gPad->SetLogy(true);
  //_extHistArea_v4->Rebin(2);
  _extHistArea_v5->Draw();
  //_extHistArea_v5->GetXaxis()->SetLimits(0,NBINS_EXTAREA*extArea_MeV);
  _extHistArea_v5->GetXaxis()->SetLimits(0,NBINS_EXTAREA*extADC_pVs);
  _extHistArea_v5->GetXaxis()->SetTitle("Energy [MeV]");
  _extHistArea_v5->GetYaxis()->SetTitle("Entries");
  //_extHistArea_rej_v5->Rebin(2);
  _extHistArea_rej_v5->SetFillColor(kBlue-8);
  _extHistArea_rej_v5->SetLineColor(kBlue-8);
  _extHistArea_rej_v5->SetLineWidth(1);
  _extHistArea_rej_v5->SetFillStyle(3004);
  _extHistArea_rej_v5->Draw("same");
  //_extHistArea_rej_v5->GetXaxis()->SetLimits(0,NBINS_EXTAREA*extArea_MeV);
  _extHistArea_rej_v5->GetXaxis()->SetLimits(0,NBINS_EXTAREA*extADC_pVs);

  //_extHistArea_acc_v5->Rebin(2);
  _extHistArea_acc_v5->SetFillColor(kRed+1);
  _extHistArea_acc_v5->SetLineColor(kRed+1);
  _extHistArea_acc_v5->SetLineWidth(1);
  _extHistArea_acc_v5->SetFillStyle(3005);
  _extHistArea_acc_v5->Draw("same");
  _extHistArea_acc_v5->GetXaxis()->SetLimits(0,NBINS_EXTAREA*extADC_pVs);
  //_extHistArea_acc_v5->GetXaxis()->SetLimits(0,NBINS_EXTAREA*extArea_MeV);



  _can->Modified();
  _can->Update();
}



void Plotter::setMonitorValues (int32_t fwhmThres=0,
                                int32_t nPulses=0,
                                int32_t nPulses_acc=0,
                                float   freq=0,
                                float   freq_acc=0,
                                double inputRange=100,
                                int32_t areaHistScale=0) {
  _fwhmThres=fwhmThres;
  _nPulses=nPulses;
  _nPulses_acc=nPulses_acc;
  _freq = freq;
  _freq_acc=freq_acc;
  _inputRange=inputRange;
  _areaHistScale=areaHistScale;
}













//-------------------------------------------------------------------
//                        HISTOGRAMMING V6
//-------------------------------------------------------------------

void Plotter::initHist_v6()
{
  if (_histFwhm_v4) delete _histFwhm_v4;
  _histFwhm_v4 = 0;
  if (_histFwhm_acc_v4) delete _histFwhm_acc_v4;
  _histFwhm_acc_v4 = 0;
  if (_histFwhm_rej_v4) delete _histFwhm_rej_v4;
  _histFwhm_rej_v4 = 0;
  if (_histMaxampl_v4) delete _histMaxampl_v4;
  _histMaxampl_v4 = 0;
  if (_histMaxampl_acc_v4) delete _histMaxampl_acc_v4;
  _histMaxampl_acc_v4 = 0;
  if (_histMaxampl_rej_v4) delete _histMaxampl_rej_v4;
  _histMaxampl_rej_v4 = 0;
  if (_histArea_v4) delete _histArea_v4;
  _histArea_v4 = 0;
  if (_histArea_acc_v4) delete _histArea_acc_v4;
  _histArea_acc_v4 = 0;
  if (_histArea_acc_fit_v4) delete _histArea_acc_fit_v4;
  _histArea_acc_fit_v4 = 0;
  if (_histArea_rej_v4) delete _histArea_rej_v4;
  _histArea_rej_v4 = 0;
  if (_extHistArea_v5) delete _extHistArea_v5;
  _extHistArea_v5 = 0;
  if (_extHistArea_acc_v5) delete _extHistArea_acc_v5;
  _extHistArea_acc_v5 = 0;
  if (_extHistArea_rej_v5) delete _extHistArea_rej_v5;
  _extHistArea_rej_v5 = 0;
  if (_extHistFwhmSlope_v6) delete _extHistFwhmSlope_v6;
  _extHistFwhmSlope_v6 = 0;
  if (_leg) delete _leg;
  _leg = 0;

  _histFwhm_v4 = new TH1D ("FWHM", "FWHM", NBINS,0,NBINS-1);
  _histFwhm_acc_v4 = new TH1D ("FWHM_acc", "FWHM_acc", NBINS,0,NBINS-1);
  _histFwhm_rej_v4 = new TH1D ("FWHM_rej", "FWHM_rej", NBINS,0,NBINS-1);
  _histMaxampl_v4 = new TH1D ("Amplitude", "Amplitude", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  _histMaxampl_acc_v4 = new TH1D ("Amplitude_acc", "Amplitude_acc", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  _histMaxampl_rej_v4 = new TH1D ("Amplitude_rej", "Amplitude_rej", NBINS_MAXAMPL,0,NBINS_MAXAMPL-1);
  _histArea_v4 = new TH1D ("Area", "Area", NBINS_AREA,0,NBINS_AREA-1);
  _histArea_acc_v4 = new TH1D ("Area_acc", "Area_acc", NBINS_AREA,0,NBINS_AREA-1);
  _histArea_acc_fit_v4 = new TH1D ("Area fit", "Area fit", NBINS_AREA,0,NBINS_AREA-1);
  _histArea_rej_v4 = new TH1D ("Area_rej", "Area_rej", NBINS_AREA,0,NBINS_AREA-1);
  _extHistArea_v5 = new TH1D ("Area", "Area", NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  _extHistArea_acc_v5 = new TH1D ("ExtArea_acc", "ExtArea_acc", NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  _extHistArea_rej_v5 = new TH1D ("ExtArea_rej", "ExtArea_rej", NBINS_EXTAREA,0,NBINS_EXTAREA-1);
  _extHistFwhmSlope_v6 = new TH2D ("Width vs Slope", "Width vs Slope",
                                   NBINS_FWHM_SLOPE_X, 0, NBINS_FWHM_SLOPE_X-1,
                                   NBINS_FWHM_SLOPE_Y, -NBINS_FWHM_SLOPE_Y+1, 0);
  _leg = new TLegend (0.65,0.75,0.9,0.9);
}


void Plotter::fillHist_v6(int32_t USBdatalen, uint8_t USBdata[262000]) {

  //just in case
  //_histArea_rej_v4=0;

  //print the buffer
  // for (int32_t i = 0; i < USBdatalen; i = i + 32)
  // {
  //   cout<<i<<" bytes"<<endl;
  //   for (int32_t j = 0; j < 32; j++)
  //     cout<<hex<<(uint32_t)USBdata[i+j]<<" ";
  //   cout<<dec<<endl;
  //
  //   if (!(i%100)) getchar();
  // }
  //hist buffers
  uint32_t histfwhm[256];
  uint32_t histmaxampl[256];
  uint32_t histalphamaxampl[256];
  // uint32_t histarea[256];
  // uint32_t histalphaarea[256];
  double histfwhm_v4[256];
  double histmaxampl_v4[256];
  // double histarea_v4[256];
  // double histalphaarea_v4[256];
  double exthistarea_v5[NBINS_EXTAREA];
  double exthistalphaarea_v5[NBINS_EXTAREA];
  double exthistfwhmslope_v6[NBINS_FWHM_SLOPE];
  int32_t  bin = 0;
  for (int32_t i = 0; i < 256; i++) {
    histfwhm[i]=0;
    histmaxampl[i]=0;
    histalphamaxampl[i]=0;
    //histarea[i]=0;
    //histalphaarea[i]=0;
    histfwhm_v4[i]=0;
    histmaxampl_v4[i]=0;
    //histarea_v4[i]=0;
    //histalphaarea_v4[i]=0;
  }
  for (int32_t i = 0; i < NBINS_EXTAREA; i++) {
    exthistarea_v5[i]=0;
    exthistalphaarea_v5[i]=0;
  }

  for (int32_t i = 0; i < NBINS_FWHM_SLOPE; i++) {
    exthistfwhmslope_v6[i]=0;
  }

  for (int32_t i = 0; i < USBdatalen-32; i = i + 32) {//read in 256 bits at a time.
    //256bit = 32 * 8bit

    //find header(64 bit 0xAA)
    int32_t headcnt = 0;
    for (int32_t j = 0; j < 2; j++) {
      if (USBdata[i+j] == 0xAA)
        headcnt++;
      else
        headcnt = 0;
    }
    if (headcnt != 2) {
      continue; // jump to the next 64 bit
    }

    //if it's header, fill the histograms with the next values.
    //first read the bin number. 16bit.
    bin = (uint32_t)(USBdata[i+2] << 8) |
          (uint32_t)(USBdata[i+3]);
    //bin value is actually one less - we get too high of a value from Fpga
    bin -= 1;

    //then get the values.
    //but not for bin 0, because it is corrupted.
    //if (bin==0) continue;

    exthistarea_v5[bin] = (uint32_t)(USBdata[i+4+27] << 0 ) |
                          (uint32_t)(USBdata[i+4+26] << 8 ) |
                          (uint32_t)(USBdata[i+4+25] << 16) |
                          (uint32_t)(USBdata[i+4+24] << 24);
    exthistalphaarea_v5[bin] =  (uint32_t)(USBdata[i+4+23] << 0 ) |
                                (uint32_t)(USBdata[i+4+22] << 8 ) |
                                (uint32_t)(USBdata[i+4+21] << 16) |
                                (uint32_t)(USBdata[i+4+20] << 24);

      // cout<<" bin "<<bin
      //   <<" extarea "<<exthistarea_v5[bin]
      //   <<" extalar "<<exthistalphaarea_v5[bin];

    if (bin>NBINS_FWHM_SLOPE-1) continue;

    exthistfwhmslope_v6[bin] =  (uint32_t)(USBdata[i+4+19] << 0 ) |
                                (uint32_t)(USBdata[i+4+18] << 8 ) |
                                (uint32_t)(USBdata[i+4+17] << 16) |
                                (uint32_t)(USBdata[i+4+16] << 24);

    cout<<" bin "<<bin
        <<" fwsl "<<exthistfwhmslope_v6[bin]
        <<endl;

    if (bin>255) continue;

    histfwhm[bin] = (uint32_t)(USBdata[i+4+3] << 0 ) |
                    (uint32_t)(USBdata[i+4+2] << 8 ) |
                    (uint32_t)(USBdata[i+4+1] << 16) |
                    (uint32_t)(USBdata[i+4+0] << 24);
    histmaxampl[bin] =  (uint32_t)(USBdata[i+4+7] << 0 ) |
                        (uint32_t)(USBdata[i+4+6] << 8 ) |
                        (uint32_t)(USBdata[i+4+5] << 16) |
                        (uint32_t)(USBdata[i+4+4] << 24);
    histalphamaxampl[bin] =   (uint32_t)(USBdata[i+4+11] << 0 ) |
                              (uint32_t)(USBdata[i+4+10] << 8 ) |
                              (uint32_t)(USBdata[i+4+9 ] << 16) |
                              (uint32_t)(USBdata[i+4+8 ] << 24);


    //
    // cout<<" fwhm "<<histfwhm[bin]
    //     <<" mamp "<<histmaxampl[bin]
    //     <<" area "<<histarea[bin]
    //     <<" alar "<<histalphaarea[bin]
    //     <<" fwsl "<<exthistfwhmslope_v6[bin];
    //    <<endl;
  }
  //reset all histograms
  // cout<<" Resetting histos"<<endl;
  initHist_v6();


  //fill all histograms
  for (int32_t i = 0; i < NBINS; i++) {
    _histFwhm_v4->Fill(i, histfwhm[i]);
    if (i>=_fwhmThres)
      _histFwhm_acc_v4->Fill(i, histfwhm[i]);
    else
      _histFwhm_rej_v4->Fill(i, histfwhm[i]);
  }
  // cout<<".";
  for (int32_t i = 0; i < NBINS_MAXAMPL; i++) {
    _histMaxampl_v4->Fill(i, histmaxampl[i]);
    _histMaxampl_acc_v4->Fill(i, histalphamaxampl[i]);
    _histMaxampl_rej_v4->Fill(i, histmaxampl[i]-histalphamaxampl[i]);
  }
  // cout<<".";
  // for (int32_t i = 0; i < NBINS_AREA; i++) {
  //   _histArea_v4->Fill(i, histarea[i]);
  //   _histArea_acc_v4->Fill(i, histalphaarea[i]);
  //   _histArea_acc_fit_v4->Fill(i, histalphaarea[i]);
  //   _histArea_rej_v4->Fill(i, histarea[i]-histalphaarea[i]);
  // }
  // cout<<".";
  for (int32_t i = 0; i < NBINS_EXTAREA; i++) {
    _extHistArea_v5->Fill(i, exthistarea_v5[i]);
    _extHistArea_acc_v5->Fill(i, exthistalphaarea_v5[i]);
    _extHistArea_rej_v5->Fill(i, exthistarea_v5[i]-exthistalphaarea_v5[i]);
  }
  // cout<<"done."<<endl;

  showHist_v6();
  //getchar();
}



void Plotter::showHist_v6()
{

  //calculate calibration values---------------------------------
  //for 100mV input, AREA_TO_MEV = 1/128*100*2^4*0.2*5.115/535.0 = 0.005975467
  // double ADC_mV = 1.0/128.0*(double)_inputRange;
  // double ADC_pVs = ADC_mV*SAMPLING_RATE*pow(2,(double)_areaHistScale);
  // double Area_MeV = ADC_pVs*ENERGY_SCALING_FACTOR_PVS_TO_MEV;
  double ADC_mV = _inputRange/128; //in mV
  double ADC_pVs = ADC_mV*SAMPLING_RATE*pow(2,(double)_areaHistScale);
  double extADC_pVs = ADC_mV*SAMPLING_RATE;
  double Area_MeV = ADC_pVs*ENERGY_SCALING_FACTOR_PVS_TO_MEV;
  double extArea_MeV = extADC_pVs*ENERGY_SCALING_FACTOR_PVS_TO_MEV;
  //cout<<" _inputRange = "<<_inputRange;
  //cout<<" ADC_mV      = "<<ADC_mV;
  //cout<<" ADC_pVs     = "<<ADC_pVs;
  //cout<<" Area_MeV    = "<<Area_MeV;
  //------------------------------------------------------------

  //-------------- Gaussian fit of the amplitude --------------
  TF1 *fit1 =  new TF1("gaussian1", "gaus", 0, 4095);
  fit1->SetNpx(10000);
  fit1->SetLineWidth(1);
  fit1->SetLineColor(kRed);
  //------------------------------------------------------------

  //draw to canvas
  if (!_can) {
    _can = new TCanvas("Real time Processing","Real time Processing",1280,800);
    _can->Divide(2,2);
  }
  gStyle->SetOptStat(0);
  _can->cd(1);
  gPad->SetLogy(true);
  _histFwhm_v4->Draw();
  _histFwhm_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);
  _histFwhm_v4->GetXaxis()->SetTitle("FWHM [ns]");
  _histFwhm_v4->GetYaxis()->SetTitle("Entries");
  _histFwhm_rej_v4->SetFillColor(kBlue-8);
  _histFwhm_rej_v4->SetLineColor(kBlue-8);
  _histFwhm_rej_v4->SetFillStyle(3004);
  _histFwhm_rej_v4->Draw("same");
  _histFwhm_rej_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);
  _histFwhm_acc_v4->SetFillColor(kRed+1);
  _histFwhm_acc_v4->SetLineColor(kRed+1);
  _histFwhm_acc_v4->Draw("same");
  _histFwhm_acc_v4->SetFillStyle(3005);
  _histFwhm_acc_v4->GetXaxis()->SetLimits(0,NBINS*SAMPLING_RATE);

  //-------------- Gaussian fit of the width --------------
  fit1->SetRange(0.41,70);
  _histFwhm_acc_v4->Fit(fit1, "RQ");
  cout<<" FWHM results: "<<endl
      <<"  MEAN(Y)  "<<fit1->GetParameter(0)
      <<"  MEAN(X)  "<<fit1->GetParameter(1)
      <<"  SIGMA(X) "<<fit1->GetParameter(2)
      <<endl;
  //------------------------------------------------------------


  _can->cd(2);
  gPad->SetLogy(true);
  _histMaxampl_v4->Draw();
  _histMaxampl_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);
  _histMaxampl_v4->GetXaxis()->SetTitle("Amplitude [mV]");
  _histMaxampl_v4->GetYaxis()->SetTitle("Entries");
  _histMaxampl_rej_v4->SetFillColor(kBlue-8);
  _histMaxampl_rej_v4->SetLineColor(kBlue-8);
  _histMaxampl_rej_v4->SetFillStyle(3004);
  _histMaxampl_rej_v4->Draw("same");
  _histMaxampl_rej_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);
  _histMaxampl_acc_v4->SetFillColor(kRed+1);
  _histMaxampl_acc_v4->SetLineColor(kRed+1);
  _histMaxampl_acc_v4->Draw("same");
  _histMaxampl_acc_v4->SetFillStyle(3005);
  _histMaxampl_acc_v4->GetXaxis()->SetLimits(0,NBINS_MAXAMPL*ADC_mV);

  //-------------- Gaussian fit of the amplitude --------------
  fit1->SetRange(10,70);
  _histMaxampl_acc_v4->Fit(fit1, "RQ");
  cout<<" AMPLITUDE results: "<<endl
      <<"  MEAN(Y)  "<<fit1->GetParameter(0)
      <<"  MEAN(X)  "<<fit1->GetParameter(1)
      <<"  SIGMA(X) "<<fit1->GetParameter(2)
      <<endl;
  //------------------------------------------------------------



  _can->cd(3);
  gPad->SetLogy(true);
  //_histArea_v4->Rebin(2);
  // _histArea_v4->Draw();
  // _histArea_v4->GetXaxis()->SetLimits(0,NBINS_AREA*ADC_pVs);
  // _histArea_v4->GetXaxis()->SetTitle("Energy [MeV]");
  // _histArea_v4->GetYaxis()->SetTitle("Entries");
  // //_histArea_rej_v4->Rebin(2);
  // _histArea_rej_v4->SetFillColor(kBlue-8);
  // _histArea_rej_v4->SetLineColor(kBlue-8);
  // _histArea_rej_v4->SetLineWidth(1);
  // _histArea_rej_v4->SetFillStyle(3004);
  // _histArea_rej_v4->Draw("same");
  // _histArea_rej_v4->GetXaxis()->SetLimits(0,NBINS_AREA*ADC_pVs);
  // //_histArea_acc_v4->Rebin(2);
  // _histArea_acc_v4->SetFillColor(kRed+1);
  // _histArea_acc_v4->SetLineColor(kRed+1);
  // _histArea_acc_v4->SetLineWidth(1);
  // _histArea_acc_v4->SetFillStyle(3005);
  // _histArea_acc_v4->Draw("same");
  // _histArea_acc_v4->GetXaxis()->SetLimits(0,NBINS_AREA*ADC_pVs);

  //   cout.precision(2);
  //   stringstream ss;
  //   ss<< "Overall count:  "<<_nPulses;
  //   ss<< " at "<<(double)((int32_t)(_freq*100))/100.0<<"/s";
  //   _leg->AddEntry(_histArea_v4,ss.str().c_str(),"F");
  //   ss.str("");
  //   ss<< "Alpha count:    "<<_nPulses_acc;
  //   ss<< " at "<<(double)((int32_t)(_freq_acc*100))/100.0<<"/s";
  //   _leg->AddEntry(_histArea_acc_v4,ss.str().c_str(),"F");
  //   ss.str("");
  //   _leg->Draw("same");
  //
  // //-------------- Gaussian fit of the area --------------
  // fit1->SetRange(100,6000);
  // _histArea_acc_v4->Fit(fit1, "RQ");
  // cout<<" AREA results: "<<endl
  //     <<"  MEAN(Y)  "<<fit1->GetParameter(0)
  //     <<"  MEAN(X)  "<<(int32_t)fit1->GetParameter(1)
  //     <<"  SIGMA(X) "<<fit1->GetParameter(2)
  //     <<endl;
  // //------------------------------------------------------------


  _can->cd(4);
  gPad->SetLogy(true);
  //_extHistArea_v4->Rebin(2);
  _extHistArea_v5->Draw();
  //_extHistArea_v5->GetXaxis()->SetLimits(0,NBINS_EXTAREA*extArea_MeV);
  _extHistArea_v5->GetXaxis()->SetLimits(0,NBINS_EXTAREA*extADC_pVs);
  _extHistArea_v5->GetXaxis()->SetTitle("Energy [MeV]");
  _extHistArea_v5->GetYaxis()->SetTitle("Entries");
  //_extHistArea_rej_v5->Rebin(2);
  _extHistArea_rej_v5->SetFillColor(kBlue-8);
  _extHistArea_rej_v5->SetLineColor(kBlue-8);
  _extHistArea_rej_v5->SetLineWidth(1);
  _extHistArea_rej_v5->SetFillStyle(3004);
  _extHistArea_rej_v5->Draw("same");
  //_extHistArea_rej_v5->GetXaxis()->SetLimits(0,NBINS_EXTAREA*extArea_MeV);
  _extHistArea_rej_v5->GetXaxis()->SetLimits(0,NBINS_EXTAREA*extADC_pVs);

  //_extHistArea_acc_v5->Rebin(2);
  _extHistArea_acc_v5->SetFillColor(kRed+1);
  _extHistArea_acc_v5->SetLineColor(kRed+1);
  _extHistArea_acc_v5->SetLineWidth(1);
  _extHistArea_acc_v5->SetFillStyle(3005);
  _extHistArea_acc_v5->Draw("same");
  _extHistArea_acc_v5->GetXaxis()->SetLimits(0,NBINS_EXTAREA*extADC_pVs);
  //_extHistArea_acc_v5->GetXaxis()->SetLimits(0,NBINS_EXTAREA*extArea_MeV);



  _can->Modified();
  _can->Update();
}
















//----------------  WAVEFORM ANALYSIS  -----------------------------------





void Plotter::analyseWaveform(int32_t USBdatalen, uint8_t USBdata[262000])
{
  //print the buffer
  for (int32_t i = 0; i < USBdatalen-32; i = i + 32)
  {
    //bool allZeros = true;
    //for (int32_t j = 0; j < 32; j++)
    //  if (USBdata[i+j]) allZeros = false;
    //if (allZeros)
    // continue;
    cout<<i<<" bytes"<<endl;
    for (int32_t j = 0; j < 32; j++)
      cout<<hex<<(uint32_t)USBdata[i+j]<<" ";
      cout<<dec<<endl;
      getchar();
  }

  //reset all histograms
  initHist_v4();

  int32_t cntHeader = 0;
  int32_t cntTrailer = 0;
  int64_t timestamp = -1;
  int64_t trigCount = -1;
  int32_t fwhmWidth = -1;
  int32_t maxAmplitude = -1;
  int32_t classification = -1;
  int32_t pulseArea = -1;
  int32_t bufX[300], bufY[300], nSamples=0;
  int32_t invert32 = 31;
  int32_t ofCnt = 0;
  bool pulseOverflow = false;
  bool header = false;
  bool maybeTrailer = false;
  bool trailer = false;
  bool data = false;
  //Read through buffer and find information on pulses
  for (int32_t i = 0; i < USBdatalen-32; i = i + 4) //read in 32 bits at a time.
  {
    cout<<endl;
    for (int32_t a = 0; a < 4; a++)
    {
      //cout<<a<<" ";
      cout<<hex<<(uint32_t)USBdata[i+a]<<" ";
      cout<<dec;//<<endl;
    }

    //----------read trailer---------------------------
    if (trailer) {
      //read fwhm width, max amplitude and classification
      fwhmWidth =    (int64_t)USBdata[i]    << 24 |
                     (int64_t)USBdata[i+1]  << 16 |
                     (int64_t)USBdata[i+2]  <<  8 |
                     (int64_t)USBdata[i+3]  <<  0;
      maxAmplitude = (int64_t)USBdata[i+4]  << 24 |
                     (int64_t)USBdata[i+5]  << 16 |
                     (int64_t)USBdata[i+6]  <<  8 |
                     (int64_t)USBdata[i+7]  <<  0;
      classification = (int64_t)USBdata[i+8]    << 24 |
                       (int64_t)USBdata[i+9]    << 16 |
                       (int64_t)USBdata[i+10]   <<  8 |
                       (int64_t)USBdata[i+11]   <<  0;
      pulseArea = (int64_t)USBdata[i+12]  << 24 |
                  (int64_t)USBdata[i+13]  << 16 |
                  (int64_t)USBdata[i+14]  <<  8 |
                  (int64_t)USBdata[i+15]  <<  0;
      i = i + 12;//to end the trailer and proceed to a new header
      trailer = false;
      cntTrailer = 0;

      //print out all information
      // cout<<"Pulse "<<trigCount<<" at "<<timestamp
      //     <<" maxAmplitude "<<maxAmplitude
      //     <<" fwhmWidth    "<<fwhmWidth
      //     <<" classification "<<classification<<endl;
      //show the graph
      //showWaveform();
      //getchar();
      _histFwhm_v4->Fill(fwhmWidth);
      _histMaxampl_v4->Fill(maxAmplitude);
      _histArea_v4->Fill(pulseArea);
      if (classification == 2)
        _histArea_acc_v4->Fill(pulseArea);

      if (1) {
      //if (classification == 2) {
        _wave = 0;
        _wave = new TGraph(nSamples,bufX,bufY);
        _wave->GetYaxis()->SetRangeUser(-10, 100);
        cout<<"Pulse "<<trigCount<<" at "<<timestamp
            <<" maxAmplitude "<<maxAmplitude
            <<" fwhmWidth    "<<fwhmWidth
            <<" classification "<<classification<<endl;
        showWaveform();
        getchar();
      }

      //if (_wave) delete _wave;
      //if (_can) delete _can;
      _can = 0;
      nSamples = 0;
      continue;
    }

    //----------read header----------------------------
    if (header) {
      //read timestamp and trigger counter
      timestamp = (int64_t)USBdata[i]    << 56 |
                  (int64_t)USBdata[i+1]  << 48 |
                  (int64_t)USBdata[i+2]  << 40 |
                  (int64_t)USBdata[i+3]  << 32 |
                  (int64_t)USBdata[i+4]  << 24 |
                  (int64_t)USBdata[i+5]  << 16 |
                  (int64_t)USBdata[i+6]  <<  8 |
                  (int64_t)USBdata[i+7]  <<  0;
      trigCount = (int64_t)USBdata[i+8]  << 56 |
                  (int64_t)USBdata[i+9]  << 48 |
                  (int64_t)USBdata[i+10] << 40 |
                  (int64_t)USBdata[i+11] << 32 |
                  (int64_t)USBdata[i+12] << 24 |
                  (int64_t)USBdata[i+13] << 16 |
                  (int64_t)USBdata[i+14] <<  8 |
                  (int64_t)USBdata[i+15] <<  0;
      i = i + 12; //to end the header and proceed to data
      data = true;
      header = false;
      cntHeader = 0;
      continue;
    }



    //----------search for header/trailer--------------
    for (int32_t j = 0; j < 4; j++) {
      if(!header) { // only check for header if not yet in header
        if (USBdata[i+j] == 0xCC )
          cntHeader++;
        else
          cntHeader = 0; //reset the counter if it's not header
        if (cntHeader == 16) {
          header = true;
          pulseOverflow = false;
          ofCnt = 0;
        } //test if header

      }
      if(!trailer) {
        if (USBdata[i+j] == 0xBB ) {
          cntTrailer++;
          maybeTrailer = true;
        }
        else
          cntTrailer = 0; //reset the counter if it's not trailer
        if (cntTrailer == 16) { //test if trailer
          trailer = true;
          maybeTrailer = false;
          data = false;
        }
      }
      continue;
    }//end of 32bit word

    //----------read data------------------------------
    if (data && !maybeTrailer && !pulseOverflow) {
      //take four 8bit numbers at a time and put them into a buffer
      for (int32_t j = 0; j < 4; j++) {
        bufX[nSamples] = nSamples;
        //make a signed number
        bufY[nSamples] = (int32_t)(int8_t)USBdata[i+j];
        nSamples++;
      }
      ofCnt+=4;
      if(ofCnt>=300) {
        pulseOverflow = true;
      }
    }


  }//end of buffer


  showHist_v4();



  _can = 0;


}

void Plotter::fillWaveform(int32_t USBdatalen, uint8_t USBdata[262000])
{
  // //print the buffer
  // for (int32_t i = 0; i < USBdatalen-32; i = i + 32)
  // {
  //   cout<<i<<" bytes"<<endl;
  //   for (int32_t j = 0; j < 32; j++)
  //     cout<<hex<<(uint32_t)USBdata[i+j]<<" ";
  //     cout<<dec<<endl;
  // }

  int32_t cntHeader = 0;
  int32_t cntTrailer = 0;
  int64_t timestamp = -1;
  int64_t trigCount = -1;
  int32_t fwhmWidth = -1;
  int32_t derivSlope = -1;
  int32_t maxAmplitude = -1;
  int32_t classification = -1;
  int32_t cutFlags = -1;
  int32_t pulseArea = -1;
  int32_t bufX[300], bufY[300], nSamples=0;
  int32_t invert32 = 31;
  int32_t ofCnt = 0;
  bool pulseOverflow = false;
  bool header = false;
  bool maybeTrailer = false;
  bool trailer = false;
  bool data = false;
  //Read through buffer and find information on pulses
  for (int32_t i = 0; i < USBdatalen-32; i = i + 4) //read in 32 bits at a time.
  {

    //----------read trailer---------------------------
    if (trailer) {
      //read fwhm width, max amplitude and classification
      derivSlope =   (int64_t)((int8_t)USBdata[i+3]-16);
      fwhmWidth =    (int64_t)USBdata[i+4]    << 24 |
                     (int64_t)USBdata[i+5]  << 16 |
                     (int64_t)USBdata[i+6]  <<  8 |
                     (int64_t)USBdata[i+7]  <<  0;
      maxAmplitude = (int64_t)USBdata[i+8]  << 24 |
                     (int64_t)USBdata[i+9]  << 16 |
                     (int64_t)USBdata[i+10]  <<  8 |
                     (int64_t)USBdata[i+11]  <<  0;
      cutFlags =       (int64_t)USBdata[i+12]    << 8 |
                       (int64_t)USBdata[i+13]    << 0;
      classification = (int64_t)USBdata[i+14]   <<  8 |
                       (int64_t)USBdata[i+15]   <<  0;
      pulseArea = (int64_t)USBdata[i+16]  << 24 |
                  (int64_t)USBdata[i+17]  << 16 |
                  (int64_t)USBdata[i+18]  <<  8 |
                  (int64_t)USBdata[i+19]  <<  0;
      i = i + 16;//to end the trailer and proceed to a new header
      trailer = false;
      cntTrailer = 0;

      //print out all information
      cout<<"Pulse "<<trigCount<<" at "<<timestamp
          <<"   maxAmplitude "<<maxAmplitude
          <<"   fwhmWidth "<<fwhmWidth
          <<"   cutFlags "<<cutFlags
          <<"   classification "<<classification
          <<"   area "<<pulseArea
          <<"   slope "<<derivSlope
          <<"   i "<<i
          <<endl;
      //show the graph

      // if (pulseArea > 600 || pulseArea < 5 ||
      //     maxAmplitude < 5 || maxAmplitude > 100 ||
      //     fwhmWidth < 5 || fwhmWidth > 200 ||
      //     derivSlope < -2) {
      //   _wave = new TGraph(nSamples,bufX,bufY);
      //   _wave->GetYaxis()->SetRangeUser(-10, 50);
      //   showWaveform();
      //   getchar();
      //   if (_wave) delete _wave;
      // }
      if (_can) delete _can;
      _can = 0;
      nSamples = 0;
      continue;
    }

    //----------read header----------------------------
    if (header) {
      //read timestamp and trigger counter
      timestamp = (int64_t)USBdata[i]    << 56 |
                  (int64_t)USBdata[i+1]  << 48 |
                  (int64_t)USBdata[i+2]  << 40 |
                  (int64_t)USBdata[i+3]  << 32 |
                  (int64_t)USBdata[i+4]  << 24 |
                  (int64_t)USBdata[i+5]  << 16 |
                  (int64_t)USBdata[i+6]  <<  8 |
                  (int64_t)USBdata[i+7]  <<  0;
      trigCount = (int64_t)USBdata[i+8]  << 56 |
                  (int64_t)USBdata[i+9]  << 48 |
                  (int64_t)USBdata[i+10] << 40 |
                  (int64_t)USBdata[i+11] << 32 |
                  (int64_t)USBdata[i+12] << 24 |
                  (int64_t)USBdata[i+13] << 16 |
                  (int64_t)USBdata[i+14] <<  8 |
                  (int64_t)USBdata[i+15] <<  0;
      i = i + 12; //to end the header and proceed to data
      data = true;
      header = false;
      cntHeader = 0;
      continue;
    }



    //----------search for header/trailer--------------
    for (int32_t j = 0; j < 4; j++) {
      if(!header) { // only check for header if not yet in header
        if (USBdata[i+j] == 0xCC )
          cntHeader++;
        else
          cntHeader = 0; //reset the counter if it's not header
        if (cntHeader == 16) {
          header = true;
          pulseOverflow = false;
          ofCnt = 0;
        } //test if header

      }
      if(!trailer) {
        if (USBdata[i+j] == 0xBB ) {
          cntTrailer++;
          maybeTrailer = true;
        }
        else
          cntTrailer = 0; //reset the counter if it's not trailer
        if (cntTrailer == 12) { //test if trailer
          trailer = true;
          maybeTrailer = false;
          data = false;
        }
      }
      continue;
    }//end of 32bit word

    //----------read data------------------------------
    if (data && !maybeTrailer && !pulseOverflow) {
      //take four 8bit numbers at a time and put them into a buffer
      for (int32_t j = 0; j < 4; j++) {
        bufX[nSamples] = nSamples;
        //make a signed number
        bufY[nSamples] = (int32_t)(int8_t)USBdata[i+j];
        nSamples++;
      }
      ofCnt+=4;
      if(ofCnt>=300) {
        pulseOverflow = true;
      }
    }

    //cout<<"buffer loop: "<<i<<endl;
  }//end of buffer

//cout<<" --------finish buffer -----"<<endl;


}

void Plotter::showWaveform()
{
  _can = 0;
  if (!_can)
    _can = new TCanvas("Real time Processing","Real time Processing",800,600);
  _wave->Draw();
  _can->Update();
}
