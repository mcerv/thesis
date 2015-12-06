#include "defines.h"
#include "plots.h"
#include <string.h>
#include <sstream>
#include "histana.h"
// #include <fstream>
// #include "TApplication.h"
// #include "TCanvas.h"
//#include "pulsefilereader.h"
//#include "plotter.h"
// #define MAXBUFFBYTES 33000
#define HISTBYTES 150000
using namespace std;

// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {

  HistAna *histAna = new HistAna();

  //---------- read from files int32_to a buffer vector ------------------
  vector<vector<uint8_t>> buf;
  stringstream ss;
  int32_t num = 30;

  for (int32_t i=num; i<num+1; i++) {
    ss.str("");
    // ss << "../../../../../../Documents/MarieCurieTALENT/Software/PSA/data/2015-08-29_lab/";
    ss << "/Volumes/WINSTORAGE/Meas/PSAcalib/Harsh/";
    if (i<10)
      ss<< "00";
    else
      ss<< "0";
    ss<<i<<"_hist.bin";

    //uint8_t *USBdata = new uint8_t[MAXBUFFBYTES];
    vector<uint8_t> USBdata;
    // if (i==4) {
    //   buf.push_back(USBdata);
    //   continue; //this run is missing
    // }

    histAna->readFileIntoBuffer(ss.str().c_str(), USBdata);
    buf.push_back(USBdata);
    USBdata.clear();
    cout<<ss.str()<<endl;
  } //end of reading int32_to a vector


  //--------------- write data into histograms ------------------------------
  string version = "";
  //std::vector<Plots> vecPlots;
  for (int32_t i=0; i<1; i++) {
    version = "6";
    //read from the buffer into histogram buffers
    Plots plot (i);// = new Plots();
    // if (i==4) {
    //   histAna->addEntry(&plot);
    //   continue; //this run is missing
    // }
    histAna->extractHistogramData(buf.at(i), &plot, version);
    histAna->addEntry(&plot);
  }
  buf.clear();
  //--------------------------- analysis ------------------------------
  histAna->plotLab();
  histAna->saveTxt();
  // histAna->plotSpectrum();

  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
