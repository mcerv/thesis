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


#define DEBUG 0
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
  // cout<<" Current dir: "<<gDirectory->pwd()<<endl;


  //===========================================================================
  //                  HYSTERESIS
  //===========================================================================
  cout<<"Plotting a hysteresis.."<<endl;
  //open Files
  //get the amplitudes
  const int32_t nSamples = 4;
  const int32_t nStages = 3;
  const int32_t nVoltages = 25;
  const int32_t nVoltagesAll = 60; //for the whole hysteresis
  const int32_t nVoltagesPerStage[3] = {12, 24, 24}; //voltage points per stage
  const int32_t stageN[nStages] = {1,2,3};
  const double voltage[nVoltages] =
    {-600.0, -550.0, -500.0, -450.0, -400.0, -350.0, -300.0, -250.0, -200.0, -150.0, -100.0, -50.0, 0.0,
      50.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0};
  const string sampleName[nSamples] = {"S52", "S37", "S79after", "S52after"};

  vector<vector<double>> voltages;
  vector<vector<double>> missingPoints;
  vector<vector<vector<double>>> ampls;
  vector<vector<double>> avgAmpl;
  vector<vector<double>> avgAmplError;
  vector<vector<double>> avgVolt;
  vector<vector<double>> avgVoltError;
  voltages.resize(nSamples);
  missingPoints.resize(nSamples);
  ampls.resize(nSamples);
  avgAmpl.resize(nSamples);
  avgAmplError.resize(nSamples);
  avgVolt.resize(nSamples);
  avgVoltError.resize(nSamples);

  for (int32_t sample=0; sample<nSamples; sample++) {
    ampls.at(sample).resize(nVoltagesAll);
    for (int32_t i=13; i<nVoltages; i++) {
      voltages.at(sample).push_back(voltage[i]); //STAGE1
    }
    for (int32_t i=nVoltages-2; i>=0; i--) {
      voltages.at(sample).push_back(voltage[i]); //STAGE2
    }
    for (int32_t i=1; i<nVoltages; i++) {
      voltages.at(sample).push_back(voltage[i]); //STAGE3
    }
  }

  // open root files
  unsigned int treeMask1 = Storage::Flags::TRACKS | Storage::Flags::CLUSTERS;
  string workdir = "/Volumes/WINSTORAGE/Meas/Hysteresis/2015-08-03/hyst/";
  string rootFileName = "";

  Storage::StorageIO* input;
  for (int32_t sample=0; sample<nSamples; sample++) {
    int32_t voltAllCnt = 0;
    for (int32_t stage=STAGE1; stage<=STAGE3; stage++) {
      for (int32_t volt=0; volt<nVoltagesPerStage[stage]; volt++) {
        ss.str("");
        ss << workdir << sampleName[sample] << "/conv/" << stageN[stage] << "/";
        ss << voltages.at(sample).at(volt + voltAllCnt) << "V.root";
        cout << "Opening file " << ss.str() << "...";
        if (!fileExists(ss.str())) {
          cout << "FILE NOT OPENED PROPERLY!" << endl;
          ampls.at(sample).at(volt + voltAllCnt).push_back(EMPTYPOINT); //fake amplitude
          continue;
        }
        cout << "OK. ";
        input = new Storage::StorageIO(ss.str().c_str(), Storage::INPUT, 0, treeMask1);
        if (!input->getNumEvents()) {
          cout << "EMPTY FILE!" << endl;
          ampls.at(sample).at(volt + voltAllCnt).push_back(EMPTYPOINT); //fake amplitude
          continue;
        }

        for (ULong64_t nevent = 0; nevent < input->getNumEvents(); nevent++) {
          Storage::Event* event = input->readEvent(nevent);
          ampls.at(sample).at(volt + voltAllCnt).push_back(event->getPlane(0)->getHit(0)->getValue());
          delete event;
        }
        cout << input->getNumEvents() << " events.";
        cout << endl;
        delete input;
      }
      voltAllCnt += nVoltagesPerStage[stage];
    }
  }
  cout << endl;

  // averaging an error estimation
  for (int32_t sample=0; sample<nSamples; sample++) {
    for (int32_t volt=0; volt<voltages.at(sample).size(); volt++) {
      double amplErrorSum = 0;
      if (ampls.at(sample).at(volt).at(0) == EMPTYPOINT)
        continue;
      double sum = 0;
      for (int32_t i=0; i<ampls.at(sample).at(volt).size(); i++) { //sumation for mean
        sum += ampls.at(sample).at(volt).at(i);
      }
      double mean = sum/ampls.at(sample).at(volt).size();
      for (int32_t i=0; i<ampls.at(sample).at(volt).size(); i++) { //sumation for sigma
        amplErrorSum += ( ampls.at(sample).at(volt).at(i) - mean ) * ( ampls.at(sample).at(volt).at(i) - mean );
      }
      amplErrorSum /= ampls.at(sample).at(volt).size(); // variance
      amplErrorSum = sqrt(amplErrorSum); //standard deviation
      if (!sampleName[sample].compare("S37") ) { //swap polarity if needed
        if (volt >= 23 && volt <= 47) mean = -mean;
      } else if (!sampleName[sample].compare("S52") ) { //swap polarity if needed
        if (volt >= 15 && volt <= 39) mean = -mean;
      } else if (!sampleName[sample].compare("S52after") ) { //swap polarity if needed
        if (volt >= 15 && volt <= 40) mean = -mean;
      } else if (!sampleName[sample].compare("S79") ) { //swap polarity if needed
        if (volt >= 15 && volt <= 39) mean = -mean;
      } else if (!sampleName[sample].compare("S79after") ) { //swap polarity if needed
        if (volt >= 20 && volt <= 45) mean = -mean;
      }
      avgAmpl.at(sample).push_back(mean); //mean of the amplitudes
      avgAmplError.at(sample).push_back(amplErrorSum);  // sigma of the amplitudes
      avgVolt.at(sample).push_back(voltages.at(sample).at(volt)); //getting rid of the empty voltages
      avgVoltError.at(sample).push_back(0.01*voltages.at(sample).at(volt));
    }
  }


  TApplication* app = new TApplication("a",NULL,NULL);
  DrawFuns* dr = new DrawFuns();
  TCanvas* can = new TCanvas("c1","c1",800,600);
  dr->prettify(can);
  TMultiGraph* mg = new TMultiGraph();
  TGraphErrors* gr[nSamples];
  TLegend* leg = new TLegend(0.2,0.7,0.4,0.95);
  const int32_t dropFirst = 11;
  for (int32_t sample=0; sample<nSamples; sample++) {
    gr[sample]= new TGraphErrors(avgAmpl.at(sample).size()-dropFirst, &avgVolt.at(sample)[dropFirst], &avgAmpl.at(sample)[dropFirst],
                                 &avgVoltError.at(sample)[dropFirst], &avgAmplError.at(sample)[dropFirst]);
    dr->prettify(gr[sample]);
    gr[sample]->SetLineColor(dr->clrVolt[sample]);
    mg->Add(gr[sample]);
    leg->AddEntry(gr[sample],sampleName[sample].c_str(),"L");
  }
  mg->Draw("ALP");
  dr->prettify(mg);
  mg->GetXaxis()->SetTitle("Bias voltage [V]");
  mg->GetYaxis()->SetTitle("Amplitude [V]");
  mg->GetYaxis()->SetRangeUser(-0.5,0.7); //[V]
  mg->GetXaxis()->SetRangeUser(-700,700); //[V]
  leg->Draw("same");
  can->Modified();
  can->Update();
  can->WaitPrimitive();
  can->Update();
  can->Clear();
  delete can;


  delete dr;
  delete app;






  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
