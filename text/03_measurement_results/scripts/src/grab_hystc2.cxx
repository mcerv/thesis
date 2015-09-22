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

#define DEBUG 1
#define S52 0
#define S79 1
#define S37 2
#define S52_363e14 3
#define S79_1e14 4
#define SCDHQ 5
#define STAGE1 0
#define STAGE2 1
#define STAGE3 2
#define EMPTYPOINT -9999.9


inline bool fileExists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

using namespace std;


// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {

  cout<<"Grabbing pulse profiles and plotting a longterm C2 decrease.."<<endl;





  stringstream ss;
  TFile* f;
  double samplingTime = 0.1; //ns
  const int nSamples = 4;
  const int32_t nStages = 3;
  const int32_t nVoltages = 13;
  const int32_t nVoltagesAll = 36; //for the whole hysteresis
  const int32_t nVoltagesPerStage[3] = {12, 12, 12}; //voltage points per stage 6 12 12
  const int32_t stageN[nStages] = {1,2,3};

  const double voltage[nVoltages] =
    {-600.0, -500.0, -400.0, -300.0, -200.0, -100.0, 0.0,
      100.0, 200.0, 300.0, 400.0, 500.0, 600.0};
  const string sampleName[nSamples] = {"S52", "S79", "S52after", "S79after"};
  string sampleNameFull[nSamples] = {"S52 3.6e14 #pi cm^{-2}", "S79 1e14 #pi cm^{-2}", "S52 3.6e14 #pi cm^{-2}", "S79 1e14 #pi cm^{-2}"};
  string sampleNameState[nSamples] = {"not pumped","not pumped","pumped", "pumped"};

  //Entry possibilities: 1 electron collection, 2 hole colelction, 0 run missing
  const int include[nSamples][nStages][nVoltages] =  //FIRST: stage 0-2, SECOND: voltage 0-12.
  {
    //S52
    0,0,0,0,0,0,0,1,1,1,1,1,1,  //Stage 1 left->right -600 -> 600
    0,1,1,1,2,2,2,2,2,2,2,2,2,  //Stage 2 left->right 600 -> -600 // AFTER SWAPPING!!!!
    0,2,2,2,2,1,1,1,1,1,1,1,1,  //Stage 3 left->right -600 -> 600
    //S79
    0,0,0,0,0,0,0,1,1,1,1,1,1, //Stage 1
    0,1,1,1,2,2,2,2,2,2,2,2,2, //Stage 2
    0,2,2,2,2,1,1,1,1,1,1,1,1,  //Stage 3
    //S52after
    0,0,0,0,0,0,0,1,1,1,1,1,1, //Stage 1
    0,1,1,1,2,2,2,2,2,2,2,2,2, //Stage 2
    0,2,2,2,2,1,1,1,1,1,1,1,1,  //Stage 3
    //S79after
    0,0,0,0,0,0,0,1,1,1,1,1,1, //Stage 1
    0,1,1,1,1,1,2,2,2,2,2,2,2, //Stage 2
    0,2,2,2,2,2,1,1,1,1,1,1,1,  //Stage 3
  };

  // TApplication* app = new TApplication("app",0,0);
  // TCanvas* can = new TCanvas("c","c",800,600);


  int32_t cntFiles = 0;
  int32_t cntFilesOpen = 0;
  int32_t cntPulses = 0;
  const int32_t nFiles = 150;
  int32_t nFilesIndividual = nFiles;
  int32_t voltInv = 0;

  TProfile profvec[nSamples][nStages][nVoltages];
  bool missingprofvec[nSamples][nStages][nVoltages];


  //get the profiles and respective timestamps from the file
  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t stage=STAGE1; stage<=STAGE3; stage++) {
      for (int32_t voltInv=0; voltInv<nVoltagesPerStage[stage]; voltInv++) {
        int32_t volt;
        if (stage == STAGE2) {
          volt = nVoltagesPerStage[stage]-voltInv; //downwards on the second stage
        } else {
          volt = voltInv;
        }
        if (!include[sample][stage][volt]) {
          missingprofvec[sample][stage][volt] = true;
          continue;
        }

        ss.str("");
        ss << "/Volumes/WINSTORAGE/Meas/Hysteresis/2015-08-06/hystC2/"
           << sampleName[sample]
           << "/conv/";
        ss << sampleName[sample]
           << "_" << stage+1
           << "_" << voltage[volt]
           << "V.root";
        TProfile prof;
        cntFiles++;
        cout << "\r" << "Opening file " << ss.str() << flush;// << endl;
        f = new TFile(ss.str().c_str(),"read");
        if (!f->IsOpen()) {
          cout<<" File " << ss.str().c_str() << " not open properly!" << endl;
          missingprofvec[sample][stage][volt] = true;
          // profvec prof;
          continue;
        }
        cntFilesOpen++;
        // if (!(TProfile*)f->Get("avgpulses_filt")) {
        if (!(TProfile*)f->Get("avgpulses")) {
          cout<<" File " << ss.str().c_str() << " doesn't include avgpulses!" << endl;
          missingprofvec[sample][stage][volt] = true;
          continue;
        }
        cntPulses++;
      	prof = (*(TProfile*)f->Get("avgpulses"));
        prof.Scale(131.7);
        prof.GetXaxis()->SetRangeUser(-5,20); //nanoseconds
        ss.str("");
        ss << sampleName[sample] << "_" << stageN[stage] << "_" << volt << "V";
        prof.SetName(ss.str().c_str());
        profvec[sample][stage][volt] = prof;
        missingprofvec[sample][stage][volt] = false;

        // if (DEBUG==2) {
        //   profvec[sample][stage][volt].Draw();
        //   can->Update();
        //   can->WaitPrimitive();
        //   can->Clear();
        // }


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
  //                      FITTING
  // ------------------------------------------------------------------
  TApplication* app = new TApplication("app",0,0);
  TCanvas* can = new TCanvas("c","c",800,600);
  double maxCur[nSamples][nStages][nVoltages];
  double chi2[nSamples][nStages][nVoltages];
  double tauDecay[nSamples][nStages][nVoltages];
  for (int32_t i=0; i<nSamples; i++) {
    for (int32_t j=0; j<nStages; j++) {
      for (int32_t k=0; k<nVoltages; k++) {
        tauDecay[i][j][k] = -10; // -10 in case it's not found
        maxCur[i][j][k] = -10; //-10 in case it's not found
        chi2[i][j][k] = -10; //-10 in case it's not found
      }
    }
  }
  //create the fit function
  TF1* risefun = new TF1("rise","[4] + [5]/[6]*exp(-(x+[3])/[5])",1.5,7.5);
  TF1* decaydoublefun = new TF1("decaydouble","[0] + ([1]-[0])*exp(-(x+[3])/[2])",1.5,7.5);

  decaydoublefun->SetParName(0,"chgInf");     //charge at infinity
  decaydoublefun->SetParName(1,"chgStart");   //starting charge
  decaydoublefun->SetParName(2,"tauRec");     //recombination
  decaydoublefun->SetParName(3,"xDisp");      //displacement in x direction
  decaydoublefun->FixParameter(0,0);
  decaydoublefun->SetParameter(1,7);
  decaydoublefun->SetParameter(2,10);
  decaydoublefun->SetParameter(3,-1);
  decaydoublefun->SetParLimits(1,0,15);
  decaydoublefun->SetParLimits(2,0,400);
  decaydoublefun->SetParLimits(3,-3,2);


  //fit the function to the profiles, but only to irrad ones
  cout<<"Fitting a decay function.."<<endl;
  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t stage = 0; stage < nStages; stage++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        if (missingprofvec[sample][stage][volt]) {
          continue; //not a profile here
        }
        cout<<"\r"<<" sam "<<sample<<" stage "<<stage<<" volt "<<volt<<" "<<flush;

        //set the fit limits
        //get the max and thres of the top
        double max = (double)profvec[sample][stage][volt].GetMaximum();
        double maxBin = profvec[sample][stage][volt].GetMaximumBin();
        cout<<"max "<<max<<" "<<flush;
        double marginFall = 0 ;
        double marginRise = 0 ;
        double thresRise = 0;
        double thresFall = 0;

        marginFall = 5;
        marginRise = 28;
        thresRise = max * 0.3;// when it crosses the 10% thres on fall slope
        thresFall = max * 0.4;// when it crosses the 10% thres on fall slope

        int32_t riseEdgeBin = 0;//(int32_t)prof->GetMaximumBin();
        int32_t fallEdgeBin = 0;
        int32_t areaOfSearch = 0;
        if (max < 3) {
          areaOfSearch = 30;
        } else {
          areaOfSearch = 200;
        }

        for (int32_t bin=maxBin; bin>(maxBin-areaOfSearch); bin--) { //find the rise threshold bin
          if (profvec[sample][stage][volt].GetBinContent(bin) >= thresRise) {
            riseEdgeBin = bin;
          }
        }
        for (int32_t bin=maxBin; bin<(maxBin+areaOfSearch); bin++) { //find the fall threshold bin
          if (profvec[sample][stage][volt].GetBinContent(bin) >= thresFall) {
            fallEdgeBin = bin;
          }
        }
        int32_t pulseWidthBin = fallEdgeBin-riseEdgeBin;
        if (pulseWidthBin < 60) { //for fast narrow pulses
          marginRise-= 10;
          marginFall-= 10;
          cout<<" Very fast "<<flush;//<<endl;
        } else if (pulseWidthBin < 80) {
          marginRise-= 5;
          marginFall-= 5;
          cout<<" Fast "<<flush;//<<endl;
        }

        //transform bin numbers into [ns] - fit cut
        double fitMinBin = riseEdgeBin + marginRise;
        double fitMaxBin = fallEdgeBin - marginFall;
        double fitMin = (-500 + riseEdgeBin + marginRise)*0.1;
        double fitMax = (-500 + fallEdgeBin - marginFall)*0.1;

        //-------------------------- FIT FUNCTIONS ------------------------
        int32_t sampleNonIrrad = 0;
        if(1) {
          double area = 0;
          for (int32_t bin=fitMinBin; bin<fitMaxBin; bin++) {
            area+= profvec[sample][stage][volt].GetBinContent(bin);
          } //area in 0.1 ns * uA
          area /= 10; //to get it in ns
          cout << "fitMin "<< fitMin << " fitMax " << fitMax << " area " << area << endl;
          // tauDecayArea[sample][volt][stage] = bisection (fitMin,fitMax,maxCur,area);


          // special fit with p0 fixed
          // decaydoublefun->FixParameter(1,profvec[sampleNonIrrad][stage][volt].GetMaximum() );

          TFitResultPtr fptr = profvec[sample][stage][volt].Fit("decaydouble","RSQ", "", fitMin, fitMax);
          if (!fptr->IsEmpty()) { //if the fit is not empty, fill arrays
            tauDecay[sample][stage][volt] = fptr->Parameter(2); //tau fit
            chi2[sample][stage][volt] = decaydoublefun->GetChisquare()/decaydoublefun->GetNDF();
            cout<<" fitted Tau "<<setw(4)<<fptr->Parameter(2) << "+-" <<fptr->ParError(2)/fptr->Parameter(2)*100 <<"\%"
                <<" max "<<fptr->Parameter(1)
                <<" xDisp "<<fptr->Parameter(3)
                <<" Chi2/NDF "<<decaydoublefun->GetChisquare()/decaydoublefun->GetNDF()
                <<flush<<endl;
          }
          else {
            continue; //empty fit
          }

        } // ----------- end fitting tau --------


        //-----------------------------------------------------------------
        if (DEBUG) {
          // profvec[sampleNonIrrad][stage][volt].Draw();
          profvec[sample][stage][volt].Draw();
          can->Update();
          can->WaitPrimitive();
          can->Clear();
        }

      }
    }
  }











  // ------------------------------------------------------------------
  //                      PLOTTING
  // ------------------------------------------------------------------
  stringstream sssEH, sssEHsam, sss;

  DrawFuns* dr = new DrawFuns();
  // TCanvas* can = new TCanvas("c","c",800,600);
  TMultiGraph* mg[nSamples];
  TLegend* leg;
  // TLatex* tex1[nSamples][nstage];
  // TLatex* texEH1[nSamples][nstage];
  // TLatex* texEH1sam[nSamples][nstage];
  // int32_t clrCnt = 0;
  // int32_t numSkip = 0;
  // // Change to graphs
  // TGraph* gr[nSamples][nstage][nFiles];
  // for (int32_t sample = 0; sample < nSamples; sample++) {
  //
  //   mg[sample] = new TMultiGraph();
  //   leg[sample] = new TLegend(0.83,0.5,0.95,0.95);
  //   if (!sampleName[sample].compare("S79")) {
  //     nFilesIndividual = 150;
  //     numSkip = 15;
  //   } else if (!sampleName[sample].compare("S52after")) {
  //     nFilesIndividual = 137;
  //     numSkip = 15;
  //   } else  if (!sampleName[sample].compare("S52")) {
  //     nFilesIndividual = 44;
  //     numSkip = 4;
  //   } else {
  //     nFilesIndividual = nFiles;
  //     numSkip = 15;
  //   }
  //   for (int32_t stage = 0; stage < nstage; stage++) {
  //     clrCnt = 0;
  //     for (int32_t num = 15; num < nFilesIndividual; num+=numSkip) {
  //       if (missingprofvec[sample][stage][num])
  //         continue;
  //       ss.str("");
  //       ss << sampleName[sample] << "_" << num;
  //       gr[sample][stage][num] = new TGraph();
  //       gr[sample][stage][num].SetName(ss.str().c_str());
  //       for (int32_t i=0; i<profvec[sample][stage][num].GetNbinsX(); i++) {
  //         gr[sample][stage][num].SetPoint(
  //           i, (-500 + i)*0.1 , profvec[sample][stage][num].GetBinContent(i) );
  //       }
  //       dr->prettify(gr[sample][stage][num]);
  //       gr[sample][stage][num].SetLineColor(dr->clrstage[clrCnt]);
  //       clrCnt++;
  //       mg[sample].Add(gr[sample][stage][num]);
  //       ss.str("");
  //       ss << avgTimestamp[sample][stage][num] << " s";
  //       leg[sample].AddEntry(gr[sample][stage][num], ss.str().c_str(),  "L");
  //     }
  //     sss.str("");
  //     sssEH.str("");
  //     sssEHsam.str("");
  //     sss << sampleName[sample] << "_";
  //     sssEHsam << "#splitline{#splitline{" << sampleNameFull[sample] << "}{"
  //              << "U_{bias}=-500V}}{"
  //              << sampleNameState[sample] << "}";
  //     sssEH << "holes";
  //     sss << stageS[stage];
  //     tex1[sample][stage] = new TLatex(-1.5,-1.3,"MEASUREMENT");
  //     tex1[sample][stage].SetTextFont(132);
  //     texEH1[sample][stage] = new TLatex(-1.5,9,sssEH.str().c_str());
  //     texEH1[sample][stage].SetTextFont(22);
  //     texEH1sam[sample][stage] = new TLatex(10,1.7,sssEHsam.str().c_str());
  //     texEH1sam[sample][stage].SetTextFont(22);
  //   }
  // }
  //
  // for (int32_t sample=0; sample<nSamples; sample++) {
  //   sss.str("");
  //   sss << "can_" << sample;
  //   can[sample] = new TCanvas(sss.str().c_str(),sss.str().c_str(),800,600);
  //   dr->prettify(can[sample]);
  //   mg[sample].Draw("AL");
  //   dr->prettify(mg[sample]);
  //   mg[sample].GetXaxis()->SetTitle("Time [ns]");
  //   mg[sample].GetYaxis()->SetTitle("Current [#mu A]");
  //   mg[sample].GetYaxis()->SetRangeUser(-1.8,10.8); //[V]
  //   mg[sample].GetXaxis()->SetRangeUser(-2,15); //[s]
  //   leg[sample].Draw("same");
  //   tex1[sample][0].Draw("same");
  //   texEH1[sample][0].Draw("same");
  //   texEH1sam[sample][0].Draw("same");
  //   can[sample].Update();
  //   can[sample].WaitPrimitive();
  //   can[sample].Update();
  // }
  //
  delete can;
  delete leg;
  delete dr;
  delete app;




  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
