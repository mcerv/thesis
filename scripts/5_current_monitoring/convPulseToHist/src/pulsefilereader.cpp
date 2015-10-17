#include "pulsefilereader.h"


PulseFileReader::~PulseFileReader ()
{
  _batchPulses.clear();
  _pulse8sel.clear();
  _pulse16.clear();
  _pulse8.clear();
}

PulseFileReader::PulseFileReader(string listname) :
  _listFileName(listname),
  _startPulse(PSA_PARSER_PULSE_START),
  _endPulse(PSA_PARSER_PULSE_START+PSA_ARRAY_LEN-1)
{
  readList();
    cout<<"pulse reader "<<endl;

  batchSelectiveReadPulseFiles(0,_pulseFiles.size());

}

PulseFileReader::PulseFileReader() :
  _startPulse(PSA_PARSER_PULSE_START),
  _endPulse(PSA_PARSER_PULSE_START+PSA_ARRAY_LEN)
{
}


void PulseFileReader::batchSelectiveReadPulseFiles(
    int startFiles,
    int endFiles)
{
  _pb = new ProgBar("Reading the pulses into memory...");
  _batchPulses.clear();
  for (int i = startFiles; i < endFiles; i++)
  {
    readSelectivePulseFile(i);
    _batchPulses.push_back(_pulse8sel);

    //_pb->show(i,endFiles);
  }
  cout<<" N. of pulses loaded into memory: "<<_batchPulses.size()<<endl;
}


void PulseFileReader::printSelectivePulseFile(int npulse)
{
  cout<<setw(20)<<"pulse 8bit selective"<<endl;
  for (int i = 0; i < _pulse8sel.size(); i++)
  //for (int i = 0; i < 50; i++)
  {
    cout<<setw(20)<<i<<setw(20)<<(int)_pulse8sel.at(i)<<endl;
  }
}


void PulseFileReader::printPulseFile(int npulse)
{
  cout<<setw(20)<<"pulse 16bit"<<setw(20)<<"pulse 8bit"<<endl;
  //for (int i = 0; i < _pulse16.size(); i++)
  for (int i = 0; i < 50; i++)
  {
    cout<<setw(20)<<_pulse16.at(i)<<setw(20)<<(int)_pulse8.at(i)<<endl;
  }
}

void PulseFileReader::batchConvert()
{
  //prepare the progress bar
  _pb = new ProgBar("Converting and writing into files...");
  for (int i = 0; i < _batchPulses.size(); i++)
  {
    convertPulseFile(i);
    _pb->show(i,_batchPulses.size());
  }
}

void PulseFileReader::convertPulseFile(int npulse)
{ //this procedure is only used to check how the pulse files look like
  //create file name
  std::ostringstream ss;
  ss << npulse;
  string fName = "data/shortPulses/pulse"+ss.str()+".txt";


  //upload all pulse into a stringstream
  std::stringstream ssout;
  for (int i=0;i<PSA_ARRAY_LEN-1;i++)
  {
    ssout << (int)_batchPulses.at(npulse).at(i) << ",";
  }
  ssout << (int)_batchPulses.at(npulse).at(PSA_ARRAY_LEN-1) << endl;

  //open out file
  ofstream outFile;
  outFile.open(fName.c_str());
  if (!outFile.is_open())
    throw " PulseFileReader::writeSelectivePulseFile Can't open pulse file!";

  //write to out file
  outFile << ssout.rdbuf();

  outFile.close();
}

void PulseFileReader::openRealPulsesFile(string fName, string direction) {
  //open out real pulse file
  cout<<" PulseFileReader: opening data file "<<fName<<endl;
  if (!direction.compare("WRITE")) {
    _outRealFile.open(fName.c_str(), ios::out | ios::app | ios::binary);
    //_inRealFile = 0;
    if (!_outRealFile.is_open())
      throw " PulseFileReader::openRealPulseFile Can't open real pulse file!";
  }
  else if (!direction.compare("READ")) {
    _inRealFile.open(fName.c_str(), ios::in | ios::app | ios::binary);
    //_outRealFile = 0;
    if (!_inRealFile.is_open())
      throw " PulseFileReader::openRealPulseFile Can't open real pulse file!";
  }



}

void PulseFileReader::closeRealPulsesFile() {
  if (_outRealFile) _outRealFile.close();
  if (_inRealFile) _inRealFile.close();
  cout<<" PulseFileReader: real pulse file closed."<<endl;
}


void PulseFileReader::readWaveforms()
{
  Plotter *plotter = new Plotter();
  streampos begin,end;
  _inRealFile.seekg (0, ios::beg);
  begin = _inRealFile.tellg();
  _inRealFile.seekg (0, ios::end);
  end = _inRealFile.tellg();
  _inRealFile.seekg (0, ios::beg);
  int32_t fileSize = end-begin;
  cout << " InRealFile size is: " << fileSize << " bytes.\n";

    uint8_t *USBdata;
    USBdata = (uint8_t *) malloc(fileSize * sizeof(uint8_t));

  uint8_t buf8;
  char chbuf8;
  int32_t i=0;
  int32_t byte = 0;
  int32_t cnt4 = 0;
  //read the file into buffer
  while (1)
  {
    //bool allZeros = true;
    //for (int32_t j = 0; j < 32; j++)
    //  if (USBdata[i+j]) allZeros = false;
    //if (allZeros)
    // continue;
    if (i >= (end-begin)-32 )
      break;
    //cout<<i<<" bytes"<<endl;
    for (int32_t j = 0; j < 32; j++) {
      _inRealFile.read(&chbuf8,1);
      buf8 = (uint8_t)chbuf8;
      USBdata[byte] = buf8;

      byte++;
      //cnt4++;
      //if (cnt4==4) cnt4 = 0;
      //cout<<hex<<(uint32_t)buf8<<" ";
    }
     cout<<dec<<endl;
    i+=32;
  }
  cout<<" Read in "<<i<<" bytes of "<<fileSize<<"."<<endl;

  //plotter->fillWaveform(fileSize, USBdata);
  // plotter->setMonitorValues( (int32_t)(50/SAMPLING_RATE),
  //                                 100,
  //                                 100,
  //                                 (float)2.0,
  //                                 (float)2.0,
  //                                 100,    //mV
  //                                 4
  //                                 );
  // plotter->analyseWaveform(i,USBdata);



}




void PulseFileReader::readHistograms()
{
  Plotter *plotter = new Plotter();
  streampos begin,end;
  _inRealFile.seekg (0, ios::beg);
  begin = _inRealFile.tellg();
  _inRealFile.seekg (0, ios::end);
  end = _inRealFile.tellg();
  _inRealFile.seekg (0, ios::beg);
  int32_t fileSize = end-begin;
  cout << " InRealFile size is: " << fileSize << " bytes.\n";

  //  uint8_t *USBdata;
  //  USBdata = (uint8_t *) malloc(fileSize * sizeof(uint8_t));
  uint8_t USBdata[262000];
  uint8_t buf8;
  char chbuf8;
  int32_t i=0;
  int32_t byte = 0;
  int32_t cnt4 = 0;
  //read the file into buffer
  while (1)
  {
    //bool allZeros = true;
    //for (int32_t j = 0; j < 32; j++)
    //  if (USBdata[i+j]) allZeros = false;
    //if (allZeros)
    // continue;
    if (i >= (end-begin)-32 )
      break;
    //cout<<i<<" bytes"<<endl;
    for (int32_t j = 0; j < 32; j++) {
      _inRealFile.read(&chbuf8,1);
      buf8 = (uint8_t)chbuf8;
      USBdata[byte] = buf8;
      byte++;
    }
    cout<<dec;//<<endl;
    i+=32;
  }
  cout<<" Read in "<<i<<" bytes of "<<fileSize<<"."<<endl;

  //plotter->fillWaveform(fileSize, USBdata);
  // plotter->setMonitorValues( (int32_t)(8/SAMPLING_RATE),
  //                                 100,
  //                                 100,
  //                                 (float)2.0,
  //                                 (float)2.0,
  //                                 100,    //mV
  //                                 4
  //                                 );
  //plotter->analyseWaveform(i,USBdata);
  // plotter->fillHist_v5(i,USBdata);


  delete(plotter);
}















void PulseFileReader::writeRealPulsesToFile(uint8_t* USBdata, int32_t USBdatalen) {
  _outRealFile.write( (char*)USBdata, USBdatalen );
}

void PulseFileReader::readSelectivePulseFile(int npulse)
{

  _pulse8sel.clear(); // clear the 8bit pulse vector
  ifstream infile( _pulseFiles.at(npulse).c_str() ); //open a new pulse file

  while (infile) // this while goes through only once.
  {
    string s;
    if (!getline( infile, s )) break;
    istringstream ss( s ); //take the line into stringstream

    int nSample = 0;
    while (ss)
    {
      uint16_t sample16;
      uint8_t sample8;
      if (!getline( ss, s, ',' )) break;      //read a string delimited by ,

      //only take 156 samples out!
      if ((nSample >= _startPulse) & (nSample <= _endPulse))
      {
        sample16 = (uint16_t)atoi(s.c_str());   //change string into 16bit int
        sample8 = (uint8_t)(sample16>>8); //16bit shift left for 8 into 8bit
        _pulse8sel.push_back( sample8);            //push back to 8bit vector
      }




      nSample++;
    }
  }
  if (!infile.eof())
  {
    cerr << "Fooey!\n";
  }
}







void PulseFileReader::readPulseFile(int npulse)
{
  _pulse16.clear(); // clear the 16bit pulse vector
  _pulse8.clear(); // clear the 8bit pulse vector
  ifstream infile( _pulseFiles.at(npulse).c_str() ); //open a new pulse file
  if (!infile.is_open())
    throw " PulseFileReader::readList Can't open pulse file!";

  while (infile) // this while goes through only once.
  {
    string s;
    if (!getline( infile, s )) break;
    istringstream ss( s ); //take the line into stringstream


    while (ss)
    {
      uint16_t sample16;
      uint8_t sample8;
      if (!getline( ss, s, ',' )) break;      //read a string delimited by ,
      sample16 = (uint16_t)atoi(s.c_str());   //change string into 16bit int
      _pulse16.push_back( sample16 );         //push back to 16bit vector

      sample8 = (uint8_t)(sample16>>8); //16bit shift left for 8 into 8bit
      _pulse8.push_back( sample8);            //push back to 8bit vector
    }
  }
  if (!infile.eof())
  {
    cerr << "Fooey!\n";
  }
  infile.close();
}



void PulseFileReader::printList()
{
  for (int i = 0; i < _pulseFiles.size(); i++)
  {
    cout<<_pulseFiles.at(i)<<endl;
  }
}

void PulseFileReader::readList()
{

  ifstream infile( _listFileName.c_str() ); //open the file with listed pulse files
  if (!infile.is_open())
    throw " PulseFileReader::readList Can't open list file!";

  while (infile)
  {
    string s;
    if (!getline( infile, s )) break; //read a line
    _pulseFiles.push_back( s );       //save it into _pulseFiles vector
  }
  if (!infile.eof())
  {
    cerr << "Fooey!\n";
  }
  infile.close();
}
