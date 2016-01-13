#include "measset.h"


MeasSet::MeasSet() {}
MeasSet::~MeasSet() {}
MeasSet::MeasSet(string fileName, int32_t nCols=15) {
  // this will read in the whole csv file.
  nRuns=0;
  ifstream inFile;
  inFile.open(fileName.c_str());

  //first row is a title.
  std::string name;
  char delim;
  for (int i=0; i<nCols; i++) {
    inFile >> name;
    std::cout << name << std::endl;
    colTitle.push_back(name);
  }

  // read other lines
  double buf;
  while (1) {
    //reading results
    inFile >> buf >> delim; cout<<buf<<" ";
    runNumber.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    noiseDb.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    noise.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    noiseStd.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    noiseAvg.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    maxAmpl.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    maxAmplStd.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    maxAmplAvg.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    area.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    areaStd.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    areaAvg.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    width.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    widthStd.push_back(buf);
    inFile >> buf >> delim; cout<<buf<<" ";
    widthAvg.push_back(buf);
    inFile >> name; cout<<name<<" ";
    comment.push_back(name);
    nRuns++;
    cout<<"nRuns "<<nRuns<<endl;

    if (!inFile.good()) break;
  }

}
