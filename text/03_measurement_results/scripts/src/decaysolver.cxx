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
#include <math.h>

#define DEBUG 2
#define S52 0
#define S79 1
#define S37 2
#define S52_363e14 3
#define S79_1e14 4




using namespace std;

double bisection (double t0, double t1, double current0, double area0) {
  const int32_t maxIterations = 100;
  const double minError = 1e-4;
  double rangeMin = 0.1;
  double rangeMax = 400;
  TF1* decayarea = new TF1 ("decayarea", "[0] - [1]*( -x*exp(-[3]/x) + x*exp(-[2]/x) )", 0, 400);
  decayarea->SetParameter(0,area0); //pulse area [uA * ns]
  decayarea->SetParameter(1,current0); //initial charge / current [uA]
  decayarea->SetParameter(2,t0); //time 0 [ns]
  decayarea->SetParameter(3,t1); //time 1 [ns]

  double tau = 0; //starting point of Tau
  double y = 0;  //has to be minimised towards 0
  int32_t iteration=0;
  for (iteration=0; iteration<maxIterations; iteration++) {
    tau = (rangeMax+rangeMin)/2;
    y = decayarea->Eval(tau);
    if (y==0.0) {
      break; //we found the exact tau;
    } else if (y > 0.0) {
      rangeMin = tau;
    } else if (y < 0.0) {
      rangeMax = tau;
    }
    if (abs(y) < minError) {
      break;
    }
  }

  cout<<" Bisection results: Tau="<<tau<<" y="<<y<<" nIterations="<<iteration<<endl;
  if (DEBUG == 2) {
    TCanvas* can = new TCanvas("can","can",800,600);
    decayarea->Draw();
    can->Update();
    can->WaitPrimitive();
    can->Update();
    can->Clear();
    delete can;
  }
  delete decayarea;
  return tau;
}


// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {
  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();

  //set global variables and objects
  stringstream ss;
  TFile* f = new TFile ("plots/plots.root","read");// = new TFile();
  if (!f->IsOpen()) {
    cout<<" File " << ss.str().c_str() << " not open properly!" << endl;
    return 0;
  }
  double samplingTime = 0.1; //ns
  const int nSamples = 5;
  const int nVoltages = 4;
  const int nTemp = 18;

  double voltage[nVoltages] = {500.0, 400.0, -400.0, -500.0};
  string sampleName[nSamples] = {"S52","S79","S37","S52_3-63e14","S79_1e14" };
  string tempS[nTemp] = {"4k","7k","10k","15k","25k","40k","60k","75k","95k",
              "115k","135k","160k","180k","210k","230k","260k","280k","295k"};
  double tempN[nTemp] = {4,7,10,15,25,40,60,75,95,115,135,160,180,210,230,260,280,295};
  // vector< vector< vector <TProfile> > > prof;
  TProfile* prof[nSamples][nTemp][nVoltages];
  bool missingprof[nSamples][nTemp][nVoltages];

  //get the profiles from the file
  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        ss.str("");
        ss << "Sample_" << sampleName[sample] << "/" << sampleName[sample]
           << "_" << tempS[temp] << "_" << voltage[volt];
        cout<<"asdf"<<endl;
        if (!(TProfile*)f->Get(ss.str().c_str() ) ) {
          cout<<" File " << ss.str().c_str() << " doesn't include " << ss.str() << endl;
          prof[sample][temp][volt] = new TProfile();
          missingprof[sample][temp][volt] = true;
          continue;
        }
        cout<<"asdf"<<endl;
      	prof[sample][temp][volt] = ((TProfile*)f->Get(ss.str().c_str() ));
        missingprof[sample][temp][volt] = false;
      }
    }
  }


  //perform bisection on all
  for (int32_t sample = S52_363e14; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        if (missingprof[sample][temp][volt]) {
          continue; //not a profile here
        }

        //set the fit limits
        //get the max and thres of the top
        double max = (double)prof[sample][temp][volt]->GetMaximum();
        double maxBin = prof[sample][temp][volt]->GetMaximumBin();
        double marginFall = 0 ;
        double marginRise = 0 ;
        double thresRise = 0;
        double thresFall = 0;

        if (temp <= 7) { //7 -> 75 K
          marginFall = 25;
          marginRise = 30;
          thresRise = max * 0.1; //because the tail is longer, 20%
          thresFall = max * 0.25;
        } else if (8 <= temp && temp < 11) { //between 75k and (excl)160k
          marginFall = 20;
          marginRise = 35;
          thresRise = max * 0.2;// when it crosses the 30% thres on fall slope
          thresFall = max * 0.4;// when it crosses the 30% thres on fall slope
        } else {
          marginFall = 23;
          marginRise = 25;
          thresRise = max * 0.1;// when it crosses the 10% thres on fall slope
          thresFall = max * 0.1;// when it crosses the 10% thres on fall slope
        }
        int32_t riseEdgeBin = 0;//(int32_t)prof->GetMaximumBin();
        int32_t fallEdgeBin = 0;
        for (int32_t bin=maxBin; bin>(maxBin-200); bin--) {
          if (prof[sample][temp][volt]->GetBinContent(bin) >= thresRise) {
            riseEdgeBin = bin;
          }
        }
        for (int32_t bin=maxBin; bin<(maxBin+200); bin++) {
          if (prof[sample][temp][volt]->GetBinContent(bin) >= thresFall) {
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


        int32_t sampleNonIrrad = 0;
        if(1) {
          //fix the space charge of the non-irrad
          sampleNonIrrad=sample;   //-----** THIS NEEDS TO BE CHANGED BY HAND!!!!!! TODO
          if ( sample==S79_1e14) {
            sampleNonIrrad = S79;
          }
          if ( sample==S52_363e14 ) {
            sampleNonIrrad = S52;
          }

          //set the max current/charge
          double maxCur = prof[sampleNonIrrad][temp][volt]->GetMaximum() ;
          //get the area from binMin to binMax
          double area = 0;
          for (int32_t bin=fitMinBin; bin<fitMaxBin; bin++) {
            area+= prof[sample][temp][volt]->GetBinContent(bin);
          } //area in 0.1 ns * uA
          area /= 10; //to get it in ns
          cout << "fitMin "<< fitMin << " fitMax " << fitMax << " maxCur "
               << maxCur << " area " << area << endl;
          bisection (fitMin,fitMax,maxCur,area);


          // cout<<" fitted Tau "<<fptr->Parameter(2)
          //     <<" max "<<fptr->Parameter(1)
          //     <<" xDisp "<<fptr->Parameter(3)
          //     <<" Chi2 "<<decaydoublefun->GetChisquare()
          //     <<flush<<endl;
          // }
          // else {
          //   continue; //empty fit
          // }

        } // ----------- end fitting tau --------

        //-----------------------------------------------------------------
        if (DEBUG) {
          TCanvas* can = new TCanvas("double","double",800,600);
          prof[sampleNonIrrad][temp][volt]->Draw();
          prof[sample][temp][volt]->Draw("same");
          can->Update();
          can->WaitPrimitive();
          can->Update();
          can->Clear();
          delete can;
        }

      }
    }
  }













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
