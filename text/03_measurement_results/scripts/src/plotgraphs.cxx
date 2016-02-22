#include <string.h>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include "drawfuns.h"
#include <vector>
#include "TFile.h"
#include "TF1.h"
#include "TApplication.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TLine.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLatex.h"
#include "TMultiGraph.h"
#include <math.h>

#define DEBUG 2
#define S52 0
#define S79 1
#define S37 2
#define S52_363e14 3
#define S79_1e14 4


using namespace std;

// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {
  //set global variables and objects
  double samplingTime = 0.1; //ns
  const int nSamples = 5;
  const int nVoltages = 10;
  const int nTemp = 18;

  // double voltage[nVoltages] = {700.0, 600.0, 500.0, 400.0, 300.0, 200.0, -200.0, -300.0, -400.0, -500.0, -600.0, -700.0};
  double voltage[nVoltages] = {700.0, 500.0, 400.0, 300.0, 200.0, -700.0, -500.0, -400.0, -300.0, -200.0};
  string sampleName[nSamples] = {"S52","S79","S37","S52_3-63e14","S79_1e14" };
  string tempS[nTemp] = {"4k","7k","10k","15k","25k","40k","60k","75k","95k",
              "115k","135k","160k","180k","210k","230k","260k","280k","295k"};
  double tempN[nTemp] = {4,7,10,15,25,40,60,75,95,115,135,160,180,210,230,260,280,295};
  // vector< vector< vector <TProfile> > > prof;

  //--------------get the profiles from the file ------------------------
  TProfile* prof[nSamples][nTemp][nVoltages];
  bool missingprof[nSamples][nTemp][nVoltages];
  stringstream ss, ssf;
  TFile* f = new TFile ("plots/pulses.root","read");// = new TFile();
  if (!f->IsOpen()) {
    cout<<" File " << ss.str().c_str() << " not open properly!" << endl;
    return 0;
  }
  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        ss.str("");
        ss << "Sample_" << sampleName[sample] << "/" << sampleName[sample]
           << "_" << tempS[temp] << "_" << voltage[volt];
        if (!(TProfile*)f->Get(ss.str().c_str() ) ) {
          cout<<" File " << ss.str().c_str() << " doesn't include " << ss.str() << endl;
          prof[sample][temp][volt] = new TProfile();
          missingprof[sample][temp][volt] = true;
          continue;
        }
      	prof[sample][temp][volt] = ((TProfile*)f->Get(ss.str().c_str() ));
        missingprof[sample][temp][volt] = false;
      }
    }
  } //----------------- end of reading in -------------------------



  //-------------------------------------------------------------
  //                    Plotting
  //-------------------------------------------------------------
  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();
  // TCanvas* can = new TCanvas("can","can",800,600);
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
  int color[22] = {2, 8, 7, 6, 4, 3, 2, 1 , 28, 26, 22, 16, 12, 11, 9, 8, 7, 6, 4, 3, 2, 1};
  int color2[22] = {1, 2, 3, 4, 6, 7, 8, 9, 1, 2, 3, 4, 6, 7, 8, 9, 1, 2, 3, 4, 6, 7};
  int color3[22] = {3, 5, 7, 9, 11, 13, 15, 17, 2, 4, 6, 8, 11, 12, 1, 4, 7, 11, 13, 16, 2, 5};

  Double_t clrScheme[6][20];
  // Double_t clrBase[6] = {kAzure, kTeal, kViolet, kPink, kGreen, kOrange};
  Double_t clrBase[6] = {kRed, kGreen,  kBlue, kViolet, kOrange, kBlack };
  for (int32_t sample=0; sample<6;sample++) {
    Double_t clr = clrBase[sample];
    for (int32_t temp=0; temp<20;temp++) {
      clrScheme[sample][temp] = clrBase[sample] + temp/4 - 4.0;
    }
  }





  // Change to graphs
  TGraph* gr[nSamples][nTemp][nVoltages];
  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        gr[sample][temp][volt] = new TGraph();
        for (int32_t i=0; i<prof[sample][temp][volt]->GetNbinsX(); i++) {
          gr[sample][temp][volt]->SetPoint(
            i, (-500 + i)*0.1 , prof[sample][temp][volt]->GetBinContent(i) );
        }
      }
    }
  }




  //----------------------------------------------------
  //      PLOTTING!
  //----------------------------------------------------
  stringstream sss;
  stringstream sssEH;
  stringstream sssEHsam;
  const int32_t nPol = 2; //number of polarities










  //
  // //----------------------------------------------------
  // //plot of nonirrad - irrad samples at +-500V across the temp range
  // //----------------------------------------------------
  // //variables
  // cout<<endl<<" Plotting nonirrad - irrad samples at +-500V across the temperature range "<<endl<<endl;
  // TCanvas* can2[nSamples][nTemp][nPol];
  // TMultiGraph* mg2[nSamples][nTemp][nPol];
  // TLegend* leg2[nSamples][nTemp][nPol];
  // TLatex* tex2[nSamples][nTemp][nPol];
  // TLatex* texEH2[nSamples][nTemp][nPol];
  // TLatex* texEH2sam[nSamples][nTemp][nPol];
  //
  // //initialise objects
  // for (int32_t sample=S52_363e14; sample<nSamples; sample++) {
  //     int32_t sampleNonIrrad;
  //     if (sample == S52_363e14) {
  //       sampleNonIrrad = S52; //500V
  //     } else if (sample == S79_1e14) {
  //       sampleNonIrrad = S79; //-500V
  //     }
  //   for (int32_t temp=0; temp<nTemp; temp++) {
  //     if (!(temp==0 || temp==4 || temp==8  || temp==9  || temp==10  || temp==13 || temp==14 || temp==15 || temp==17)) {
  //       continue;
  //     }
  //     for (int32_t i=0; i<nPol; i++) {
  //       mg2[sample][temp][i] = new TMultiGraph();
  //       sssEH.str("");
  //       sssEHsam.str("");
  //       sssEHsam << "#splitline{" << sampleName[sampleNonIrrad] << "}{T=" << tempN[temp] << "K}";
  //       if (!i) {
  //         sssEH << "electrons";
  //       } else {
  //         sssEH << "holes";
  //       }
  //       leg2[sample][temp][i] = new TLegend(0.6,0.8,0.95,0.95);
  //       tex2[sample][temp][i] = new TLatex(-1.5,-1.3,"MEASUREMENT");
  //       tex2[sample][temp][i]->SetTextFont(132);
  //       texEH2[sample][temp][i] = new TLatex(-1.5,9,sssEH.str().c_str());
  //       texEH2[sample][temp][i]->SetTextFont(22);
  //       texEH2sam[sample][temp][i] = new TLatex(11,2,sssEHsam.str().c_str());
  //       texEH2sam[sample][temp][i]->SetTextFont(22);
  //     }
  //   }
  // }
  //
  // //fill the multigraph with selected pulses
  // for (int32_t i=0; i<nPol; i++) {
  //   int32_t volt;
  //   if (!i) {
  //     volt = 1; //500V
  //   } else {
  //     volt = 6; //-500V
  //   }
  //   for (int32_t sample = S52_363e14; sample < nSamples; sample++) {
  //     int32_t clrCnt = 0;
  //     int32_t sampleNonIrrad;
  //     if (sample == S52_363e14) {
  //       sampleNonIrrad = S52; //500V
  //     } else if (sample == S79_1e14) {
  //       sampleNonIrrad = S79; //-500V
  //     }
  //     for (int32_t temp = 0; temp < nTemp; temp++) {
  //       if (!(temp==0 || temp==4 || temp==8  || temp==9  || temp==10  || temp==13 || temp==14 || temp==15 || temp==17)) {
  //         continue;
  //       }
  //       dr->prettify(gr[sample][temp][volt]);
  //       dr->prettify(gr[sampleNonIrrad][temp][volt]);
  //       gr[sample][temp][volt]->SetLineColor(dr->clrTemp[clrCnt+1]);
  //       gr[sampleNonIrrad][temp][volt]->SetLineColor(dr->clrTemp[clrCnt]);
  //       gr[sample][temp][volt]->SetLineWidth(3);
  //       gr[sampleNonIrrad][temp][volt]->SetLineWidth(3);
  //       //clrCnt++;
  //       mg2[sample][temp][i]->Add(gr[sample][temp][volt],"l");
  //       mg2[sample][temp][i]->Add(gr[sampleNonIrrad][temp][volt],"l");
  //       sss.str("");
  //       // sss << sampleName[sampleNonIrrad] << " ";
  //       if (sample == S52_363e14) {
  //         sss << "3.63";
  //       } else {
  //         sss << "1";
  //       }
  //       sss << " #times 10{}^{14} p/cm{}^{2} ";
  //
  //       leg2[sample][temp][i]->AddEntry(gr[sample][temp][volt],sss.str().c_str(),"L");
  //       sss.str("");
  //       // sss << sampleName[sampleNonIrrad] << " ";
  //       sss.str("");
  //       sss << "non-irrad";
  //       leg2[sample][temp][i]->AddEntry(gr[sampleNonIrrad][temp][volt],sss.str().c_str(),"L");
  //     }
  //   }
  // }
  //
  // //draw the pulses vs temp for individual samples and polarities
  // for (int32_t sample=S52_363e14; sample<nSamples; sample++) {
  //   int32_t sampleNonIrrad;
  //   if (sample == S52_363e14) {
  //     sampleNonIrrad = S52; //500V
  //   } else if (sample == S79_1e14) {
  //     sampleNonIrrad = S79; //-500V
  //   }
  //   for (int32_t temp = 0; temp < nTemp; temp++) {
  //     // sss.str("");
  //     // sss << sampleName[sampleNonIrrad] << " comparison";
  //     if (!(temp==0 || temp==4 || temp==8  || temp==9  || temp==10  || temp==13 || temp==14 || temp==15 || temp==17)) {
  //       continue;
  //     }
  //     for (int32_t i=0; i<nPol; i++) {
  //       sss.str("");
  //       sss << sampleName[sample] << "_";
  //       if (!i) sss << "-";
  //       sss << tempS[temp] << "_500";
  //       can2[sample][temp][i] = new TCanvas (sss.str().c_str(),sss.str().c_str(),800,600);
  //       can2[sample][temp][i]->cd(); //500V
  //       dr->prettify(can2[sample][temp][i]);
  //       mg2[sample][temp][i]->Draw("A");
  //       dr->prettify(mg2[sample][temp][i]);
  //       // mg2[sample][temp][i]->SetLineWidth(2);
  //       mg2[sample][temp][i]->GetXaxis()->SetRangeUser(-2,15);
  //       mg2[sample][temp][i]->GetYaxis()->SetRangeUser(-1.8,10.8);
  //       mg2[sample][temp][i]->GetXaxis()->SetTitle("Time [ns]");
  //       mg2[sample][temp][i]->GetYaxis()->SetTitle("Current [#mu A]");
  //       // tex2[sample][temp][i]->Draw("same");
  //       texEH2[sample][temp][i]->Draw("same");
  //       texEH2sam[sample][temp][i]->Draw("same");
  //       sss.str("");
  //       sss << sampleName[sampleNonIrrad];
  //       if (!i) {
  //         sss << " @ " << tempN[temp] << " K, 500 V";
  //       } else {
  //         sss << " @ " << tempN[temp] << " K, -500 V";
  //       }
  //       // leg2[sample][temp][i]->SetHeader(sss.str().c_str());
  //       leg2[sample][temp][i]->SetTextFont(132);
  //       leg2[sample][temp][i]->SetTextSize(0.04);
  //       leg2[sample][temp][i]->Draw("same");
  //       can2[sample][temp][i]->Update();
  //       can2[sample][temp][i]->Modified();
  //       // can[sample][temp][i]->WaitPrimitive();
  //       can2[sample][temp][i]->Update();
  //       sssEH.str("");
  //       sssEH << "plots/beforeafter/" << "comparison" << sampleName[sample];
  //       if (!i) sssEH << "_elecs";
  //       else sssEH << "_holes";
  //       sssEH << "_" << tempS[temp];
  //       sssEH << ".pdf";
  //       can2[sample][temp][i]->SaveAs(sssEH.str().c_str());
  //       // getchar();
  //       // can[i]->Clear();
  //       // delete can[i];
  //     }
  //   }
  // }
  // can2[nSamples-1][nTemp-1][nPol-1]->WaitPrimitive();
  //
  //
  //
  // cout<<" Press ENTER to proceed.. "<<endl;
  // getchar();
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  // //----------------------------------------------------
  // //plot of individual samples at several bias voltages at selected temps
  // //----------------------------------------------------
  // cout<<endl<<" Plotting individual samples at several bias voltages at selected temps "<<endl<<endl;
  // const int32_t nPlottedTemps = 2; //260 and 280 k
  // TCanvas* c1[nSamples][nPlottedTemps][nPol];
  // TMultiGraph* mg1[nSamples][nPlottedTemps][nPol];
  // TLegend* leg1[nSamples][nPlottedTemps][nPol];
  // TLatex* tex1[nSamples][nPlottedTemps][nPol];
  // TLatex* texEH1[nSamples][nPlottedTemps][nPol];
  // TLatex* texEH1sam[nSamples][nPlottedTemps][nPol];
  //
  // //initialise objects
  // for (int32_t sample=0; sample<nSamples; sample++) {
  //   int32_t sampleNonIrrad;
  //   if (sample == S52_363e14) {
  //     sampleNonIrrad = S52; //500V
  //   } else if (sample == S79_1e14) {
  //     sampleNonIrrad = S79; //-500V
  //   }
  //   for (int32_t temp=0; temp<nPlottedTemps; temp++) {
  //     int32_t temp1;
  //     if (!temp) {
  //       temp1 = 14;
  //     } else {
  //       temp1 = 15;
  //     }
  //     for (int32_t i=0; i<nPol; i++) {
  //       mg1[sample][temp][i] = new TMultiGraph();
  //       sss.str("");
  //       sssEH.str("");
  //       sssEHsam.str("");
  //       sss << sampleName[sample] << "_";
  //       sssEHsam << "#splitline{" << sampleName[sample] << "}{" << tempN[temp1] << " K}";
  //       if (!i) {
  //         sss << "-";
  //         sssEH << "electrons";
  //       } else {
  //         sssEH << "holes";
  //       }
  //       sss << tempS[temp1];
  //       leg1[sample][temp][i] = new TLegend(0.8,0.7,0.95,0.95);
  //       tex1[sample][temp][i] = new TLatex(-1.5,-1.3,"MEASUREMENT");
  //       tex1[sample][temp][i]->SetTextFont(132);
  //       texEH1[sample][temp][i] = new TLatex(-1.5,9,sssEH.str().c_str());
  //       texEH1[sample][temp][i]->SetTextFont(22);
  //       texEH1sam[sample][temp][i] = new TLatex(11,2,sssEHsam.str().c_str());
  //       texEH1sam[sample][temp][i]->SetTextFont(22);
  //     }
  //   }
  // }
  //
  // //fill the multigraph with selected pulses
  // int32_t nVoltPol = nVoltages/2;
  // for (int32_t sample = 0; sample < nSamples; sample++) {
  //   for (int32_t temp = 0; temp < nPlottedTemps; temp++) {
  //     int32_t clrCnt[2] = {0,0};
  //     int32_t temp1;
  //     if (!temp) {
  //       temp1 = 14;
  //     } else {
  //       temp1 = 15;
  //     }
  //     for (int32_t i=0; i<nPol; i++) {
  //       for (int32_t volt = i*nVoltPol; volt < (i+1)*nVoltPol; volt++) { //neg and pos
  //         if (missingprof[sample][temp1][volt]) {
  //           continue; //not a profile here
  //           clrCnt[i]++;
  //         }
  //         dr->prettify(gr[sample][temp1][volt]);
  //         gr[sample][temp1][volt]->SetLineColor(dr->clrVolt[clrCnt[i]]);
  //         gr[sample][temp1][volt]->SetLineWidth(3);
  //
  //         clrCnt[i]++;
  //         mg1[sample][temp][i]->Add(gr[sample][temp1][volt],"l");
  //         sss.str("");
  //         sss << voltage[volt] << " V";
  //         leg1[sample][temp][i]->AddEntry(gr[sample][temp1][volt],sss.str().c_str(),"L");
  //       }
  //     }
  //   }
  // }
  //
  // //draw the pulses vs temp for individual samples and polarities
  // for (int32_t sample=0; sample<nSamples; sample++) {
  //   for (int32_t temp=0; temp<nPlottedTemps; temp++) {
  //     int32_t temp1;
  //     if (!temp) {
  //       temp1 = 14;
  //     } else {
  //       temp1 = 15;
  //     }
  //
  //     for (int32_t i=0; i<nPol; i++) {
  //       sss.str("");
  //       sss << sampleName[sample] << " at "<< tempS[temp1];
  //       if(!i) sss<<"_elecs";
  //       else sss<<"_holes";
  //       c1[sample][temp][i] = new TCanvas (sss.str().c_str(),sss.str().c_str(),800,600);
  //       c1[sample][temp][i]->cd();
  //       dr->prettify(c1[sample][temp][i]);
  //       mg1[sample][temp][i]->Draw("A");
  //       dr->prettify(mg1[sample][temp][i]);
  //       mg1[sample][temp][i]->GetXaxis()->SetRangeUser(-2,15);
  //       mg1[sample][temp][i]->GetYaxis()->SetRangeUser(-1.8,10.8);
  //       mg1[sample][temp][i]->GetXaxis()->SetTitle("Time [ns]");
  //       mg1[sample][temp][i]->GetYaxis()->SetTitle("Current [#mu A]");
  //       // tex1[sample][temp][i]->Draw("same");
  //       texEH1[sample][temp][i]->Draw("same");
  //       texEH1sam[sample][temp][i]->Draw("same");
  //       // leg1[sample][temp][i]->SetHeader(sss.str().c_str());
  //       leg1[sample][temp][i]->SetTextFont(132);
  //       leg1[sample][temp][i]->SetTextSize(0.04);
  //       leg1[sample][temp][i]->Draw("same");
  //       c1[sample][temp][i]->Update();
  //       c1[sample][temp][i]->Modified();
  //       // can[sample][temp][i]->WaitPrimitive();
  //       c1[sample][temp][i]->Update();
  //       sssEH.str("");
  //       sssEH << "plots/pulsesVolt/" << "varVolt_" << sampleName[sample];
  //       if (!i) sssEH << "_elecs";
  //       else sssEH << "_holes";
  //       sssEH << "_" << tempS[temp1];
  //       sssEH << ".pdf";
  //       c1[sample][temp][i]->SaveAs(sssEH.str().c_str());
  //     }
  //   }
  // }
  // c1[nSamples-1][nPlottedTemps-1][nPol-1]->WaitPrimitive();










cout<<" Press ENTER to proceed.. "<<endl;
getchar();










  //----------------------------------------------------
  //plot of individual samples at +-500V across the temp range
  //----------------------------------------------------
  //variables
  cout<<endl<<" Plotting individual samples at +-500V across the temperature range "<<endl<<endl;
  TCanvas* can[nSamples][nPol];
  TMultiGraph* mg[nSamples][nPol];
  TLegend* leg[nSamples][nPol];
  TLatex* tex[nSamples][nPol];
  TLatex* texEH[nSamples][nPol];
  TLatex* texEHsam[nSamples][nPol];

  //initialise objects
  for (int32_t sample=0; sample<nSamples; sample++) {
    for (int32_t i=0; i<nPol; i++) {
      mg[sample][i] = new TMultiGraph();
      sss.str("");
      sssEH.str("");
      sssEHsam.str("");
      sss << sampleName[sample] << "_";
      sssEHsam << "#splitline{" << sampleName[sample] << "}{U_{bias}=";
      if (!i) {
        sss << "-";
        sssEHsam << "-";
        sssEH << "electrons";
      } else {
        sssEH << "holes";
      }
      sss << "500";
      sssEHsam << "500 V}";
      leg[sample][i] = new TLegend(0.8,0.5,0.95,0.95);
      tex[sample][i] = new TLatex(-1.5,-1.3,"MEASUREMENT");
      tex[sample][i]->SetTextFont(132);
      texEH[sample][i] = new TLatex(-1.5,9,sssEH.str().c_str());
      texEH[sample][i]->SetTextFont(22);
      texEHsam[sample][i] = new TLatex(11,2,sssEHsam.str().c_str());
      texEHsam[sample][i]->SetTextFont(22);
    }
  }

  //fill the multigraph with selected pulses
  int32_t volt;
  for (int32_t i=0; i<nPol; i++) {
    if (!i) {
      volt = 1; //500V
    } else {
      volt = 6; //-500V
    }
    for (int32_t sample = 0; sample < nSamples; sample++) {
      int32_t clrCnt = 0;
      for (int32_t temp = 0; temp < nTemp; temp++) {
        if (!(temp==0 || temp==4 || temp==8  || temp==9  || temp==10 || temp==11  || temp==13  || temp==15 || temp==17)) {
          continue;
        }
        dr->prettify(gr[sample][temp][volt]);
        gr[sample][temp][volt]->SetLineColor(dr->clrTemp[clrCnt]);
        gr[sample][temp][volt]->SetLineWidth(3);
        clrCnt++;
        mg[sample][i]->Add(gr[sample][temp][volt],"l");
        sss.str("");
        // if (!i) {
          sss << tempN[temp] << " K";
        // } else {
          // sss << tempN[temp] << " K @ -500 V";
        // }
        leg[sample][i]->AddEntry(gr[sample][temp][volt],sss.str().c_str(),"L");
        }
      }
    }

  //draw the pulses vs temp for individual samples and polarities
  for (int32_t sample=0; sample<nSamples; sample++) {
    for (int32_t i=0; i<nPol; i++) {
      sss.str("");
      sss << sampleName[sample] << " temp. range";
      if(!i) sss<<"_elecs";
      else sss<<"_holes";
      can[sample][i] = new TCanvas (sss.str().c_str(),sss.str().c_str(),800,600);
      dr->prettify(can[sample][i]);
      can[sample][i]->cd(); //500V
      mg[sample][i]->Draw("A");
      dr->prettify(mg[sample][i]);
      mg[sample][i]->GetXaxis()->SetRangeUser(-2,15);
      mg[sample][i]->GetYaxis()->SetRangeUser(-1.8,10.8);
      mg[sample][i]->GetXaxis()->SetTitle("Time [ns]");
      mg[sample][i]->GetYaxis()->SetTitle("Current [#mu A]");
      // tex[sample][i]->Draw("same");
      texEH[sample][i]->Draw("same");
      texEHsam[sample][i]->Draw("same");
      // leg[sample][i]->SetHeader(sss.str().c_str());
      leg[sample][i]->SetTextFont(132);
      leg[sample][i]->SetTextSize(0.04);
      leg[sample][i]->Draw("same");
      can[sample][i]->Update();
      can[sample][i]->Modified();
      // can[sample][i]->WaitPrimitive();
      can[sample][i]->Update();
      sssEH.str("");
      sssEH << "plots/pulses/" << sampleName[sample];
      if (!i) sssEH << "_elecs";
      else sssEH << "_holes";
      sssEH << ".pdf";
      can[sample][i]->SaveAs(sssEH.str().c_str());
      // getchar();
      // can[i]->Clear();
      // delete can[i];
    }
  }
  can[nSamples-1][nPol-1]->WaitPrimitive();


  //----------------------------------------------------
  //plot of individual samples at several bias voltages at selected temps
  //----------------------------------------------------
  cout<<endl<<" Plotting samples S52 and S79 before and after irrad for 500V across the temp range "<<endl<<endl;
































  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        delete prof[sample][temp][volt];
      }
    }
  }

  f->Close();
  delete(f);

  delete(app);
  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
