#include <iostream>
#include <stdint.h>


class MeasSet {


public:
  MeasSet();
  //MeasSet(int32_t nCols, int32_t nRows);
  ~MeasSet();
  vector<double> runNumber;
  vector<double> noiseDb;
  vector<double> noiseDbStd;
  vector<double> noise;
  vector<double> noiseStd;
  vector<double> noiseAvg;
  vector<double> maxAmpl;
  vector<double> maxAmplStd;
  vector<double> maxAmplAvg;
  vector<double> area;
  vector<double> areaStd;
  vector<double> areaAvg;
  vector<double> width;
  vector<double> widthStd;
  vector<double> widthAvg;


private:

};

MeasSet::MeasSet() {}
MeasSet::~MeasSet() {}
//MeasSet::MeasSet(int32_t nCols, int32_t nRows) {}
