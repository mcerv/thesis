/*
  This class only used for testing

*/

#ifndef __PULSEFILEREADER_H_DEFINED__
#define __PULSEFILEREADER_H_DEFINED__

#include "progbar.h"
#include "plotter.h"
#include "defines.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


class PulseFileReader {

public:
  PulseFileReader(
    string      //datalist file name
  );
  PulseFileReader();
  ~PulseFileReader();

  void readList();
  void printList();

  void readPulseFile(   //read one pulse in UNSIGNED vector (ps I/O requirements)
    int             //file number
    );
  void readSelectivePulseFile(   //read ONLY 156 BYTES directly into sample8 vec
    int             //file number
    );

  void printPulseFile(
    int             //file number
    );

  void printSelectivePulseFile(
    int             //file number
    );

  void batchSelectiveReadPulseFiles(
    int,           //starting file number
    int            //end file number
    );
  void batchConvert(); //convert all files in buffer to 156-byte-long files

  vector <vector <uint8_t> > _batchPulses; //vector of 8bit 156byte long pulses

  // real pulses
  void writeRealPulsesToFile(uint8_t* , int32_t );
  void openRealPulsesFile(string, string);
  void closeRealPulsesFile();
  void readWaveforms();
  void readHistograms();




private:
  void convertPulseFile(  //write the new pulse into a file
    int                 //pulse number
    );
  ProgBar *_pb;
  string _listFileName;
  vector <string> _pulseFiles;
  vector <uint16_t> _pulse16;
  vector <uint8_t> _pulse8;
  vector <uint8_t> _pulse8sel;  //selective 8bit pulse (only 156 bytes)
  int32_t _startPulse;
  int32_t _endPulse;


  //real pulses
  ofstream _outRealFile;
  ifstream _inRealFile;

};




#endif
