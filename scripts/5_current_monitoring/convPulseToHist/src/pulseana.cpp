#include "pulseana.h"


PulseAna::PulseAna() {
  m_vp.clear();
  for (int32_t i=0; i<NBINS; i++){
    m_histfwhm[i]=0;
    m_histmaxampl[i]=0;
    m_histalphamaxampl[i]=0;
  }
  for (int32_t i=0; i<NBINS_FWHM_SLOPE; i++){
    m_exthistfwhmslope[i]=0;
  }
  for (int32_t i=0; i<NBINS; i++){
    m_exthistarea[i]=0;
    m_exthistalphaarea[i]=0;
  }
}

PulseAna::~PulseAna() {
  m_vp.clear();
}

void PulseAna::clear() {
  m_vp.clear();
}

void PulseAna::readPulseFile(string fName, string version) {
  streampos begin,end;
  ifstream _inRealFile;
  _inRealFile.open(fName.c_str(), ios::in | ios::ate | ios::binary);
  if (!_inRealFile.is_open())
    throw " PulseAna::readPulseFile : wrong file name. File not open.";
  // fileSize = _inRealFile.tellg();
  const int32_t size = (int32_t)_inRealFile.tellg() /10;
  _inRealFile.seekg (0, ios::beg);
  begin = _inRealFile.tellg();
  cout<<" File size is "<<size<<" bytes."<<endl;
  char* memblock = new char[size];
  _inRealFile.read(memblock, size);

  std::vector<char> memblockvec;
  for (int32_t i=0; i<size; i++) {
    memblockvec.push_back(memblock[i]);
  }

  if (!version.compare("C2"))
    fillWaveformC2(memblockvec);
  else if (!version.compare("Cx"))
    fillWaveform(memblockvec);

  _inRealFile.close();

  delete[] memblock;
}

void PulseAna::fillWaveformC2(std::vector<char> memblock)
{

  // //print the buffer
  // for (int64_t i = 0; i < 1000; i = i + 32)
  // {
  //   cout<<i<<" bytes"<<endl;
  //   for (int32_t j = 0; j < 32; j++)
  //     cout<<hex<<(uint32_t)m_memblock[i+j]<<" ";
  //     cout<<dec<<endl;
  // }

  int32_t cntHeader = 0;
  int32_t cntTrailer = 0;
  int64_t timestamp = -1;
  int64_t trigCount = -1;
  int32_t fwhmWidth = -1;
  int32_t derivSlope = -1;
  int32_t maxAmplitude = -1;
  int32_t classification = -1;
  int32_t cutFlags = -1;
  int32_t pulseArea = -1;
  int32_t bufX[3000], bufY[3000], nSamples=0;
  int32_t invert32 = 31;
  int32_t ofCnt = 0;
  bool pulseOverflow = false;
  bool header = false;
  bool maybeTrailer = false;
  bool trailer = false;
  bool data = false;
  //Read through buffer and find information on pulses
  for (int32_t i = 0; i < memblock.size()-32; i = i + 4) //read in 32 bits at a time.
  {
    //----------read trailer---------------------------
    if (trailer) {
      //read fwhm width, max amplitude and classification
      int8_t slp = 0xC0; //1100_0000 or derivslope(6bit)
      derivSlope =   (int32_t)(slp | (int8_t)memblock[i+1]) <<  0;
      derivSlope =   (int32_t)((int8_t)((uint8_t)memblock[i+1]<<2)/4 ) <<  0;
      fwhmWidth =    (int64_t)(uint8_t)memblock[i+2]  <<  8 |
                     (int64_t)(uint8_t)memblock[i+3]  <<  0;
      maxAmplitude = (int64_t)(uint8_t)memblock[i+4]  <<  8 |
                     (int64_t)(uint8_t)memblock[i+5]  <<  0;
      cutFlags =       (int64_t)(uint8_t)memblock[i+6]    << 8 |
                       (int64_t)(uint8_t)memblock[i+7]    << 0;
      classification = (int64_t)(uint8_t)memblock[i+8]   <<  8 |
                       (int64_t)(uint8_t)memblock[i+9]   <<  0;
      pulseArea = (int64_t)(uint8_t)memblock[i+10]  <<  8 |
                  (int64_t)(uint8_t)memblock[i+11]  <<  0;

      // derivSlope = derivSlope/4;
      i = i + 12;//to end the trailer and proceed to a new header
      trailer = false;
      cntTrailer = 0;

      //print out all information
      // cout<<"Pulse "<<trigCount<<" at "<<timestamp
      //     <<"   maxAmplitude "<<maxAmplitude
      //     <<"   fwhmWidth "<<fwhmWidth
      //     <<"   cutFlags "<<cutFlags
      //     <<"   classification "<<classification
      //     <<"   area "<<pulseArea
      //     <<"   slope "<<derivSlope
      //     <<"   i "<<i
      //     <<endl;
      // //show the graph
      // getchar();
      //save all values to pulse;
      Pulse* pulse = new Pulse (timestamp, trigCount, maxAmplitude, fwhmWidth,
                               pulseArea, derivSlope, cutFlags, classification);
      pulse->setWf(nSamples, bufX, bufY);
      m_vp.push_back(*pulse);
      delete(pulse);

      nSamples = 0;
      continue;
    }

    //----------read header----------------------------
    if (header) {
      //read timestamp and trigger counter
      // wfLen     = (int64_t)memblock[i]    << 0;
      // geel_trig = (int64_t)memblock[i+1]  << 8 |
                  // (int64_t)memblock[i+2]  << 0;
      timestamp = (int64_t)(uint8_t)memblock[i+3]  << 32 |
                  (int64_t)(uint8_t)memblock[i+4]  << 24 |
                  (int64_t)(uint8_t)memblock[i+5]  << 16 |
                  (int64_t)(uint8_t)memblock[i+6]  <<  8 |
                  (int64_t)(uint8_t)memblock[i+7]  <<  0;
      trigCount = (int64_t)(uint8_t)memblock[i+8]  << 56 |
                  (int64_t)(uint8_t)memblock[i+9]  << 48 |
                  (int64_t)(uint8_t)memblock[i+10] << 40 |
                  (int64_t)(uint8_t)memblock[i+11] << 32 |
                  (int64_t)(uint8_t)memblock[i+12] << 24 |
                  (int64_t)(uint8_t)memblock[i+13] << 16 |
                  (int64_t)(uint8_t)memblock[i+14] <<  8 |
                  (int64_t)(uint8_t)memblock[i+15] <<  0;
      i = i + 12; //to end the header and proceed to data
      data = true;
      header = false;
      cntHeader = 0;
      continue;
    }

    //----------search for header/trailer--------------
    for (int32_t j = 0; j < 4; j+=2) {
      if(!header) { // only check for header if not yet in header
        if ((int8_t)memblock[i+j] == (int8_t)0xAA && (int8_t)memblock[i+j+1] == (int8_t)0xFF )
          cntHeader++;
        else
          cntHeader = 0; //reset the counter if it's not header
        if (cntHeader == 8) {
          header = true;
          pulseOverflow = false;
          ofCnt = 0;
        } //test if header

      }

      if(!trailer) {
        if ((int8_t)memblock[i+j] == (int8_t)0xEE && (int8_t)memblock[i+j+1] == (int8_t)0xAA) {
          cntTrailer++;
          maybeTrailer = true;
        }
        else
          cntTrailer = 0; //reset the counter if it's not trailer
        if (cntTrailer == 6) { //test if trailer
          trailer = true;
          maybeTrailer = false;
          data = false;
        }
      }

      continue;
    }//end of 32bit word

    //----------read data------------------------------
    if (data && !maybeTrailer && !pulseOverflow) {
      //take four 8bit numbers at a time and put them into a buffer
      for (int32_t j = 0; j < 4; j++) {
        bufX[nSamples] = nSamples;
        //make a signed number
        bufY[nSamples] = (int32_t)(int8_t)memblock[i+j];
        nSamples++;
      }
      ofCnt+=4;
      if(ofCnt>=512) {
        pulseOverflow = true;
      }
    }
  }//end of buffer
}





void PulseAna::fillWaveform(std::vector<char> memblock)
{
  //print the buffer
  // for (int32_t i = 0; i < memblock.size()-32; i = i + 32)
  // {
  //   if (i > 3494*512 || i < 3489*512) continue;
  //   cout<<i<<" bytes"<<endl;
  //   for (int32_t j = 0; j < 32; j++)
  //     cout<<hex<<(uint32_t)(uint8_t)memblock[i+j]<<" ";
  //   cout<<dec<<endl;
  // }
// getchar();
  int32_t cntHeader = 0;
  int32_t cntTrailer = 0;
  int64_t timestamp = -1;
  int64_t trigCount = -1;
  int64_t cnt = 0;
  int32_t fwhmWidth = -1;
  int32_t derivSlope = -1;
  int32_t maxAmplitude = -1;
  int32_t classification = -1;
  int32_t cutFlags = -1;
  int32_t pulseArea = -1;
  int32_t bufX[10000], bufY[10000], nSamples=0;
  int32_t invert32 = 31;
  int32_t ofCnt = 0;
  int32_t geel_trigtime = 0;
  int32_t waveformLength = 0;
  bool pulseOverflow = false;
  bool header = false;
  bool maybeTrailer = false;
  bool pulseCompleted = false;
  bool data = false;
  //Read through buffer and find information on pulses
  for (int32_t i = 0; i < memblock.size()-32; i = i + 4) //read in 32 bits at a time.
  {
      // if (cnt>3488) {
      //   for (int32_t j = 0; j < 4; j++)
      //     cout<<hex<<(uint32_t)(uint8_t)memblock[i+j]<<" ";
      //   cout<<dec<<endl;
      // }

    //----------search for header/trailer--------------
    if ((int8_t)memblock[i] == (int8_t)0xBB && (int8_t)memblock[i+1] == (int8_t)0xFF ) { //test if header
      //safety: check if the previous pulse ended properly.
      nSamples = 0;


      waveformLength = (int64_t)(uint8_t)memblock[i+2]  <<  8 |
                       (int64_t)(uint8_t)memblock[i+3]  <<  0;
      data = true;
      pulseOverflow = false;
      ofCnt = 0;
      cntHeader = 0;
      pulseCompleted=false;
      // cout<<endl<<endl<<" Pulse "<<cnt<<endl;
      continue;
    } //test if header

    if ((int8_t)memblock[i] == (int8_t)0xFF && (int8_t)memblock[i+1] == (int8_t)0xAA) { //test if trailer
      maxAmplitude = (int64_t)(uint8_t)memblock[i+2]  <<  8 |
                     (int64_t)(uint8_t)memblock[i+3]  <<  0;
      // // print out all information
      // cout<<"Pulse "<<trigCount<<" at "<<timestamp
      //     <<"   maxAmplitude "<<maxAmplitude
      //     <<endl;
      // // show the graph
      // cout<<" byte "<<i<<" of "<<memblock.size()<<endl;
      // // getchar();
      Pulse* pulse = new Pulse (0, cnt, maxAmplitude, 0, 0, 0, 0, 0);
      pulse->setWf(nSamples, bufX, bufY);
      pulse->print();
      m_vp.push_back(*pulse);
      delete(pulse);
      cnt++;


      // if (maxAmplitude < 147)
      // {
      //   _wave = new TGraph(nSamples,bufX,bufY);
      //   cout<<"Pulse "<<trigCount<<" at "<<timestamp
      //       <<"   maxAmplitude "<<maxAmplitude
      //       <<endl;
      //   showWaveform();
      // }
      // if (_can) delete _can;
      // _can = 0;
      pulseCompleted = true;
      nSamples = 0;
      ofCnt = 0;
      continue;
    }

    //----------read data------------------------------
    if (data && !pulseOverflow) {
      //take four 8bit numbers at a time and put them into a buffer
      for (int32_t j = 0; j < 4; j++) {
        bufX[nSamples] = nSamples;
        //make a signed number
        bufY[nSamples] = (int32_t)(int8_t)memblock[i+j];
        nSamples++;
      }
      ofCnt+=4;
      if(ofCnt>512) {
        pulseOverflow = true;
      }//eCompleted = false;
    //cuts
    }
  }//end of buffer

}

void PulseAna::analyse() {

  for (int64_t i=0; i<m_vp.size(); i++) {
    if (-m_vp.at(i).m_slope*64 + m_vp.at(i).m_width < NBINS_FWHM_SLOPE)
      m_exthistfwhmslope[-m_vp.at(i).m_slope*64 + m_vp.at(i).m_width]++;
    if(m_vp.at(i).m_width<NBINS)
      m_histfwhm[m_vp.at(i).m_width]++;
    if(m_vp.at(i).m_ampl<NBINS)
      m_histmaxampl[m_vp.at(i).m_ampl]++;
    if(m_vp.at(i).m_area<NBINS_EXTAREA)
      m_exthistarea[m_vp.at(i).m_area]++;
    if(m_vp.at(i).m_slope<NBINS_FWHM_SLOPE)
      m_histslope[m_vp.at(i).m_slope]++;
    m_histfwhm[m_vp.at(i).m_width]++;
    m_histmaxampl[m_vp.at(i).m_ampl]++;
    m_exthistarea[m_vp.at(i).m_area]++;
    m_histslope[m_vp.at(i).m_slope]++;
    if (m_vp.at(i).m_slope*64 + m_vp.at(i).m_width < NBINS_FWHM_SLOPE) {
      m_exthistfwhmslope[m_vp.at(i).m_slope*64 + m_vp.at(i).m_width]++;
    }
    if (m_vp.at(i).m_class==2) {//if accepted
      m_exthistalphaarea[m_vp.at(i).m_area]++;
      m_histalphamaxampl[m_vp.at(i).m_ampl]++;
    }
  }
}

void PulseAna::printHistLine(int32_t i) {
  cout << " " << (uint16_t)0xAAAA;
  cout << " " << (uint16_t)i;
  if (i<NBINS_MAXAMPL)cout << " " << (uint32_t)m_histfwhm[i];
  if (i<NBINS_MAXAMPL)cout << " " << (uint32_t)m_histmaxampl[i];
  if (i<NBINS_MAXAMPL)cout << " " << (uint32_t)m_histalphamaxampl[i];
  if (i<NBINS_MAXAMPL)cout << " " << (uint32_t)0;
  if (i<NBINS_FWHM_SLOPE)cout << " " << (uint32_t)m_exthistfwhmslope[i];
  if (i<NBINS_EXTAREA)cout << " " << (uint32_t)m_exthistarea[i];
  if (i<NBINS_EXTAREA)cout << " " << (uint32_t)m_exthistalphaarea[i] << endl;
}

void PulseAna::writeToHistFile(string fName) {
  ofstream outfile (fName.c_str(), ios::out | ios::binary);
  if (!outfile.is_open())
    throw "PulseAna::writeToHistFile : Output binary file failed to open!";

  char buf[32];

  for (int32_t i=0; i<NBINS_EXTAREA; i++) {
    for (int32_t a=0; a<32; a++)
      buf[a]=0;
    if (i<NBINS_MAXAMPL) {
      buf[0] = (char)0xAA;
      buf[1] = (char)0xAA;
      buf[2] = (char)(i >> 8);
      buf[3] = (char)(i & 0xFF);
      buf[6] = (char)((int32_t)m_histfwhm[i] >> 8);
      buf[7] = (char)((int32_t)m_histfwhm[i] & 0xFF);
      buf[10] = (char)((int32_t)m_histmaxampl[i] >> 8);
      buf[11] = (char)((int32_t)m_histmaxampl[i] & 0xFF);
      buf[14] = (char)((int32_t)m_histalphamaxampl[i] >> 8);
      buf[15] = (char)((int32_t)m_histalphamaxampl[i] & 0xFF);
      buf[22] = (char)((int32_t)m_exthistfwhmslope[i] >> 8);
      buf[23] = (char)((int32_t)m_exthistfwhmslope[i] & 0xFF);
      buf[26] = (char)((int32_t)m_exthistarea[i] >> 8);
      buf[27] = (char)((int32_t)m_exthistarea[i] & 0xFF);
      buf[30] = (char)((int32_t)m_exthistalphaarea[i] >> 8);
      buf[31] = (char)((int32_t)m_exthistalphaarea[i] & 0xFF);
    }
    else if (i<NBINS_FWHM_SLOPE) {
      buf[0] = (char)0xAA;
      buf[1] = (char)0xAA;
      buf[2] = (char)(i >> 8);
      buf[3] = (char)(i & 0xFF);
      buf[22] = (char)((int32_t)m_exthistfwhmslope[i] >> 8);
      buf[23] = (char)((int32_t)m_exthistfwhmslope[i] & 0xFF);
      buf[26] = (char)((int32_t)m_exthistarea[i] >> 8);
      buf[27] = (char)((int32_t)m_exthistarea[i] & 0xFF);
      buf[30] = (char)((int32_t)m_exthistalphaarea[i] >> 8);
      buf[31] = (char)((int32_t)m_exthistalphaarea[i] & 0xFF);
    }
    else if (i<NBINS_EXTAREA){
      buf[0] = (char)0xAA;
      buf[1] = (char)0xAA;
      buf[2] = (char)(i >> 8);
      buf[3] = (char)(i & 0xFF);
      buf[26] = (char)((int32_t)m_exthistarea[i] >> 8);
      buf[27] = (char)((int32_t)m_exthistarea[i] & 0xFF);
      buf[30] = (char)((int32_t)m_exthistalphaarea[i] >> 8);
      buf[31] = (char)((int32_t)m_exthistalphaarea[i] & 0xFF);
    }
    else
      break;
    outfile.write(buf, 32);
  }

  outfile.close();
}



void PulseAna::showPulses() {
  TApplication* app = new TApplication("app",0,0);
  TCanvas can ("c1","c1",800,600) ;//new TCanvas("can","can",800,600);
  //set the cuts
  int32_t cutWidthMin = 30;
  int32_t cutWidthMax = 255;
  int32_t cutAmplMin = 0;
  int32_t cutAmplMax = 255;
  int32_t cutAreaMin = 0;
  int32_t cutAreaMax = 4095;
  int32_t cutSlopeMin = 0;
  int32_t cutSlopeMax = -32;
  for (int32_t i=0; i<m_vp.size(); i++) {
    //cuts
    if (m_vp.at(i).m_width<cutWidthMin) continue;
    if (m_vp.at(i).m_width>=cutWidthMax) continue;
    if (m_vp.at(i).m_ampl<cutAmplMin) continue;
    if (m_vp.at(i).m_ampl>=cutAmplMax) continue;
    if (m_vp.at(i).m_area<cutAreaMin) continue;
    if (m_vp.at(i).m_area>=cutAreaMax) continue;
    if (m_vp.at(i).m_slope>=cutSlopeMin) continue;
    if (m_vp.at(i).m_slope<cutSlopeMax) continue;

    can.Clear();
    // can =  TCanvas ("c1","c1",800,600);
    m_vp.at(i).print();
    can.Update();
    can.WaitPrimitive();
    can.Update();

  }

  delete(app);

}
