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
  cout<<"Grabbing pulse profiles and plotting a longterm C2 decrease.."<<endl;

  stringstream ss;
  TFile* f;
  double samplingTime = 0.1; //ns
  const int nSamples = 4;
  const int nVoltages = 4; //but two polarities
  const int nTemp = 1;
  const int nPol = 2; //0 - holes, 1 - elecs
  double voltage[nPol][nVoltages] = {-300.0, -500.0, -700.0, -900.0, 300.0, 500.0, 700.0, 900.0};

  int32_t polarity[nSamples] = {0, 0, 1, 1}; //0-holes, 1-electrons

  string sampleName[nSamples] = {"S79", "S79after", "S79e", "S79eafter"};

  string sampleNameFull[nSamples] = {
    "S79 1e14 #pi cm^{-2}",   "S79 1e14 #pi cm^{-2}",   "S79 1e14 #pi cm^{-2}",   "S79 1e14 #pi cm^{-2}"};

  string sampleNameState[nSamples] = {
    "not pumped","pumped","not pumped", "pumped"};

  int32_t nFilesIndividual[nSamples][nVoltages] = { 99, 83, 110, 84,
                                                    312, 301, 346, 480,
                                                    83, 101, 94, 0,
                                                    254, 375, 496, 0 };
  int32_t numSkip[nSamples][nVoltages] = { 9 , 8,  11, 8,
                               31, 30, 34, 48,
                               8, 10, 9, 0,
                               25, 37, 49, 0};

  int32_t numStart[nSamples][nVoltages] = { 0, 0, 0, 0,
                                            0, 0, 0, 0,
                                            0, 0, 0, 0,
                                            0, 0, 0, 0 };

  int32_t numReal[nSamples][nVoltages] = { 0, 0, 0, 0,
                                           0, 0, 0, 0,
                                           0, 0, 0, 0,
                                           0, 0, 0, 0 };

  string tempS[nTemp] = {"1"};
  double tempN[nTemp] = {1};

  int32_t cntFiles = 0;
  int32_t cntFilesOpen = 0;
  int32_t cntPulses = 0;
  const int32_t nFiles = 50;
  // int32_t nFilesIndividual = nFiles;

  cout << "Creating buffers for the profiles ... ";
  TProfile profvec[nSamples][nVoltages][nFiles];
  bool missingprof[nSamples][nVoltages][nFiles];
  int32_t avgTimestamp[nSamples][nVoltages][nFiles];
  cout << "done." << endl;

  // TApplication *ap = new TApplication("a",0,0);
  // TCanvas* c1 = new TCanvas("c","c",800,600);


  //get the profiles and respective timestamps from the file
  for (int32_t sample = 0; sample < nSamples; sample++) {
    int32_t pol = polarity[sample];
    for (int32_t volt = 0; volt < nVoltages; volt++) { // only one  - "1"
      for (int32_t num = numStart[sample][volt]; num < nFilesIndividual[sample][volt]; num+= numSkip[sample][volt]) {
        ss.str("");
        ss << "/Volumes/WINSTORAGE/Meas/Hysteresis/2015-08-18/voltIncrease/"
           << sampleName[sample]
           << "/conv/";
        ss << sampleName[sample]
           << "_1_" << voltage[pol][volt]
           << "V_" << num << ".root";
        TProfile prof;
        cntFiles++;
        cout << "\r" << "Opening file " << ss.str() << flush;// << endl;
        f = new TFile(ss.str().c_str(),"read");
        if (!f->IsOpen()) {
          cout<<" File " << ss.str().c_str() << " not open properly!" << endl;
          // missingprof[sample][volt][numReal[sample][volt]] = true;
          // numReal[sample][volt]++;
          // profvec prof;
          continue;
        }
        cntFilesOpen++;
        if (!(TProfile*)f->Get("avgpulses")) {
          cout<<" File " << ss.str().c_str() << " doesn't include avgpulses!" << endl;
          // missingprof[sample][volt][numReal[sample][volt]] = true;
          // numReal[sample][volt]++;
          continue;
        }
        if (!((TParameter<Int_t>*)f->Get("avgTimestamp")) ) {
          cout<<" File " << ss.str().c_str() << " doesn't include avgTimestamp!" << endl;
          // missingprof[sample][volt][numReal[sample][volt]] = true;
          // numReal[sample][volt]++;
          continue;
        }
        avgTimestamp[sample][volt][numReal[sample][volt]] = (((TParameter<Int_t>*)f->Get("avgTimestamp"))->GetVal());

        cntPulses++;
      	prof = (*(TProfile*)f->Get("avgpulses"));
        prof.Scale(131.7);
        prof.GetXaxis()->SetRangeUser(-5,20); //nanoseconds
        ss.str("");
        ss << sampleName[sample] << "_" << voltage[pol][volt] << "_" << num;
        prof.SetName(ss.str().c_str());
        profvec[sample][volt][numReal[sample][volt]] = prof;
        // missingprof[sample][volt][numReal[sample][volt]] = false;

        // prof.Draw();
        // c1->Update();
        // c1->WaitPrimitive();
        // c1->Update();
        // delete c1;
        // delete ap;
        numReal[sample][volt]++;
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
  TCanvas* can[nSamples][nVoltages];
  TMultiGraph* mg[nSamples][nVoltages];
  TLegend* leg[nSamples][nVoltages];
  TLatex* tex1[nSamples][nVoltages];
  TLatex* texEH1[nSamples][nVoltages];
  TLatex* texEH1sam[nSamples][nVoltages];
  // Change to graphs
  TGraph* gr[nSamples][nVoltages][nFiles];

  cout << " Making graphs " << endl << flush;
  for (int32_t sample = 0; sample < nSamples; sample++) {
    int32_t pol = polarity[sample];
    for (int32_t volt = 0; volt < nVoltages; volt++) {
      mg[sample][volt] = new TMultiGraph();
      leg[sample][volt] = new TLegend(0.83,0.5,0.95,0.95);
      int32_t clrCnt = 0;
      // for (int32_t num = numStart[sample][volt]; num < nFilesIndividual[sample][volt]; num+= numSkip[sample][volt]) {
      for (int32_t num = 0; num < numReal[sample][volt]; num++) {
        ss.str("");
        ss << sampleName[sample] << "_" << voltage[pol][volt] << "_" << num;
        gr[sample][volt][num] = new TGraph();
        gr[sample][volt][num]->SetName(ss.str().c_str());
        for (int32_t i=0; i<profvec[sample][volt][num].GetNbinsX(); i++) {
          gr[sample][volt][num]->SetPoint(
            i, (-500 + i)*0.1 , profvec[sample][volt][num].GetBinContent(i) );
        }
        dr->prettify(gr[sample][volt][num]);
        // gr[sample][volt][num]->Draw();
        // c1->Update();
        // c1->WaitPrimitive();
        // c1->Update();
        gr[sample][volt][num]->SetLineColor(dr->clrTemp[clrCnt]);
        clrCnt++;
        mg[sample][volt]->Add(gr[sample][volt][num]);
        ss.str("");
        ss << avgTimestamp[sample][volt][num] << " s";
        leg[sample][volt]->AddEntry(gr[sample][volt][num], ss.str().c_str(),  "L");
      }
      sss.str("");
      sssEH.str("");
      sssEHsam.str("");
      sss << sampleName[sample] << "_";
      sssEHsam << "#splitline{#splitline{" << sampleNameFull[sample] << "}{"
      << "U_{bias}=" << voltage[pol][volt] << "V}}{"
      << sampleNameState[sample] << "}";
      if (polarity[sample])
        sssEH << "electrons"; //500V
      else
        sssEH << "holes";
      sss << voltage[pol][volt];
      tex1[sample][volt] = new TLatex(-1.5,-1.3,"MEASUREMENT");
      tex1[sample][volt]->SetTextFont(132);
      texEH1[sample][volt] = new TLatex(-1.5,10.5,sssEH.str().c_str());
      texEH1[sample][volt]->SetTextFont(22);
      texEH1sam[sample][volt] = new TLatex(10,1.7,sssEHsam.str().c_str());
      texEH1sam[sample][volt]->SetTextFont(22);
    }
  }

  cout << " Showing graphs " << endl << flush;
  for (int32_t sample = 0; sample < nSamples; sample++) {
    int32_t pol = polarity[sample];
    for (int32_t volt = 0; volt < nVoltages; volt++) {
      if (nFilesIndividual[sample][volt]==0)
        continue;
      sss.str("");
      sss << "can_" << sample << "_" << voltage[pol][volt];
      cout << sss.str() << endl;
      can[sample][volt] = new TCanvas(sss.str().c_str(),sss.str().c_str(),800,600);
      dr->prettify(can[sample][volt]);
      mg[sample][volt]->Draw("AL");
      dr->prettify(mg[sample][volt]);
      mg[sample][volt]->GetXaxis()->SetTitle("Time [ns]");
      mg[sample][volt]->GetYaxis()->SetTitle("Current [#mu A]");
      mg[sample][volt]->GetYaxis()->SetRangeUser(-1.8,13.8); //[V]
      mg[sample][volt]->GetXaxis()->SetRangeUser(-2,15); //[s]
      leg[sample][volt]->Draw("same");
      tex1[sample][volt]->Draw("same");
      texEH1[sample][volt]->Draw("same");
      texEH1sam[sample][volt]->Draw("same");
      can[sample][volt]->Update();
      can[sample][volt]->WaitPrimitive();
      ss.str("");
      ss << "plots/c2voltincrease/" << sampleName[sample] << "_" << voltage[pol][volt] << ".pdf";
      can[sample][volt]->SaveAs(ss.str().c_str());
      can[sample][volt]->Update();
    }
  }


  delete dr;
  delete app;




  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
