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
#include "TParameter.h"
#include "converters/storage/storageio.h"
#include "converters/storage/event.h"
#include "converters/storage/plane.h"
#include "converters/storage/hit.h"
#include "TLatex.h"

#define DEBUG 0
#define S52 0
#define S79 1
#define S37 2
#define S52after 3
#define S79after 4

#define STAGE1 0
#define STAGE2 1
#define STAGE3 2
#define EMPTYPOINT -9999.9

using namespace std;

inline bool fileExists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {



  stringstream ss;
  float amp = 138.;
  float Qconstant = 2e-11/amp; // hence charge is in C



  //===========================================================================
  //                  LONG TERM MEASUREMENT
  //===========================================================================
  cout<<"Plotting a longterm stability.."<<endl;

  const int32_t nSamples = 5;
  const string sampleName[nSamples] = {"S37", "S79after", "S52after", "S79", "S52"};
  const string dir = "/Volumes/WINSTORAGE/Meas/Hysteresis/2015-08-03/longterm/";

  //Start graphs
  vector<vector<double>> amplitude;
  vector<vector<double>> timestamp;
  vector<vector<double>> amplitudeAvg;
  vector<vector<double>> amplitudeAvgError;
  vector<vector<double>> timestampAvg;
  vector<vector<double>> timestampAvgError;
  amplitude.resize(nSamples);
  timestamp.resize(nSamples);
  amplitudeAvg.resize(nSamples);
  amplitudeAvgError.resize(nSamples);
  timestampAvg.resize(nSamples);
  timestampAvgError.resize(nSamples);


  //open files and get the amplitudes.
  for (int32_t sample=0; sample<nSamples; sample++) {
    ss.str("");
    ss << dir << sampleName[sample] << "/" << sampleName[sample] << "_500V.root";
    string inputName = ss.str();

    unsigned int treeMask = Storage::Flags::TRACKS | Storage::Flags::CLUSTERS;
    Storage::StorageIO input(inputName.c_str(), Storage::INPUT, 0, treeMask);
    double startTS = 0;
    for (ULong64_t nevent = 0; nevent < input.getNumEvents(); nevent++) {
      Storage::Event* event = input.readEvent(nevent);
      if (!nevent) {
        startTS = event->getTimeStamp()*1e-6; //get the starting timestamp
      }
      timestamp.at(sample).push_back(event->getTimeStamp()*1e-6 - startTS);
      amplitude.at(sample).push_back(event->getPlane(0)->getHit(0)->getValue());
      // cout << " ts " << event->getTimeStamp()*1e-6
      //      << " startts " << startTS
      //      << " tscur " << event->getTimeStamp()*1e-6 - startTS
      //      << " ampl " << event->getPlane(0)->getHit(0)->getValue()
      //      << endl;
      // gr->SetPoint(nevent, event->getTimeStamp()*1e-6 - startTS, event->getPlane(0)->getHit(0)->getValue() ); //in seconds
      delete event;
    }
  }

  for (int32_t sample=0; sample<nSamples; sample++) {
    //average over ten hits
    const int32_t averaging = 50;
    int32_t avgCnt = 0;
    double amplSum = 0;
    double amplAvg = 0;
    double amplSumError = 0;
    for (int32_t i=0; i<timestamp.at(sample).size(); i++) {
      amplSum += amplitude.at(sample).at(i);
      avgCnt++;
      if (avgCnt>=averaging) {
        //mean
        amplAvg = amplSum/averaging;
        //sigma
        for (int32_t a=0; a<averaging; a++) {
          amplSumError += ( amplitude.at(sample).at(i-a) - amplAvg ) * ( amplitude.at(sample).at(i-a) - amplSum );
        }
        amplSumError /= averaging; // variance
        amplSumError = sqrt(amplSumError); //standard deviation
        // cout << " i= "<< i
        //      << " Timestmp= " << timestamp.at(i)
        //      << " AvgAmpl= " << amplAvg
        //      << " Error= " << amplSumError
        //      << endl;
        //write out
        amplitudeAvg.at(sample).push_back(amplAvg); //the average of "averaging" hits
        amplitudeAvgError.at(sample).push_back(amplSumError);
        timestampAvg.at(sample).push_back(timestamp.at(sample).at(i-averaging/2) );//averaged timestamp
        timestampAvgError.at(sample).push_back(0.0);
        amplSum = 0;
        amplSumError = 0;
        avgCnt = 0;
      }
    }
  }

  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();
  TCanvas* can = new TCanvas("can","can",800,600);
  dr->prettify(can);

  TMultiGraph* mg = new TMultiGraph();
  TGraphErrors* gr[nSamples];
  TLegend* leg = new TLegend(0.62,0.4,0.83,0.67);

  for (int32_t sample=0; sample<nSamples; sample++) {
    gr[sample] = new TGraphErrors(amplitudeAvg.at(sample).size(), &timestampAvg.at(sample)[0], &amplitudeAvg.at(sample)[0],
                                  &timestampAvgError.at(sample)[0], &amplitudeAvgError.at(sample)[0]);
    dr->prettify(gr[sample]);
    // gr[sample]->SetLineColor(dr->clrVolt[sample]);
    gr[sample]->SetMarkerStyle(24-sample);
    mg->Add(gr[sample]);
    ss.str("");

    string samName;
    if (!sampleName[sample].compare("S52after"))
      samName = "S52 primed";
    else if (!sampleName[sample].compare("S79after"))
      samName = "S79 primed";
    else
      samName = sampleName[sample];

    ss << samName;
    leg->AddEntry(gr[sample], ss.str().c_str(),  "LEP");
  }
  mg->Draw("ALP");
  dr->prettify(mg);
  mg->GetXaxis()->SetTitle("Time [s]");
  mg->GetYaxis()->SetTitle("Amplitude [V]");
  mg->GetYaxis()->SetRangeUser(0,0.7); //[V]
  mg->GetXaxis()->SetRangeUser(0,1150); //[s]
  leg->Draw("same");

  TLatex *tex1 = new TLatex(5,0.01,"MEASUREMENT");
  tex1->SetTextFont(132);
  tex1->Draw("same");


  can->Update();
  can->WaitPrimitive();
  can->Update();
  can->Clear();
  delete can;
  delete leg;


  delete dr;
  delete app;
  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
