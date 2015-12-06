//#include <vector.h>
#include <sstream>
#include "defines.h"
#include "plots.h"
#define MAXBUFFBYTES 262000
#define HISTBYTES 150000


char* readFileIntoBuffer(string fName);
void extractHistogramDataV5(int32_t USBdatalen, char USBdata[262000], Plots plot);



// ------------------------------ MAIN PROGRAM ----------------------
void plotReactorIrrad() {


  //---------- read from files int32_to a buffer vector ------------------
  std::vector<char*> buf;
  stringstream ss;
  for (int32_t i=0; i<13; i++) {
    if (i==3) {
      buf.push_back(0);
      continue; //this run is missing
    }
    ss.str("");
    ss << "../../data/2015-03-26_reactor/";
    if (i<10)
      ss<< "00";
    else
      ss<< "0";
    ss<<i<<"_hist.bin";

    char USBdata[MAXBUFFBYTES] = readFileIntoBuffer(ss.str().c_str());
    buf.push_back(USBdata);
    cout<<"i "<<ss.str()<<endl;
  } //end of reading int32_to a vector


  //--------------------------- analysis ------------------------------
  std::vector<char*> ch;

  for (int32_t i=0; i<13; i++) {

    if (i==3) continue; //this run is missing



    //read from the buffer into histogram buffers
    Plots *plot = new Plot();
    extractHistogramDataV5(HISTBYTES, buf.at(i), plot);




  }






  //---------








}




















char* readFileint32_toBuffer(string fName) {

  char USBdata[MAXBUFFBYTES];
  streampos begin,end;
  ifstream _inRealFile;
  _inRealFile.open(fName.c_str(), ios::in | ios::app | ios::binary);
  _inRealFile.seekg (0, ios::beg);
  begin = _inRealFile.tellg();
  _inRealFile.seekg (0, ios::end);
  end = _inRealFile.tellg();
  _inRealFile.seekg (0, ios::beg);
  int32_t fileSize = end-begin;
  cout << " InRealFile size is: " << fileSize << " bytes.\n";
  _inRealFile.close();
  return USBdata;
}


void extractHistogramDataV5(int32_t USBdatalen, char USBdata[262000]) {
  //Plotter::fillHist_v5
  int32_t histfwhm[NBINS];
  int32_t histmaxampl[NBINS];
  int32_t histalphamaxampl[NBINS];
  int32_t histarea[NBINS];
  int32_t histalphaarea[NBINS];
  double histfwhm_v4[NBINS];
  double histmaxampl_v4[NBINS];
  double histarea_v4[NBINS];
  double histalphaarea_v4[NBINS];
  double exthistarea_v5[NBINS_EXTAREA];
  double exthistalphaarea_v5[NBINS_EXTAREA];
  int32_t  bin = 0;
  for (int32_t i = 0; i < 256; i++) {
    histfwhm[i]=0;
    histmaxampl[i]=0;
    histalphamaxampl[i]=0;
    histarea[i]=0;
    histalphaarea[i]=0;
    histfwhm_v4[i]=0;
    histmaxampl_v4[i]=0;
    histarea_v4[i]=0;
    histalphaarea_v4[i]=0;
  }
  for (int32_t i = 0; i < NBINS_EXTAREA; i++) {
    exthistarea_v5[i]=0;
    exthistalphaarea_v5[i]=0;
  }


  for (int32_t i = 0; i < USBdatalen-32; i = i + 32) {//read in 256 bits at a time.
    //256bit = 32 * 8bit
    //find header(64 bit 0xAA)
    int32_t headcnt = 0;
    for (int32_t j = 0; j < 2; j++) {
      if (USBdata[i+j] == 0xAA)
        headcnt++;
      else
        headcnt = 0;
    }
    if (headcnt != 2) {
      continue; // jump to the next 64 bit
    }
    //if it's header, fill the histograms with the next values.
    //first read the bin number. 16bit.
    bin = (int32_t)(USBdata[i+2] << 8) |
          (int32_t)(USBdata[i+3]);
    //bin value is actually one less - we get too high of a value from Fpga
    bin -= 1;

    //then get the values.
    //but not for bin 0, because it is corrupted.
    //if (bin==0) continue;

    exthistarea_v5[bin] = (int32_t)(USBdata[i+4+27] << 0 ) |
                          (int32_t)(USBdata[i+4+26] << 8 ) |
                          (int32_t)(USBdata[i+4+25] << 16) |
                          (int32_t)(USBdata[i+4+24] << 24);
    exthistalphaarea_v5[bin] =  (int32_t)(USBdata[i+4+23] << 0 ) |
                                (int32_t)(USBdata[i+4+22] << 8 ) |
                                (int32_t)(USBdata[i+4+21] << 16) |
                                (int32_t)(USBdata[i+4+20] << 24);

      // cout<<" bin "<<bin
      //   <<" extarea "<<exthistarea_v5[bin]
      //   <<" extalar "<<exthistalphaarea_v5[bin];
      //


    if (bin>255) continue;
    histfwhm[bin] = (int32_t)(USBdata[i+4+3] << 0 ) |
                    (int32_t)(USBdata[i+4+2] << 8 ) |
                    (int32_t)(USBdata[i+4+1] << 16) |
                    (int32_t)(USBdata[i+4+0] << 24);
    histmaxampl[bin] =  (int32_t)(USBdata[i+4+7] << 0 ) |
                        (int32_t)(USBdata[i+4+6] << 8 ) |
                        (int32_t)(USBdata[i+4+5] << 16) |
                        (int32_t)(USBdata[i+4+4] << 24);
    histalphamaxampl[bin] =   (int32_t)(USBdata[i+4+11] << 0 ) |
                              (int32_t)(USBdata[i+4+10] << 8 ) |
                              (int32_t)(USBdata[i+4+9 ] << 16) |
                              (int32_t)(USBdata[i+4+8 ] << 24);
    histarea[bin] = (int32_t)(USBdata[i+4+15] << 0 ) |
                    (int32_t)(USBdata[i+4+14] << 8 ) |
                    (int32_t)(USBdata[i+4+13] << 16) |
                    (int32_t)(USBdata[i+4+12] << 24);
    histalphaarea[bin] =  (int32_t)(USBdata[i+4+19] << 0 ) |
                          (int32_t)(USBdata[i+4+18] << 8 ) |
                          (int32_t)(USBdata[i+4+17] << 16) |
                          (int32_t)(USBdata[i+4+16] << 24);
    //
    // cout<<" fwhm "<<histfwhm[bin]
    //     <<" mamp "<<histmaxampl[bin]
    //     <<" area "<<histarea[bin]
    //     <<" alar "<<histalphaarea[bin]
    //    <<endl;
  }

}
