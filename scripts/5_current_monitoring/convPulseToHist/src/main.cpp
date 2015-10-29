#include "defines.h"
#include "pulseana.h"
#include <string.h>
#include <sstream>
// #include <fstream>
// #include "TApplication.h"
// #include "TCanvas.h"
//#include "pulsefilereader.h"
//#include "plotter.h"
#define MAXBUFFBYTES 2e31-1
#define HISTBYTES 150000
using namespace std;

// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {

  PulseAna* pana = new PulseAna();


  //---------- read from files int32_to a buffer vector ------------------
  stringstream ss, sshist;
  for (int32_t i=0; i<1; i++) {
    ss.str("");
    sshist.str("");
    // ss << "../../data/";
    // sshist << "../../data/";
    // ss << "../../../../../../Documents/MarieCurieTALENT/Software/PSA/data/2015-08-29_lab/";
    // sshist << "../../../../../../Documents/MarieCurieTALENT/Software/PSA/data/2015-08-29_lab/";
    ss << "/Volumes/WINSTORAGE/Meas/PSAcalib/Harsh/";
    sshist << "/Volumes/WINSTORAGE/Meas/PSAcalib/Harsh/";
    if (i<10) {
      ss<< "30";
      sshist<< "30";
    }
    else{
      ss<< "0";
      sshist<< "0";
    }
    ss<<i<<"_pulse.bin";
    sshist<<i<<"_hist.bin";
    cout<<"Starting..."<<endl;

    pana->readPulseFile(ss.str(),"Cx");
    pana->analyse();
    cout<<ss.str()<<", "<<sshist.str()<< endl;
    // pana->showPulses();
    // pana->plot();
    // pana->plotCx();
    pana->writeToHistFile(sshist.str());
  } //end of reading int32_to a vector

  pana->clear();
  delete pana;
  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
