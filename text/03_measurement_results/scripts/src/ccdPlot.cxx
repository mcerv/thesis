#include <string.h>
#include <sstream>
#include "drawfuns.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"
#include "waveformana.h"
#include "TMultiGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include <fstream>

using namespace std;

//---fit function------
//---------------------

// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {

  DrawFuns* dr = new DrawFuns();
  TApplication* app = new TApplication("app",0,0);

  const int nSamples = 3;
  const int nVoltages = 15;

  // string sampleName[nSamples] = {"S79","S52","S37"};
  string sampleName[nSamples] = {"S52","S79","S37"};
  string realSampleName[nSamples] = {"S79","S52","S37"};
  float sampleThick[nSamples] = {515.0, 529.0, 548.0};  //um -- REAL

  TGraphErrors *gr[nSamples];
  TCanvas *c[nSamples];
  TCanvas *cHist;
  TH1F* histLan[10];
  TFile *f;
  cout<<"Starting"<<endl;
  float radPtFlux[3] = {3.63, 1, 0};
  float radPt20percentFluxe[3] = {0.726, 0.2, 0};
  float radPtFluxe[3];// = {0.726, 0.2, 0};
  int cntRadPt = 0;
  float radPoint[100];
  float radPointe[100];

  for (int32_t sample = 0; sample < nSamples; sample++) {
    radPtFluxe[sample] = sqrt (
          radPt20percentFluxe[sample]*radPt20percentFluxe[sample]
        + (0.06*radPtFlux[sample])*(0.06*radPtFlux[sample])
       );
  }


  for (int sample = 0; sample < nSamples; sample++) {
    float mean[100];
    float meane[100];
    float mpv[100];
    float mpve[100];
    float ccd[100];
    float ccde[100];
    float Q[100];
    float Qe[100];
    float field[100];
    float fielde[100];

    int num = 0;
    const float amp = 9.3; // mV/fC
    const float correctS37 = 1;//0.9;   // to account for surface problems
    const float correctS79 = 1;  // because I haven't taken highest CCD points
    // 1 C = 6.241e18 elecs ->
    const float fcToElecs = 6.241e3;

    string fileName = "data/ccd_";
    fileName += sampleName[sample];
    fileName += ".txt";
    cout<<"Filename "<<fileName<<endl;
    // istream textfile (fileName.c_str());
    // if (!textfile.is_open())
    // {
    //   cout<<"****File "<< fileName <<" not open properly!"<<endl;
    // }
    // else
    // {
    //   cout<<"File "<<fileName<<" open."<<endl;
    // }
    gr[sample] = new TGraphErrors(fileName.c_str());

    if (!sampleName[sample].compare("S37")) {
      for (int i=0;i<gr[sample]->GetN();i++) gr[sample]->GetY()[i] *= 0.92;
    }

    //c[sample] = new TCanvas(fileName.c_str(),fileName.c_str(),800,600);
    //dr->prettify(gr[sample]);
    //gr[sample]->Draw("AP");
    //c[sample]->Update();
    // c[sample]->WaitPrimitive();
    // c[sample]->Update();
  }



  TMultiGraph* mg = new TMultiGraph();
  mg->Add(gr[0]);
  mg->Add(gr[1]);
  mg->Add(gr[2]);

  TCanvas* c2 = new TCanvas("together","together",800,600);
  dr->prettify(c2);
  dr->prettify(gr[0]);
  dr->prettify(gr[1]);
  dr->prettify(gr[2]);
  // gr[1]->SetLineColor(kBlue+1);
  // gr[2]->SetLineColor(kBlack);
  gr[1]->SetMarkerStyle(21);
  gr[2]->SetMarkerStyle(22);
  gr[0]->Draw("AP");
  gr[0]->GetYaxis()->SetRangeUser(0,650);
  gr[0]->GetXaxis()->SetRangeUser(0,1.6);
  gr[1]->Draw("P SAME");
  gr[2]->Draw("P SAME");
  gr[0]->GetXaxis()->SetTitle("Electric field [V/ #mu m]");
  gr[0]->GetYaxis()->SetTitle("Charge collection distance [ #mu m]");

  TLegend* leg1 = new TLegend(0.57,0.20,0.965,0.44);
  leg1->AddEntry(gr[2],"S37 non-irrad","lep");
  leg1->AddEntry(gr[1],"S79 1e14 #pi cm{}^{-2}","lep");
  leg1->AddEntry(gr[0],"S52 3.63e14 #pi cm{}^{-2}","lep");
  leg1->Draw("same");
  c2->Update();
  c2->WaitPrimitive();


  //------- hist of radiation damage ---------------
  // TGraph* hRad = new TGraph();
  const float Lambda0 = 1;
  const float k = 1.8e-18;
  const float k14 = 1.8e-4;
  const float irrad[10] = {0, 1, 3.63}; //e14
  const float Lambda0k = Lambda0 * k;

  // y = Lambda0 / ( Lambda0*k* x  + 1 )

  TF1* fun = new TF1("f1","530 / ( 530 * 1.8e-4 * x + 1 ) ", -2, 20);
  // fun->SetLineColor(kBlue);
  fun->SetLineStyle(9);

  fun->Draw();
  // c3->Update();
  // c3->WaitPrimitive();

  //------ add points at 500V to rad plot ------------------
  TGraphErrors* gp[3];
  gp[0] = new TGraphErrors();
  for (int32_t sample=0; sample<nSamples; sample++) {
    double x, y;
    double xe, ye;
    gr[sample]->GetPoint(3,x,y);
    ye=gr[sample]->GetErrorY(3);
    if (!sample) {
      y = 1.1*y;
      ye = 1.1*ye;
    }
    gp[0]->SetPoint (sample,radPtFlux[sample],y);
    gp[0]->SetPointError (sample,radPtFluxe[sample],ye);
  }
  dr->prettify(gp[0]);

  TF1* fitf = new TF1("fit","[0] / ([0]*[1]*x + 1) ", 0, 20);
  fitf->SetParameter(0,500);
  fitf->SetParameter(1,2.2e-4);
  fitf->SetLineColor(kBlack);
  TFitResultPtr ptr = gp[0]->Fit("fit","SR");
  cout<<endl<<" --------------------------------"<<endl;
  cout<<" Fitted k = "<<ptr->Parameter(1)*10000
      <<" +- "<<ptr->ParError(1)*10000
      <<" x 10^-18"
      <<endl;
  cout<<" --------------------------------"<<endl;

  TLegend* leg2 = new TLegend(0.48,0.77,0.95,0.95);
  leg2->SetTextFont(42);
  leg2->AddEntry(fun,"RD42 fit: k = 2.2 #times 10^{-18}","l");
  leg2->AddEntry(gp[0],"CCD at 1 V/#mu m","lep");
  // stringstream sskfit;
  // sskfit.str("");
  // sskfit << "Data fit: k_{fit} = " <<
  //        << 3.2#pm
  leg2->AddEntry(fitf, "Data fit: k_{fit} = 4.4 #pm 1.2 #times 10^{-18} ","l");


  // gp[0]->GetXaxis()->SetTitle("Radiation dose [10^{14} pions cm{}^{-2}]");
  // gp[0]->GetYaxis()->SetTitle("Charge collection distance [#mum]");
  // fitf->GetXaxis()->SetTitle("Radiation dose [10^{14} pions cm{}^{-2}]");
  // fitf->GetYaxis()->SetTitle("Charge collection distance [#mum]");
  TCanvas* c3 = new TCanvas("raddamage","raddamage",800,600);
  dr->prettify(c3);
  // c3->DrawFrame(fun->GetXmin(), fun->GetMinimum(),
  //               fun->GetXmax(), fun->GetMaximum(),
  //               "");
  fitf->Draw();
  gp[0]->Draw("AP");
  fun->Draw("SAME");
  leg2->Draw("same");
  gp[0]->GetXaxis()->SetTitle("Radiation dose [10^{14} #pi cm^{-2}]");
  gp[0]->GetYaxis()->SetTitle("Charge collection distance [ #mu m]");
  gp[0]->GetXaxis()->SetRangeUser(-2,20);

  TLatex* lat = new TLatex();



  c3->Modified();
  c3->Update();
  c3->WaitPrimitive();
  delete(c3);
  //--------------------------------------------------------

  //------- hist of landaus ------------------------------
  // histLan[sample]
  //--------------------------------------------------------





  delete(dr);
  delete(app);

  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
