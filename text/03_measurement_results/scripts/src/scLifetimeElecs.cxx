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
#include "TParameter.h"
#include "TLine.h"
#include "TH2.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLatex.h"

#define DEBUG 0
#define S52 0
#define S79 1
#define S37 2
#define S52_363e14 3
#define S79_1e14 4
#define SCDHQ 5




using namespace std;


// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {
  cout<<"Plotting longterm C2 space charge lifetime.. ELECS"<<endl;

  stringstream ss;
  TFile* f;
  double samplingTime = 0.1; //ns
  const int nSamples = 1;
  const int nVoltages = 1;
  const int nTemp = 18;

  double voltage[nVoltages] = {500.0};
  int32_t polarity[nSamples] = {1};

  string sampleName[nSamples] = {"S79eafter"};

  string sampleNameFull[nSamples] = { "S79 1e14 #pi cm^{-2}"};

  string sampleNameState[nSamples] = {"pumped"};

  int32_t nFilesIndividual[nTemp] = { 142, 1, 1, 1,
                                        1, 1, 1, 1,
                                        1, 1, 1, 1,
                                        1, 1, 1, 1, 1, 1};
  int32_t numSkip[nTemp] = { 20, 1, 1, 1,
                                1, 1, 1, 1,
                                1, 1, 1, 1,
                                1, 1, 1, 1,
                                1, 1};

  string tempS[nTemp] = { "1",  "2",  "3",  "4",
                          "5",  "6",  "7",  "8",
                          "9", "10", "11", "12",
                         "13", "14", "15", "16",
                         "17", "18"};
  string tempName[nTemp] = {"Irradiation",
                            "Check after 1 min",
                            "Check after 3 min",
                            "Check after 10 min",
                            "Check after 30 min",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "",
                            ""
                          };
  double tempN[nTemp] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                        12, 13, 14, 15, 16, 17, 18};

  int32_t cntFiles = 0;
  int32_t cntFilesOpen = 0;
  int32_t cntPulses = 0;
  const int32_t nFiles = 200;
  // int32_t nFilesIndividual = nFiles;

  TProfile profvec[nSamples][nTemp][nFiles];
  TH2F pulse[nSamples][nTemp][nFiles];
  bool missingprof[nSamples][nTemp][nFiles];
  int32_t avgTimestamp[nSamples][nTemp][nFiles];


  //get the profiles and respective timestamps from the file
  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) { // only one  - "1"
      for (int32_t num = 0; num < nFilesIndividual[temp]; num+= numSkip[temp]) {
        ss.str("");
        ss << "/Users/mcerv/Documents/MarieCurieTALENT/Software/tmpScLifetime/"
           << sampleName[sample]
           << "/conv/";
        ss << sampleName[sample]
           << "_"
           << tempS[temp]
           << "_" << voltage[sample]
           << "V_" << num << ".root";
        TProfile prof;
        cntFiles++;
        cout << "\r" << "Opening file " << ss.str() << flush;// << endl;
        f = new TFile(ss.str().c_str(),"read");
        if (!f->IsOpen()) {
          cout<<" File " << ss.str().c_str() << " not open properly!" << endl;
          missingprof[sample][temp][num] = true;
          // profvec prof;
          continue;
        }
        cntFilesOpen++;
        // if (!(TProfile*)f->Get("avgpulses_filt")) {
        if (!(TProfile*)f->Get("avgpulses")) {
          cout<<" File " << ss.str().c_str() << " doesn't include avgpulses!" << endl;
          missingprof[sample][temp][num] = true;
          continue;
        }
        if (!(TH2F*)f->Get("pulses")) {
          cout<<" File " << ss.str().c_str() << " doesn't include pulses!" << endl;
          missingprof[sample][temp][num] = true;
          continue;
        }
        if (!((TParameter<Int_t>*)f->Get("avgTimestamp")) ) {
          cout<<" File " << ss.str().c_str() << " doesn't include avgTimestamp!" << endl;
          missingprof[sample][temp][num] = true;
          continue;
        }
        avgTimestamp[sample][temp][num] = (((TParameter<Int_t>*)f->Get("avgTimestamp"))->GetVal());

        cntPulses++;
      	prof = (*(TProfile*)f->Get("avgpulses"));
        prof.Scale(131.7);
        prof.GetXaxis()->SetRangeUser(-5,20); //nanoseconds
        ss.str("");
        ss << sampleName[sample] << "_" << tempS[temp] << "_" << num;
        prof.SetName(ss.str().c_str());
        profvec[sample][temp][num] = prof;
        pulse[sample][temp][num] = (*(TH2F*)f->Get("pulses"));
        missingprof[sample][temp][num] = false;
        f->Close();
        delete(f);
      }
    }
  }
  cout<<endl<<endl<<endl;
  cout<<" Files requested: "<<cntFiles
      <<", failed to open: "<<cntFiles-cntFilesOpen
      <<", no pulse found: "<<cntFiles-cntPulses
      <<endl<<endl;


  // ------------------------------------------------------------------
  //                      PLOTTING
  // ------------------------------------------------------------------
  stringstream sssEH, sssEHsam, sss;

  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();
  TCanvas* can[nTemp][nFiles];
  // TMultiGraph* mg[nSamples];
  TLegend* leg[nTemp][nFiles];
  TLatex* tex1[nSamples][nTemp];
  TLatex* texEH1[nSamples][nTemp];
  TLatex* texEH1sam[nSamples][nTemp];
  int32_t clrCnt = 0;
  // Change to graphs
  // TGraph* gr[nSamples][nTemp][nFiles];

  for (int32_t sample = 0; sample < nSamples; sample++) {
    // mg[sample] = new TMultiGraph();
    for (int32_t temp = 0; temp < nTemp; temp++) {

      clrCnt = 0;
      for (int32_t num = 0; num < nFilesIndividual[temp]; num+=numSkip[temp]) {
        if (missingprof[sample][temp][num])
          continue;
        leg[temp][num] = new TLegend(0.83,0.5,0.95,0.95);
        ss.str("");
        ss << sampleName[sample] << "_" << tempS[temp] << "_" << num;
        cout << ss.str() <<endl;
        // gr[sample][temp][num] = new TGraph();
        // gr[sample][temp][num]->SetName(ss.str().c_str());
        // for (int32_t i=0; i<profvec[sample][temp][num].GetNbinsX(); i++) {
        //   gr[sample][temp][num]->SetPoint(
        //     i, (-500 + i)*0.1 , profvec[sample][temp][num].GetBinContent(i) );
        // }
        dr->prettify(&pulse[sample][temp][num]);
        // gr[sample][temp][num]->SetLineColor(dr->clrTemp[clrCnt]);
        clrCnt++;
        // mg[sample]->Add(gr[sample][temp][num]);
        ss.str("");
        ss << avgTimestamp[sample][temp][num] << " s";
        leg[temp][num]->AddEntry(&pulse[sample][temp][num], ss.str().c_str(),  "L");
      }
      sss.str("");
      sssEH.str("");
      sssEHsam.str("");
      sss << sampleName[sample] << "_";
      sssEHsam << "#splitline{#splitline{" << tempName[temp] << "}{"
               << "U_{bias}=" << voltage[polarity[sample]] << "V}}{"
               << sampleNameState[sample] << "}";
      if (polarity[sample])
        sssEH << "electrons"; //500V
      else
        sssEH << "holes";
      sss << tempS[temp];
      tex1[sample][temp] = new TLatex(-1.5,-1.3,"MEASUREMENT");
      tex1[sample][temp]->SetTextFont(132);
      texEH1[sample][temp] = new TLatex(-1.5,10.5,sssEH.str().c_str());
      texEH1[sample][temp]->SetTextFont(22);
      texEH1sam[sample][temp] = new TLatex(10,1.7,sssEHsam.str().c_str());
      texEH1sam[sample][temp]->SetTextFont(22);
    }
  }

  for (int32_t sample=0; sample<nSamples; sample++) {
    for (int32_t temp=0; temp<nTemp; temp++) {
      for (int32_t num=0; num<nFilesIndividual[temp]; num+=numSkip[temp]) {
        sss.str("");
        sss << "can_" << sample << "_" << temp << "_" << num;
        can[temp][num] = new TCanvas(sss.str().c_str(),sss.str().c_str(),800,600);
        dr->prettify(can[temp][num]);
        pulse[sample][temp][num].Draw("colz");
        // dr->prettify(mg[sample]);
        pulse[sample][temp][num].GetXaxis()->SetTitle("Time [ns]");
        pulse[sample][temp][num].GetYaxis()->SetTitle("Amplitude [V]");
        pulse[sample][temp][num].GetYaxis()->SetRangeUser(-0.02,0.15); //[V]
        pulse[sample][temp][num].GetXaxis()->SetRangeUser(-2,30); //[s]
        leg[temp][num]->Draw("same");
        tex1[sample][temp]->Draw("same");
        texEH1[sample][temp]->Draw("same");
        texEH1sam[sample][temp]->Draw("same");
        can[temp][num]->Update();
        can[temp][num]->WaitPrimitive();
        ss.str("");
        ss << "plots/scLifetime/" << sampleName[sample] << "_" << tempS[temp] << "_" << num << ".pdf";
        // can[sample]->SaveAs(ss.str().c_str());
        can[temp][num]->Update();
      }
    }
  }

  // delete can;
  // delete leg;
  delete dr;
  delete app;




  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
