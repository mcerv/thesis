#include <iostream>
#include <stdint.h>
#include <fstream>
#include <vector>

using namespace std;

class MeasSet {


public:
  MeasSet(string fileName, int32_t nCols);
  MeasSet();
  ~MeasSet();

  vector<string> colTitle;
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
  vector<string> comment;
  int32_t nRuns;


private:

};
