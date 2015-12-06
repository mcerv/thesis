#include "pulse.h"

Pulse::Pulse(const Pulse& obj) {
  m_timestamp = obj.m_timestamp;
  m_trigCount = obj.m_trigCount;
  m_ampl = obj.m_ampl;
  m_width = obj.m_width;
  m_area = obj.m_area;
  m_slope = obj.m_slope;
  m_cutFlags = obj.m_cutFlags;
  m_class = obj.m_class;
  m_wfLen = obj.m_wfLen;
  m_graph = obj.m_graph;
  X.clear();
  Y.clear();
  for (int32_t i=0; i<obj.X.size(); i++) {
    X.push_back(obj.X.at(i));
  }
  for (int32_t i=0; i<obj.Y.size(); i++) {
    Y.push_back(obj.Y.at(i));
  }
}

Pulse::Pulse() {
  // histFwhm_v4 = new TH1D ("FWHM", "FWHM", NBINS,0,NBINS-1);
}


Pulse::Pulse(int32_t id) {
  // std::stringstream ss;
  // ss.str(""); ss << "FWHM_" << id;
  // histFwhm_v4 = new TH1D (ss.str().c_str(), ss.str().c_str(), NBINS,0,NBINS-1);
}

Pulse::Pulse(int64_t timestamp, int32_t trigCount, int16_t ampl, int16_t width,
             int16_t area, int16_t slope, int16_t cutFlags, int16_t classification) :
m_timestamp(timestamp),
m_trigCount(trigCount),
m_ampl(ampl),
m_width(width),
m_area(area),
m_slope(slope),
m_cutFlags(cutFlags),
m_class(classification),
m_wfLen(0)
//m_graph(0)
{
  // histFwhm_v4 = new TH1D ("FWHM", "FWHM", NBINS,0,NBINS-1);
}


Pulse::~Pulse() {
  // if (histFwhm_v4) delete histFwhm_v4;
}

void Pulse::setWf(int32_t n, int32_t* x, int32_t* y) {
  m_wfLen = n;
  for (int32_t i=0; i<n; i++) {
    X.push_back(x[i]);
    Y.push_back(y[i]);
  }
  // m_graph = 0;
}

TGraph* Pulse::getGraph() {
  if (m_wfLen < 1) return NULL;
  m_graph = TGraph (m_wfLen, &X[0], &Y[0]);
  return &m_graph;
}

void Pulse::print() {
  cout<<" ------------------------------------------"<<endl;
  cout<<" Pulse "<<m_trigCount<<" at "<<m_timestamp<<" :"<<endl;
  cout<<"  Amplitude    "<<m_ampl<<endl;
  cout<<"  Width        "<<m_width<<endl;
  cout<<"  Area         "<<m_area<<endl;
  cout<<"  Slope        "<<m_slope<<endl;
  cout<<"  Cut flags    "<<m_cutFlags<<endl;
  cout<<"  Class        "<<m_class<<endl;
  cout<<"  Waveform len "<<m_wfLen<<endl;

}
