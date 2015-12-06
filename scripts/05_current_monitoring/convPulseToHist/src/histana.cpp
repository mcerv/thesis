#include "histana.h"

#define S37a      0
#define S37n      1
#define S79a      3
#define S79n      2
#define S79ap     5
#define S79np     6
#define S79npinv  7
#define S52a      8
#define S52n      9
#define S52ap     11
#define S52np     12
#define Sin       10
//version 6: 12



HistAna::HistAna() {
  m_vp.clear();
}

HistAna::~HistAna() {

}


void HistAna::readFileIntoBuffer(string fName, vector<uint8_t>& USBdata) {
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
  if (fileSize==300000) {
    _inRealFile.seekg (150000);
  }

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
    //if (i >= (end-begin)-64 )
    if (i >= MAXBUFFBYTES)
      break;
    //cout<<i<<" bytes"<<endl;
    for (int32_t j = 0; j < 32; j++) {
      _inRealFile.read(&chbuf8,1);
      buf8 = (uint8_t)chbuf8;
      //USBdata[byte] = buf8;
      USBdata.push_back(buf8);
      byte++;
    }
    cout<<dec;//<<endl;
    i+=32;
  }
  //cout<<" Read in "<<i<<" bytes of "<<fileSize<<"."<<endl;
  _inRealFile.close();
}

void HistAna::extractHistogramData(vector<uint8_t>& USBdata, Plots *plot, string version) {

  //print the buffer
  // if (!version.compare("6")){
    // for (int32_t i = 0; i < 0x2FFF; i = i + 32)
    // {
    //   cout<<i<<" bytes"<<endl;
    //   for (int32_t j = 0; j < 32; j++)
    //     cout<<hex<<(uint32_t)USBdata[i+j]<<" ";
    //   cout<<dec<<endl;
    //   if (!(i%20)) getchar();
    // }
  // }

  //initialise buffers
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
  double exthistfwhmslope_v6[NBINS_FWHM_SLOPE];

  //reset buffers
  int32_t  bin = 0;
  for (int32_t i = 0; i < NBINS; i++) {
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
  for (int32_t i = 0; i < NBINS_FWHM_SLOPE; i++) {
    exthistfwhmslope_v6[i]=0;
  }

  //fill buffers
  for (int32_t i = 0; i < MAXBUFFBYTES-64; i = i + 32) {//read in 256 bits at a time.
    //256bit = 32 * 8bit

    //find header(64 bit 0xAA)
    int32_t headcnt = 0;
    for (int32_t j = 0; j < 2; j++) {
      if (USBdata.at(i+j) == 0xAA)
        headcnt++;
      else
        headcnt = 0;
    }
    if (headcnt != 2) {
      continue; // jump to the next 64 bit
    }
    //if it's header, fill the histograms with the next values.
    //first read the bin number. 16bit.
    bin = (uint32_t)(USBdata.at(i+2) << 8) |
          (uint32_t)(USBdata.at(i+3));
    //bin value is actually one less - we get too high of a value from Fpga
    bin -= 1;

    //first the longest histograms
    if (bin>NBINS_EXTAREA) continue;

    exthistarea_v5[bin] = (uint32_t)(USBdata.at(i+4+27) << 0 ) |
                          (uint32_t)(USBdata.at(i+4+26) << 8 ) |
                          (uint32_t)(USBdata.at(i+4+25) << 16) |
                          (uint32_t)(USBdata.at(i+4+24) << 24);
    exthistalphaarea_v5[bin] =  (uint32_t)(USBdata.at(i+4+23) << 0 ) |
                                (uint32_t)(USBdata.at(i+4+22) << 8 ) |
                                (uint32_t)(USBdata.at(i+4+21) << 16) |
                                (uint32_t)(USBdata.at(i+4+20) << 24);

    //then the middle length
    if (bin>NBINS_FWHM_SLOPE-1) continue;
    //there's a difference in mapping between VERSION 5 and 6
    if (!version.compare("6")) {
      exthistfwhmslope_v6[bin] =  (uint32_t)(USBdata.at(i+4+19) << 0 ) |
                                  (uint32_t)(USBdata.at(i+4+18) << 8 ) |
                                  (uint32_t)(USBdata.at(i+4+17) << 16) |
                                  (uint32_t)(USBdata.at(i+4+16) << 24);
    }

    //and in the end the shortest
    if (bin>NBINS) continue;

    histfwhm[bin] = (uint32_t)(USBdata.at(i+4+3) << 0 ) |
                    (uint32_t)(USBdata.at(i+4+2) << 8 ) |
                    (uint32_t)(USBdata.at(i+4+1) << 16) |
                    (uint32_t)(USBdata.at(i+4+0) << 24);
    histmaxampl[bin] =  (uint32_t)(USBdata.at(i+4+7) << 0 ) |
                        (uint32_t)(USBdata.at(i+4+6) << 8 ) |
                        (uint32_t)(USBdata.at(i+4+5) << 16) |
                        (uint32_t)(USBdata.at(i+4+4) << 24);
    histalphamaxampl[bin] =   (uint32_t)(USBdata.at(i+4+11) << 0 ) |
                              (uint32_t)(USBdata.at(i+4+10) << 8 ) |
                              (uint32_t)(USBdata.at(i+4+9 ) << 16) |
                              (uint32_t)(USBdata.at(i+4+8 ) << 24);
    //there's a difference in mapping between VERSION 5 and 6
    if (!version.compare("5")) {
      histarea[bin] = (uint32_t)(USBdata.at(i+4+15) << 0 ) |
                      (uint32_t)(USBdata.at(i+4+14) << 8 ) |
                      (uint32_t)(USBdata.at(i+4+13) << 16) |
                      (uint32_t)(USBdata.at(i+4+12) << 24);
      histalphaarea[bin] =  (uint32_t)(USBdata.at(i+4+19) << 0 ) |
                            (uint32_t)(USBdata.at(i+4+18) << 8 ) |
                            (uint32_t)(USBdata.at(i+4+17) << 16) |
                            (uint32_t)(USBdata.at(i+4+16) << 24);
    }
  }// end of reading into arrays


  //fill in the plots
  int32_t fwhmThres = 0;
  for (int32_t i = 0; i < NBINS; i++) {
    plot->histFwhm_v4->Fill(i, histfwhm[i]);
    if (i>=fwhmThres)
      plot->histFwhm_acc_v4->Fill(i, histfwhm[i]);
    else
      plot->histFwhm_rej_v4->Fill(i, histfwhm[i]);
  }
  for (int32_t i = 0; i < NBINS_MAXAMPL; i++) {
    plot->histMaxampl_v4->Fill(i, histmaxampl[i]);
    plot->histMaxampl_acc_v4->Fill(i, histalphamaxampl[i]);
    plot->histMaxampl_rej_v4->Fill(i, histmaxampl[i]-histalphamaxampl[i]);
  }
  for (int32_t i = 0; i < NBINS_EXTAREA; i++) {
    plot->extHistArea_v5->Fill(i, exthistarea_v5[i]);
    plot->extHistArea_acc_v5->Fill(i, exthistalphaarea_v5[i]);
    plot->extHistArea_rej_v5->Fill(i, exthistarea_v5[i]-exthistalphaarea_v5[i]);
  }
  if (!version.compare("5")) {
    for (int32_t i = 0; i < NBINS_AREA; i++) {
      plot->histArea_v4->Fill(i, histarea[i]);
      plot->histArea_acc_v4->Fill(i, histalphaarea[i]);
      plot->histArea_acc_fit_v4->Fill(i, histalphaarea[i]);
      plot->histArea_rej_v4->Fill(i, histarea[i]-histalphaarea[i]);
    }
  }
  if (!version.compare("6")) {
    int32_t cnt = 0;
    for (int32_t y = 0; y < NBINS_FWHM_SLOPE_Y; y++) {
      for (int32_t x = 0; x < NBINS_FWHM_SLOPE_X; x++) {
      plot->extHistFwhmSlope_v6->Fill(x,-y, exthistfwhmslope_v6[y*NBINS_FWHM_SLOPE_X + x]);
      }
    }
  }


  //add the plots to the internal buffer.
  //addEntry(plot);

}

void HistAna::addEntry (Plots *plot) {
  m_vp.push_back(*plot);
}

TH1D* HistAna::extToNormal(TH1D* hOld) {
  //old hist has 4096 bins and the new one only 256.
  const char *oldName = hOld->GetName();
  std::stringstream ss;
  ss << oldName << "_new";
  TH1D *hNew = new TH1D (ss.str().c_str(),ss.str().c_str(),256,0,255);
  double binBuf=0;
  for (int32_t i=0; i<256; i++) {
    binBuf=0;
    for (int32_t cnt=0; cnt<16; cnt++) {
      binBuf+=(double)hOld->GetBinContent(i*16+cnt);
    }
    hNew->Fill(i,binBuf);
  }

  return hNew;
}



void HistAna::plotReactor() {

  TApplication *app = new TApplication("app",0,0);
  DrawFuns *dr = new DrawFuns();
  //--------------------------- analysis ------------------------------


  //----debug the missing 005 area hist and 000 area hist
  m_vp.at(S79ap).histArea_v4 = extToNormal(m_vp.at(S79ap).extHistArea_v5);
  m_vp.at(S37a).histArea_v4 = extToNormal(m_vp.at(S37a).extHistArea_v5);

  //--------- comparison pre-post pumping S52 1e14 ------------
  dr->prettify(m_vp.at(S52a).histFwhm_v4,"blue");
  dr->prettify(m_vp.at(S52ap).histFwhm_v4);
  dr->prettify(m_vp.at(S52a).histMaxampl_v4,"blue");
  dr->prettify(m_vp.at(S52ap).histMaxampl_v4);
  dr->prettify(m_vp.at(S52a).histArea_v4,"blue");
  dr->prettify(m_vp.at(S52ap).histArea_v4);
  dr->prettify(m_vp.at(S52n).histFwhm_v4,"blue");
  dr->prettify(m_vp.at(S52np).histFwhm_v4);
  dr->prettify(m_vp.at(S52n).histMaxampl_v4,"blue");
  dr->prettify(m_vp.at(S52np).histMaxampl_v4);
  dr->prettify(m_vp.at(S52n).histArea_v4,"blue");
  dr->prettify(m_vp.at(S52np).histArea_v4);
  dr->normalise(m_vp.at(S52a).histFwhm_v4);
  dr->normalise(m_vp.at(S52ap).histFwhm_v4);
  dr->normalise(m_vp.at(S52a).histMaxampl_v4);
  dr->normalise(m_vp.at(S52ap).histMaxampl_v4);
  dr->normalise(m_vp.at(S52a).histArea_v4);
  dr->normalise(m_vp.at(S52ap).histArea_v4);
  dr->normalise(m_vp.at(S52n).histFwhm_v4);
  dr->normalise(m_vp.at(S52np).histFwhm_v4);
  dr->normalise(m_vp.at(S52n).histMaxampl_v4);
  dr->normalise(m_vp.at(S52np).histMaxampl_v4);
  dr->normalise(m_vp.at(S52n).histArea_v4);  //dr->prettify(m_vp.at(S52np).extistArea_v4);
  dr->normalise(m_vp.at(S52np).histArea_v4);  //dr->prettify(m_vp.at(S52np).extistArea_v4);

  TCanvas *cPump[2];
  cPump[0] = new TCanvas("can","can",1200,800);
  dr->prettify(cPump[0]);
  cPump[0]->Divide(3,2);
  cPump[0]->cd(1);
  m_vp.at(S52a).histFwhm_v4->Draw();
  m_vp.at(S52a).histFwhm_v4->GetXaxis()->SetRangeUser(0,40);
  m_vp.at(S52ap).histFwhm_v4->Draw("same");
  cPump[0]->cd(2);
  m_vp.at(S52a).histMaxampl_v4->Draw();
  m_vp.at(S52ap).histMaxampl_v4->Draw("same");
  cPump[0]->cd(3);
  m_vp.at(S52a).histArea_v4->Draw();
  m_vp.at(S52ap).histArea_v4->Draw("same");
  cPump[0]->cd(4);
  m_vp.at(S52n).histFwhm_v4->GetXaxis()->SetRangeUser(0,40);
  m_vp.at(S52n).histFwhm_v4->Draw();
  m_vp.at(S52np).histFwhm_v4->Draw("same");
  cPump[0]->cd(5);
  m_vp.at(S52n).histMaxampl_v4->Draw();
  m_vp.at(S52np).histMaxampl_v4->Draw("same");
  cPump[0]->cd(6);
  m_vp.at(S52n).histArea_v4->Draw();
  m_vp.at(S52np).histArea_v4->Draw("same");
  cPump[0]->Update();
  cPump[0]->WaitPrimitive();


  //--------- comparison pre-post pumping S79  4e14 ------------
  //TH1D* histAreaS79ap = extToNormal(m_vp.at(S79ap).extHistArea_v5);

  dr->prettify(m_vp.at(S79a).histFwhm_v4,"blue");
  dr->prettify(m_vp.at(S79ap).histFwhm_v4);
  dr->prettify(m_vp.at(S79a).histMaxampl_v4,"blue");
  dr->prettify(m_vp.at(S79ap).histMaxampl_v4);
  dr->prettify(m_vp.at(S79a).histArea_v4,"blue");
  dr->prettify(m_vp.at(S79ap).histArea_v4);
  //dr->prettify(histAreaS79ap);
  dr->prettify(m_vp.at(S79n).histFwhm_v4,"blue");
  dr->prettify(m_vp.at(S79np).histFwhm_v4);
  dr->prettify(m_vp.at(S79n).histMaxampl_v4,"blue");
  dr->prettify(m_vp.at(S79np).histMaxampl_v4);
  dr->prettify(m_vp.at(S79n).histArea_v4,"blue");
  dr->prettify(m_vp.at(S79np).histArea_v4);
  dr->normalise(m_vp.at(S79a).histFwhm_v4);
  dr->normalise(m_vp.at(S79ap).histFwhm_v4);
  dr->normalise(m_vp.at(S79a).histMaxampl_v4);
  dr->normalise(m_vp.at(S79ap).histMaxampl_v4);
  dr->normalise(m_vp.at(S79a).histArea_v4);
  dr->normalise(m_vp.at(S79ap).histArea_v4);
  //dr->normalise(histAreaS79ap);
  dr->normalise(m_vp.at(S79n).histFwhm_v4);
  dr->normalise(m_vp.at(S79np).histFwhm_v4);
  dr->normalise(m_vp.at(S79n).histMaxampl_v4);
  dr->normalise(m_vp.at(S79np).histMaxampl_v4);
  dr->normalise(m_vp.at(S79n).histArea_v4);
  dr->normalise(m_vp.at(S79np).histArea_v4);

  cPump[1] = new TCanvas("can1","can1",1200,800);
  dr->prettify(cPump[1]);
  cPump[1]->Divide(3,2);
  cPump[1]->cd(1);
  m_vp.at(S79a).histFwhm_v4->Draw();
  m_vp.at(S79a).histFwhm_v4->GetXaxis()->SetRangeUser(0,40);
  m_vp.at(S79ap).histFwhm_v4->Draw("same");
  cPump[1]->cd(2);
  m_vp.at(S79a).histMaxampl_v4->Draw();
  m_vp.at(S79ap).histMaxampl_v4->Draw("same");
  cPump[1]->cd(3);
  m_vp.at(S79a).histArea_v4->Draw();
  m_vp.at(S79ap).histArea_v4->Draw("same");
  cPump[1]->cd(4);
  m_vp.at(S79n).histFwhm_v4->GetXaxis()->SetRangeUser(0,40);
  m_vp.at(S79n).histFwhm_v4->Draw();
  m_vp.at(S79np).histFwhm_v4->Draw("same");
  cPump[1]->cd(5);
  m_vp.at(S79n).histMaxampl_v4->Draw();
  m_vp.at(S79np).histMaxampl_v4->Draw("same");
  cPump[1]->cd(6);
  m_vp.at(S79n).histArea_v4->Draw();
  m_vp.at(S79np).histArea_v4->Draw("same");
  cPump[1]->Update();
  cPump[1]->WaitPrimitive();

  delete(cPump[0]);
  delete(cPump[1]);



  //--------- comparison between samples - pumped ------------
  // int32_t thicke14 = 548;
  // int32_t thickS52 = 529;
  // int32_t thickS79 = 515;
  dr->prettify(m_vp.at(S52ap).histFwhm_v4,"blue");
  dr->prettify(m_vp.at(S52ap).histMaxampl_v4,"blue");
  dr->prettify(m_vp.at(S52ap).histArea_v4,"blue");
  dr->prettify(m_vp.at(S52np).histFwhm_v4,"blue");
  dr->prettify(m_vp.at(S52np).histMaxampl_v4,"blue");
  dr->prettify(m_vp.at(S52np).histArea_v4,"blue");
  dr->prettify(m_vp.at(S52a).histFwhm_v4,"light blue");
  dr->prettify(m_vp.at(S52a).histMaxampl_v4,"light blue");
  dr->prettify(m_vp.at(S52a).histArea_v4,"light blue");
  dr->prettify(m_vp.at(S52n).histFwhm_v4,"light blue");
  dr->prettify(m_vp.at(S52n).histMaxampl_v4,"light blue");
  dr->prettify(m_vp.at(S52n).histArea_v4,"light blue");
  dr->prettify(m_vp.at(S79ap).histFwhm_v4,"green");
  dr->prettify(m_vp.at(S79ap).histMaxampl_v4,"green");
  dr->prettify(m_vp.at(S79ap).histArea_v4,"green");
  dr->prettify(m_vp.at(S79np).histFwhm_v4,"green");
  dr->prettify(m_vp.at(S79np).histMaxampl_v4,"green");
  dr->prettify(m_vp.at(S79np).histArea_v4,"green");
  dr->prettify(m_vp.at(S79a).histFwhm_v4,"light green");
  dr->prettify(m_vp.at(S79a).histMaxampl_v4,"light green");
  dr->prettify(m_vp.at(S79a).histArea_v4,"light green");
  dr->prettify(m_vp.at(S79n).histFwhm_v4,"light green");
  dr->prettify(m_vp.at(S79n).histMaxampl_v4,"light green");
  dr->prettify(m_vp.at(S79n).histArea_v4,"light green");
  dr->prettify(m_vp.at(S37a).histFwhm_v4,"red");
  dr->prettify(m_vp.at(S37a).histMaxampl_v4,"red");
  dr->prettify(m_vp.at(S37a).histArea_v4,"red");
  dr->prettify(m_vp.at(S37n).histFwhm_v4,"red");
  dr->prettify(m_vp.at(S37n).histMaxampl_v4,"red");
  dr->prettify(m_vp.at(S37n).histArea_v4,"red");
  // dr->normalise(m_vp.at(S37a).histFwhm_v4);
  // dr->normalise(m_vp.at(S37a).histMaxampl_v4);
  // dr->normalise(m_vp.at(S37a).histArea_v4);
  // dr->normalise(m_vp.at(S37n).histFwhm_v4);
  // dr->normalise(m_vp.at(S37n).histMaxampl_v4);
  // dr->normalise(m_vp.at(S37n).histArea_v4);  //dr->prettify(m_vp.at(S52np).extistArea_v4);

  TCanvas *cSamples[2];
  cSamples[0] = new TCanvas("can","can",1200,800);
  dr->prettify(cSamples[0]);
  cSamples[0]->Divide(3,2);
  cSamples[0]->cd(1);
  m_vp.at(S37a).histFwhm_v4->Draw();
  m_vp.at(S37a).histFwhm_v4->GetXaxis()->SetRangeUser(0,40);
  // m_vp.at(S52ap).histFwhm_v4->Draw("same");
  // m_vp.at(S79ap).histFwhm_v4->Draw("same");
  // m_vp.at(S79a).histFwhm_v4->Draw("same");
  // m_vp.at(S52a).histFwhm_v4->Draw("same");
  cSamples[0]->cd(2);
  m_vp.at(S37a).histMaxampl_v4->Draw();
  // m_vp.at(S52ap).histMaxampl_v4->Draw("same");
  // m_vp.at(S79ap).histMaxampl_v4->Draw("same");
  // m_vp.at(S79a).histMaxampl_v4->Draw("same");
  // m_vp.at(S52a).histMaxampl_v4->Draw("same");
  cSamples[0]->cd(3);
  m_vp.at(S37a).histArea_v4->Draw();
  // m_vp.at(S52ap).histArea_v4->Draw("same");
  // m_vp.at(S79ap).histArea_v4->Draw("same");
  // m_vp.at(S79a).histArea_v4->Draw("same");
  // m_vp.at(S52a).histArea_v4->Draw("same");
  cSamples[0]->Update();

  cSamples[0]->cd(4);
  m_vp.at(S37n).histFwhm_v4->Draw();
  m_vp.at(S37n).histFwhm_v4->GetXaxis()->SetRangeUser(0,40);
  // m_vp.at(S52np).histFwhm_v4->Draw("same");
  // m_vp.at(S79np).histFwhm_v4->Draw("same");
  // m_vp.at(S79n).histFwhm_v4->Draw("same");
  // m_vp.at(S52n).histFwhm_v4->Draw("same");
  cSamples[0]->cd(5);
  m_vp.at(S37n).histMaxampl_v4->Draw();
  // m_vp.at(S52np).histMaxampl_v4->Draw("same");
  // m_vp.at(S79np).histMaxampl_v4->Draw("same");
  // m_vp.at(S79n).histMaxampl_v4->Draw("same");
  // m_vp.at(S52n).histMaxampl_v4->Draw("same");
  cSamples[0]->cd(6);
  m_vp.at(S37n).histArea_v4->Draw();
  // m_vp.at(S52np).histArea_v4->Draw("same");
  // m_vp.at(S79np).histArea_v4->Draw("same");
  // m_vp.at(S79n).histArea_v4->Draw("same");
  // m_vp.at(S52n).histArea_v4->Draw("same");
  cSamples[0]->Update();

  cSamples[0]->WaitPrimitive();

  delete(cSamples[0]);

  //--------- comparison pumped S79 POS and NEG voltage 4e14 ------------
  // therefore between S79np and S79npinv
  TCanvas *cPosNeg[2];
  //dr->prettify(histAreaS79ap);
  dr->prettify(m_vp.at(S79npinv).histFwhm_v4,"blue");
  dr->prettify(m_vp.at(S79np).histFwhm_v4);
  dr->prettify(m_vp.at(S79npinv).histMaxampl_v4,"blue");
  dr->prettify(m_vp.at(S79np).histMaxampl_v4);
  dr->prettify(m_vp.at(S79npinv).histArea_v4,"blue");
  dr->prettify(m_vp.at(S79np).histArea_v4);

  //dr->normalise(histAreaS79ap);
  dr->normalise(m_vp.at(S79npinv).histFwhm_v4);
  dr->normalise(m_vp.at(S79np).histFwhm_v4);
  dr->normalise(m_vp.at(S79npinv).histMaxampl_v4);
  dr->normalise(m_vp.at(S79np).histMaxampl_v4);
  dr->normalise(m_vp.at(S79npinv).histArea_v4);
  dr->normalise(m_vp.at(S79np).histArea_v4);

  cPosNeg[1] = new TCanvas("can1","can1",1200,800);
  dr->prettify(cPosNeg[1]);
  cPosNeg[1]->Divide(3,2);
  cPosNeg[1]->cd(4);
  m_vp.at(S79npinv).histFwhm_v4->GetXaxis()->SetRangeUser(0,40);
  m_vp.at(S79npinv).histFwhm_v4->Draw();
  m_vp.at(S79np).histFwhm_v4->Draw("same");
  cPosNeg[1]->cd(5);
  m_vp.at(S79npinv).histMaxampl_v4->Draw();
  m_vp.at(S79np).histMaxampl_v4->Draw("same");
  cPosNeg[1]->cd(6);
  m_vp.at(S79npinv).histArea_v4->Draw();
  m_vp.at(S79np).histArea_v4->Draw("same");
  cPosNeg[1]->Update();
  cPosNeg[1]->WaitPrimitive();

  delete(cPosNeg[1]);


  //S52real = 515, S79real = 529
  //can->SetEditable(true);
  //can->Modified();
  //can->Update();
  //getchar();

  delete(dr);
  //delete(can);
  delete(app);
}
