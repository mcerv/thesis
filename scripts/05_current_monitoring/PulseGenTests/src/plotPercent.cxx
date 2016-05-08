#include "defines.h"
#include "plots.h"
#include <string.h>
#include <vector>
#include <sstream>
//#include "histana.h"
#include "measset.h"
#include "drawfuns.h"
//root
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TApplication.h"

#define FIXFACTORAMPL 1.05

using namespace std;



void plotAmpl();
void plotWidth();
void plotArea();
void plotSNR();


// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {

  TApplication *app = new TApplication("app",0,0);

  plotSNR();

  plotAmpl();
  plotWidth();
  plotArea();



  delete(app);








  //delete psaSet;
  //delete scopeSet;
  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------




void plotAmpl() {

  //read a data file into a dataset
  MeasSet psaSet ("data/ampl/ampl-7bit_PSA.csv",15);
  MeasSet scopeSet ("data/ampl/ampl-7bit_scope.csv",15);
  DrawFuns dr;


  //calculate things
  vector<double> psaAmplEff, psaAmplEffErr;
  for (int32_t i=0; i<psaSet.nRuns; i++) {
    // psaAmplEff.push_back(psaSet.maxAmpl.at(i)/scopeSet.maxAmpl.at(i));
    psaAmplEff.push_back(100.0*FIXFACTORAMPL*psaSet.maxAmpl.at(i)/scopeSet.maxAmpl.at(i)-100);
    psaAmplEffErr.push_back( 100.0*dr.relError(psaSet.maxAmpl.at(i),
                                      psaSet.maxAmplStd.at(i),
                                      scopeSet.maxAmpl.at(i),
                                      scopeSet.maxAmplStd.at(i) ));
    cout<<" cur "<< psaSet.maxAmpl.at(i)/scopeSet.maxAmpl.at(i) <<endl;
  }


  //draw things
  TCanvas *c1 = new TCanvas("Amplitude","Amplitude",1200,800);
  TGraphErrors *grAmpl = new TGraphErrors(psaSet.nRuns, &scopeSet.maxAmpl[0], &psaAmplEff[0],
                                                  &psaAmplEffErr[0], &psaAmplEffErr[0] );
  dr.prettify(c1);
  dr.prettify(grAmpl);
  grAmpl->GetYaxis()->SetTitle("e_{ampl} [%]");
  grAmpl->GetXaxis()->SetTitle("Amplitude [mV]");
  grAmpl->Draw("AP");
  grAmpl->GetXaxis()->SetRangeUser(6,2000);
  grAmpl->GetXaxis()->SetLimits(6,2000);
  grAmpl->GetYaxis()->SetRangeUser(-21,21);
  grAmpl->GetYaxis()->SetLimits(-21,21);
  grAmpl->GetXaxis()->SetTitleSize(0.07);
  grAmpl->GetYaxis()->SetTitleSize(0.07);
  grAmpl->GetXaxis()->SetLabelSize(0.06);
  grAmpl->GetYaxis()->SetLabelSize(0.06);
  grAmpl->GetXaxis()->SetTitleOffset(1.1);
  grAmpl->GetYaxis()->SetTitleOffset(0.9);

  c1->SetLogx();
  c1->Update();
  c1->WaitPrimitive();

  delete(grAmpl);
  delete(c1);

}




void plotWidth() {

  //read a data file into a dataset
  MeasSet psaShortSet ("data/width/width_PSA_short.csv",15);
  //MeasSet psaLongSet ("data/width/width_PSA_long.csv",15);
  MeasSet scope200Set ("data/width/width_scope_200MHz.csv",15);
  MeasSet scopeSet ("data/width/width_scope.csv",15);
  DrawFuns dr;


  //calculate error bars
  vector<double> psaWidthEff, psaWidthEffErr;
  for (int32_t i=0; i<psaShortSet.nRuns; i++) {
    psaWidthEff.push_back(100.0*psaShortSet.width.at(i)/scopeSet.width.at(i)-100);
    psaWidthEffErr.push_back( 100.0*dr.relError(psaShortSet.width.at(i),
                                      psaShortSet.widthStd.at(i),
                                      scopeSet.width.at(i),
                                      scopeSet.widthStd.at(i) ));
    cout<<" cur "<< psaShortSet.width.at(i)/scopeSet.width.at(i) <<endl;
  }
  vector<double> psaWidth200Eff, psaWidth200EffErr;
  for (int32_t i=0; i<psaShortSet.nRuns; i++) {
    psaWidth200Eff.push_back(100.0*psaShortSet.width.at(i)/scope200Set.width.at(i)-100);
    psaWidth200EffErr.push_back( 100.0*dr.relError(psaShortSet.width.at(i),
                                      psaShortSet.widthStd.at(i),
                                      scope200Set.width.at(i),
                                      scope200Set.widthStd.at(i) ));
    cout<<" cur "<< psaShortSet.width.at(i)/scope200Set.width.at(i) <<endl;
  }



  //draw width
  TCanvas *c1 = new TCanvas("Width","Width",1200,800);
  TGraphErrors *grWidth = new TGraphErrors(psaShortSet.nRuns, &scopeSet.width[0], &psaWidthEff[0],
                                                  &scopeSet.widthStd[0], &psaWidthEffErr[0] );
  dr.prettify(c1);
  dr.prettify(grWidth);
  // grWidth->GetYaxis()->SetRangeUser(0,1.2);
  grWidth->GetYaxis()->SetTitle("e_{width} [%]");
  grWidth->GetXaxis()->SetTitle("Width [ns]");
  grWidth->GetXaxis()->SetRangeUser(0.9,200);
  grWidth->GetXaxis()->SetLimits(0.9,200);
  grWidth->Draw("AP");
  grWidth->GetXaxis()->SetRangeUser(0.9,200);
  grWidth->GetXaxis()->SetLimits(0.9,200);
  grWidth->GetYaxis()->SetRangeUser(-21,21);
  grWidth->GetYaxis()->SetLimits(-21,21);
  grWidth->GetXaxis()->SetTitleSize(0.07);
  grWidth->GetYaxis()->SetTitleSize(0.07);
  grWidth->GetXaxis()->SetLabelSize(0.06);
  grWidth->GetYaxis()->SetLabelSize(0.06);
  grWidth->GetXaxis()->SetTitleOffset(1.1);
  grWidth->GetYaxis()->SetTitleOffset(0.9);
  TLegend *leg = new TLegend (0.15,0.15, 0.5, 0.5);

  c1->SetLogx();
  c1->Update();
  c1->WaitPrimitive();

  //draw width 200
  TGraphErrors *grWidth200 = new TGraphErrors(psaShortSet.nRuns, &scope200Set.width[0], &psaWidth200Eff[0],
                                                  &scope200Set.widthStd[0], &psaWidth200EffErr[0] );
  dr.prettify(grWidth200);
  grWidth200->Draw("SAME P");
  grWidth200->SetMarkerStyle(24);
  c1->Update();
  c1->WaitPrimitive();





  delete(grWidth);
  delete(c1);

}



void plotArea() {

  //read a data file into a dataset
  MeasSet psaShortSet ("data/width/width_PSA_short.csv",15);
  //MeasSet psaLongSet ("data/width/width_PSA_long.csv",15);
  MeasSet scope200Set ("data/width/width_scope_200MHz.csv",15);
  MeasSet scopeSet ("data/width/width_scope.csv",15);
  DrawFuns dr;


  //calculate error bars
  vector<double> psaWidthEff, psaWidthEffErr;
  for (int32_t i=0; i<psaShortSet.nRuns; i++) {
    psaWidthEff.push_back(100.0*psaShortSet.area.at(i)/scopeSet.area.at(i)-100);
    psaWidthEffErr.push_back( 100.0*dr.relError(psaShortSet.area.at(i),
                                      psaShortSet.areaStd.at(i),
                                      scopeSet.area.at(i),
                                      scopeSet.areaStd.at(i) ));
    cout<<" cur "<< psaShortSet.area.at(i)/scopeSet.area.at(i) <<endl;
  }
  vector<double> psaWidth200Eff, psaWidth200EffErr;
  for (int32_t i=0; i<psaShortSet.nRuns; i++) {
    psaWidth200Eff.push_back(100.0*psaShortSet.area.at(i)/scope200Set.area.at(i)-100);
    psaWidth200EffErr.push_back( 100.0*dr.relError(psaShortSet.area.at(i),
                                      psaShortSet.areaStd.at(i),
                                      scope200Set.area.at(i),
                                      scope200Set.areaStd.at(i) ));
    cout<<" cur "<< psaShortSet.area.at(i)/scope200Set.area.at(i) <<endl;
  }



  //draw width
  TCanvas *c1 = new TCanvas("area","area",1200,800);
  TGraphErrors *grWidth = new TGraphErrors(psaShortSet.nRuns, &scopeSet.area[0], &psaWidthEff[0],
                                                  &scopeSet.areaStd[0], &psaWidthEffErr[0] );
  dr.prettify(c1);
  dr.prettify(grWidth);
  // grWidth->GetYaxis()->SetRangeUser(0,1.2);
  grWidth->GetYaxis()->SetTitle("e_{area} [%]");
  grWidth->GetXaxis()->SetTitle("Area [pVs]");
  grWidth->GetXaxis()->SetTitleSize(0.07);
  grWidth->GetYaxis()->SetTitleSize(0.07);
  grWidth->GetXaxis()->SetLabelSize(0.06);
  grWidth->GetYaxis()->SetLabelSize(0.06);
  grWidth->GetXaxis()->SetTitleOffset(1.1);
  grWidth->GetYaxis()->SetTitleOffset(0.9);
  // grWidth->GetXaxis()->SetRangeUser(0.9,200);
  // grWidth->GetXaxis()->SetLimits(0.9,200);
  // grWidth->GetYaxis()->SetRangeUser(0,150);
  // grWidth->GetYaxis()->SetLimits(0,150);
  grWidth->Draw("AP");
  c1->SetLogx();
  c1->Update();
  c1->WaitPrimitive();

  //draw width 200
  TGraphErrors *grWidth200 = new TGraphErrors(psaShortSet.nRuns, &scope200Set.area[0], &psaWidth200Eff[0],
                                                  &scope200Set.areaStd[0], &psaWidth200EffErr[0] );
  dr.prettify(grWidth200);
  grWidth200->Draw("SAME P");
  grWidth200->SetMarkerStyle(24);
  c1->Update();
  c1->WaitPrimitive();





  delete(grWidth);
  delete(c1);

}















void plotSNR() {

  //read a data file into a dataset
  MeasSet psaSet ("data/noise/noisemeas_PSA.csv",15);
  //MeasSet psaLongSet ("data/width/width_PSA_long.csv",15);

  MeasSet scopeSet ("data/width/noisemeas_scope.csv",15);
  DrawFuns dr;

  //calculate error bars
  vector<double> SNR, SNRErr;
  for (int32_t i=0; i<psaSet.nRuns; i++) {
    SNR.push_back(psaSet.maxAmplAvg.at(i)/psaSet.noise.at(i));
    SNRErr.push_back( psaSet.maxAmplAvg.at(i)/psaSet.noise.at(i) * 0.01 );
    cout<<" SNR "<< SNR.at(i) << " +- " << SNRErr.at(i) <<endl;
  }

double AreaAVG = 460;
double WidthAVG = 8;
double AmplAVG = 60;

vector<double> psaAreaEff, psaAreaEffErr;
for (int32_t i=0; i<psaSet.nRuns; i++) {
  psaAreaEff.push_back(100.0*psaSet.area.at(i)/AreaAVG-100);
  psaAreaEffErr.push_back( 100.0*psaSet.areaStd.at(i)/AreaAVG  );
  cout<<" cur "<< psaSet.area.at(i)/AreaAVG-100 <<endl;
}
vector<double> psaWidthEff, psaWidthEffErr;
for (int32_t i=0; i<psaSet.nRuns; i++) {
  psaWidthEff.push_back(100.0*psaSet.width.at(i)/WidthAVG-100);
  psaWidthEffErr.push_back( 100.0*psaSet.widthStd.at(i)/WidthAVG  );
  cout<<" cur "<< psaSet.width.at(i)/WidthAVG-100 <<endl;
}
vector<double> psaAmplEff, psaAmplEffErr;
for (int32_t i=0; i<psaSet.nRuns; i++) {
  psaAmplEff.push_back(100.0*psaSet.maxAmpl.at(i)/AmplAVG-100);
  psaAmplEffErr.push_back( 100.0*psaSet.maxAmplStd.at(i)/AmplAVG  );
  cout<<" cur "<< psaSet.maxAmpl.at(i)/AmplAVG-100 <<endl;
}


  //draw width
  TCanvas *c1 = new TCanvas("Width","Width",1200,800);
  TGraphErrors *grWidth = new TGraphErrors(psaSet.nRuns, &SNR[0], &psaWidthEff[0],
                                                  &SNRErr[0], &psaWidthEffErr[0] );
  TGraphErrors *grAmpl = new TGraphErrors(psaSet.nRuns, &SNR[0], &psaAmplEff[0],
                                                  &SNRErr[0], &psaAmplEffErr[0] );
  TGraphErrors *grArea = new TGraphErrors(psaSet.nRuns, &SNR[0], &psaAreaEff[0],
                                                  &SNRErr[0], &psaAreaEffErr[0] );
  dr.prettify(grWidth);
  dr.prettify(grAmpl);
  dr.prettify(grArea);

  dr.prettify(c1);
  c1->SetLogx();

  // grWidth->GetYaxis()->SetRangeUser(0,1.2);
  grWidth->GetYaxis()->SetTitle("e_{width} [%]");
  grWidth->GetXaxis()->SetTitle("SNR");
  grAmpl->GetYaxis()->SetTitle("e_{ampl} [%]");
  grAmpl->GetXaxis()->SetTitle("SNR");
  grArea->GetYaxis()->SetTitle("e_{area} [%]");
  grArea->GetXaxis()->SetTitle("SNR");

  grWidth->GetXaxis()->SetTitleSize(0.07);
  grWidth->GetYaxis()->SetTitleSize(0.07);
  grWidth->GetXaxis()->SetLabelSize(0.06);
  grWidth->GetYaxis()->SetLabelSize(0.06);
  grWidth->GetXaxis()->SetTitleOffset(1.1);
  grWidth->GetYaxis()->SetTitleOffset(0.9);
  // cout << "ndiv " << grWidth->GetXaxis()->GetNdivisions() << endl;
  // grWidth->GetYaxis()->SetNdivisions(grWidth->GetXaxis()->GetNdivisions()/100);
  grAmpl->GetXaxis()->SetTitleSize(0.07);
  grAmpl->GetYaxis()->SetTitleSize(0.07);
  grAmpl->GetXaxis()->SetLabelSize(0.06);
  grAmpl->GetYaxis()->SetLabelSize(0.06);
  grAmpl->GetXaxis()->SetTitleOffset(1.1);
  grAmpl->GetYaxis()->SetTitleOffset(0.9);
  grArea->GetXaxis()->SetTitleSize(0.07);
  grArea->GetYaxis()->SetTitleSize(0.07);
  grArea->GetXaxis()->SetLabelSize(0.06);
  grArea->GetYaxis()->SetLabelSize(0.06);
  grArea->GetXaxis()->SetTitleOffset(1.1);
  grArea->GetYaxis()->SetTitleOffset(0.9);


  grWidth->Draw("AP");
  grWidth->GetXaxis()->SetRangeUser(4,101);
  grWidth->GetXaxis()->SetLimits(4,101);
  grWidth->GetYaxis()->SetRangeUser(-21,21);
  grWidth->GetYaxis()->SetLimits(-21,21);

  c1->Update();
  c1->WaitPrimitive();
  grAmpl->Draw("AP");
  grAmpl->GetXaxis()->SetRangeUser(4,101);
  grAmpl->GetXaxis()->SetLimits(4,101);
  grAmpl->GetYaxis()->SetRangeUser(-21,21);
  grAmpl->GetYaxis()->SetLimits(-21,21);
  c1->Update();
  c1->WaitPrimitive();
  grArea->Draw("AP");
  grArea->GetXaxis()->SetRangeUser(4,101);
  grArea->GetXaxis()->SetLimits(4,101);

  grArea->GetYaxis()->SetRangeUser(-21,21);
  grArea->GetYaxis()->SetLimits(-21,21);
  c1->Update();
  c1->WaitPrimitive();





  delete(grWidth);
  delete(c1);

}
