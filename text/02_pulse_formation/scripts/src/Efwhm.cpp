#include "drawfuns.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TF1.h"
#include "TGraph.h"
#include "Tlegend.h"
#include <iostream>
#include <sstream>

using namespace std;

int32_t main() {
  TApplication* app = new TApplication ("app",0,0);
  DrawFuns* dr = new DrawFuns();
  TF1* fun[2];
  TGraph* gr[2];
  stringstream ss;
  for (int32_t i=0; i<2; i++) {
    ss.str("");
    ss << "Efwhm_"<<i;
    fun[i] = new TF1(ss.str().c_str(), "1000*2.35*sqrt([0]*[1]*x)",0,10);
    gr[i] = new TGraph(fun[i]);
  }

  TCanvas* can = new TCanvas("fwhm","fwhm",900,600);

  //parameters for diamond
  fun[0]->SetParameter(0,13e-6);//Ei
  fun[0]->SetParameter(1,0.08);//fano factor
  //parameters for silicon
  fun[1]->SetParameter(0,3.6e-6);//Ei
  fun[1]->SetParameter(1,0.115);//fano factor
  // fun[1]->Draw("same");


  for (int32_t i=0; i<2; i++) {
    gr[i] = new TGraph(fun[i]);
  }
  dr->prettify(gr[0]);
  dr->prettify(gr[1]);
  gr[0]->GetXaxis()->SetTitle("Energy [MeV]");
  gr[0]->GetYaxis()->SetTitle("Intrinsic energy resolution [keV FWHM]");
  gr[0]->Draw("AL.");
  gr[1]->SetLineColor(kBlue+1);
  gr[1]->Draw("same");



  //legend
  TLegend* leg = new TLegend(0.7,0.8,0.97,0.97);
  leg->SetHeader("Energy resolution");
  leg->SetTextFont(132);
  leg->AddEntry(gr[0],"Diamond","l");
  leg->AddEntry(gr[1],"Silicon","l");
  leg->Draw();
  // fun[0]->GetXaxis()->SetRangeUser(0,50000);

  dr->prettify(can);
  can->Update();
  can->WaitPrimitive();
  delete(can);
  delete(dr);
  delete(app);
  cout<<"Done."<<endl;
  return 0;
}
