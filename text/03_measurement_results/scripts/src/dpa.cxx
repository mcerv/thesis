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

  TGraph *gr[3];
  TMultiGraph *mg = new TMultiGraph();
  string fileName[3];
  fileName[0] = "data/DPAinDiamondP.csv";
  fileName[1] = "data/DPAinDiamondN.csv";
  fileName[2] = "data/DPAinDiamondPi.csv";
  string radType[3] = {"Protons","Neutrons","Pions"};
  TLegend* leg = new TLegend(0.6,0.7,0.95,0.95);
  // cout<<"Filename "<<fileName<<endl;

  for (int32_t i=0; i<3; i++) {
    gr[i] = new TGraph(fileName[i].c_str());
    dr->prettify(gr[i]);
    gr[i]->SetLineStyle(2*i+i);
    gr[i]->SetLineWidth(2);
    leg->AddEntry(gr[i],radType[i].c_str(),"l");
    mg->Add(gr[i]);
  }




  TCanvas* c2 = new TCanvas("together","together",800,600);
  dr->prettify(c2);
  gPad->SetLogx();
  gPad->SetLogy();
  mg->Draw("AL");
  dr->prettify(mg);
  mg->GetXaxis()->SetTitle("Energy [GeV]");
  mg->GetYaxis()->SetTitle("Displacements per atom");
  mg->GetYaxis()->SetRangeUser(5e-23,2.5e-21);
  // leg->AddEntry(lin,"Linearised PSI operating range","L");
  leg->Draw("same");

  TLatex *tex1 = new TLatex(0.0007,6e-23,"SIMULATION");
  tex1->SetTextFont(132);
  tex1->Draw("same");


  TFitResultPtr ptr;
  TF1* lin = new TF1("pol1","pol1");
  ptr = gr[2]->Fit(lin,"S","",0.2,0.4);
  //dpa at 300, 330, 270 MeV
  double n = ptr->Parameter(0);
  double k = ptr->Parameter(1);
  //y = k*x + n;
  double y300, y330, y270;
  y300 = k*0.3+n;
  y330 = k*0.33+n;
  y270 = k*0.27+n;
  cout<<" k = "<<k<<"  n = "<<n<<endl;
  cout << "DPA at 300 = "<<y300 <<endl
   << "DPA at 330 = "<<y330<< " , " << (y330-y300)/y300<<"\% of y300" <<endl
    << "DPA at 270 = "<<y270<< " , " << (y270-y300)/y300<<"\% of y300"
    <<endl<<flush;

  stringstream sstex;
  // sstex << "DPA (300#pm 10\% MeV) = "
  sstex << "DPA(PSI) = "
        << "(1.25#pm0.09)#times10 ^{-22}";
  TLatex *tex2 = new TLatex(0.12,2e-22,sstex.str().c_str());
  tex2->SetTextFont(132);
  tex2->Draw("same");


  // TPad *subpad = new TPad("subpad","",0.6,0.56,0.97,0.97);
  // subpad->Draw();
  // subpad->cd();
  // dr->prettify(subpad);
  // // subpad->SetFrameFillStyle(4000);
  // subpad->SetTopMargin(0);
  // subpad->SetBottomMargin(0);
  // subpad->SetLeftMargin(0);
  // subpad->SetRightMargin(0);
  // TFitResultPtr ptr;
  // ptr = gr[2]->Fit("pol1","","",0.2,0.4);
  // gr[2]->Draw("AL");
  // gr[2]->GetYaxis()->SetRangeUser(1e-22,2e-22);
  // // gr[2]->GetXaxis()->SetRangeUser(0.05,0.8);
  // gr[2]->GetXaxis()->SetLimits(0.2,0.4);
  // gr[2]->GetXaxis()->SetTitle("");
  // gr[2]->GetYaxis()->SetTitle("");
  // gr[2]->GetXaxis()->SetTicks("+-");
  // gr[2]->GetYaxis()->SetTicks("+-");
  // gPad->SetLogx();
  // gPad->SetLogy();


  c2->Update();
  c2->WaitPrimitive();
  c2->Update();


  // TMultiGraph* mg = new TMultiGraph();
  // dr->prettify(c2);
  // dr->prettify(gr[0]);
  // dr->prettify(gr[1]);
  // dr->prettify(gr[2]);
  // // gr[1]->SetLineColor(kBlue+1);
  // // gr[2]->SetLineColor(kBlack);
  // gr[1]->SetMarkerStyle(21);
  // gr[2]->SetMarkerStyle(22);
  // gr[0]->Draw("AP");
  // gr[0]->GetYaxis()->SetRangeUser(0,650);
  // gr[0]->GetXaxis()->SetRangeUser(0,1.6);
  // gr[1]->Draw("P SAME");
  // gr[2]->Draw("P SAME");
  // gr[0]->GetXaxis()->SetTitle("Electric field [V/ #mu m]");
  // gr[0]->GetYaxis()->SetTitle("Charge collection distance [ #mu m]");
  //
  // TLegend* leg1 = new TLegend(0.60,0.20,0.95,0.40);
  // leg1->AddEntry(gr[2],"S37 non-irrad","lep");
  // leg1->AddEntry(gr[0],"S79 1e14 #pi cm{}^{-2}","lep");
  // leg1->AddEntry(gr[1],"S52 3.63e14 #pi cm{}^{-2}","lep");
  // leg1->Draw("same");
  // c2->Update();
  // c2->WaitPrimitive();
  //
  //
  // //------- hist of radiation damage ---------------
  // // TGraph* hRad = new TGraph();
  // const float Lambda0 = 1;
  // const float k = 1.8e-18;
  // const float k14 = 1.8e-4;
  // const float irrad[10] = {0, 1, 3.63}; //e14
  // const float Lambda0k = Lambda0 * k;
  //
  // // y = Lambda0 / ( Lambda0*k* x  + 1 )
  //
  // TF1* fun = new TF1("f1","530 / ( 530 * 1.8e-4 * x + 1 ) ", -2, 20);
  // // fun->SetLineColor(kBlue);
  // fun->SetLineStyle(9);
  //
  // fun->Draw();
  // // c3->Update();
  // // c3->WaitPrimitive();
  //
  // //------ add points at 500V to rad plot ------------------
  // TGraphErrors* gp[3];
  // gp[0] = new TGraphErrors();
  // for (int32_t sample=0; sample<nSamples; sample++) {
  //   double x, y;
  //   double xe, ye;
  //   gr[sample]->GetPoint(0,x,y);
  //   ye=gr[sample]->GetErrorY(0);
  //   gp[0]->SetPoint (sample,radPtFlux[sample],y);
  //   gp[0]->SetPointError (sample,radPtFluxe[sample],ye);
  // }
  // dr->prettify(gp[0]);
  //
  // TF1* fitf = new TF1("fit","[0] / ([0]*[1]*x + 1) ", 0, 20);
  // fitf->SetParameter(0,500);
  // fitf->SetParameter(1,1.8e-4);
  // fitf->SetLineColor(kBlack);
  // TFitResultPtr ptr = gp[0]->Fit("fit","SR");
  // cout<<endl<<" --------------------------------"<<endl;
  // cout<<" Fitted k = "<<ptr->Parameter(1)*10000
  //     <<" +- "<<ptr->ParError(1)*10000
  //     <<" x 10^-18"
  //     <<endl;
  // cout<<" --------------------------------"<<endl;
  //
  // TLegend* leg2 = new TLegend(0.5,0.7,0.95,0.95);
  // leg2->SetTextFont(132);
  // leg2->AddEntry(fun,"RD42 fit: k = 1.8 #times 10^{-18}","l");
  // leg2->AddEntry(gp[0],"CCD at >500 V","lep");
  // // stringstream sskfit;
  // // sskfit.str("");
  // // sskfit << "Data fit: k_{fit} = " <<
  // //        << 3.2#pm
  // leg2->AddEntry(fitf, "Data fit: k_{fit} = 3.2 #pm 1 #times 10^{-18}","l");
  //
  //
  // // gp[0]->GetXaxis()->SetTitle("Radiation dose [10^{14} pions cm{}^{-2}]");
  // // gp[0]->GetYaxis()->SetTitle("Charge collection distance [#mum]");
  // // fitf->GetXaxis()->SetTitle("Radiation dose [10^{14} pions cm{}^{-2}]");
  // // fitf->GetYaxis()->SetTitle("Charge collection distance [#mum]");
  // TCanvas* c3 = new TCanvas("raddamage","raddamage",800,600);
  // dr->prettify(c3);
  // // c3->DrawFrame(fun->GetXmin(), fun->GetMinimum(),
  // //               fun->GetXmax(), fun->GetMaximum(),
  // //               "");
  // fitf->Draw();
  // gp[0]->Draw("AP");
  // fun->Draw("SAME");
  // leg2->Draw("same");
  // gp[0]->GetXaxis()->SetTitle("Radiation dose [10{}^{14} #pi cm{}^{-2}]");
  // gp[0]->GetYaxis()->SetTitle("Charge collection distance [ #mu m]");
  // gp[0]->GetXaxis()->SetRangeUser(-2,20);
  //
  // TLatex* lat = new TLatex();
  //
  //
  //
  // c3->Modified();
  // c3->Update();
  // c3->WaitPrimitive();
  // delete(c3);
  // //--------------------------------------------------------

  //------- hist of landaus ------------------------------
  // histLan[sample]
  //--------------------------------------------------------





  delete(dr);
  delete(app);

  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
