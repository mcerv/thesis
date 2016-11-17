#include "drawfuns.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TF1.h"
#include "TGraph.h"
#include "Tlegend.h"
#include "TAxis.h"
#include "TGaxis.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "math.h"

using namespace std;

void rescaleaxis(TGraph *g,double scale)
{
 int N=g->GetN();
 double *y=g->GetX();
 int i=0;
 while(i<N)
 {
 y[i]=y[i]*scale;
 i=i+1;
 }
 g->GetHistogram()->Delete();
 g->SetHistogram(0);
 }
void clear (float* arr, int nSamples) {
  for (int i=0; i<nSamples; i++) {
    arr[i]=0;
  }
}
void axis (float* arr, int nSamples) {
  for (int i=0; i<nSamples; i++) {
    arr[i]=i;
  }
}
void normalise (float* arr, int nSamples, int normFactor) {
  for (int i=0; i<nSamples; i++) {
    arr[i]/=normFactor;
  }
}
void integration(float* origArr, float* integArr, int nSamples) {
  integArr[0] = origArr[0];
  for (int i=1; i<nSamples; i++) {
    integArr[i] = integArr[i-1] + origArr[i];
  }
}
void limitation(float* origArr, float* limArr, const int nSamples, float bandwidthFactor) {
  //high bandwidthFactor -> high bandwidth (between 0-1)
  limArr[0]=0;
  for (int i=1; i<nSamples; i++) {
    limArr[i] = bandwidthFactor  * origArr[i] + (1-bandwidthFactor) * limArr[i-1];
  }
}
void limitation(float* limArr, const int nSamples, float bandwidthFactor) {
  //perform the limitation on itself
  //high bandwidthFactor -> high bandwidth (between 0-1)
  float origArr[nSamples];
  for (int i=0; i<nSamples; i++) {
    origArr[i] = limArr[i];
  }
  limArr[0]=0;
  for (int i=1; i<nSamples; i++) {
    limArr[i] = bandwidthFactor  * origArr[i] + (1-bandwidthFactor) * limArr[i-1];
  }


}
void decayProduct(float* limArr, const int nSamples, float decayConstant) {
  // y = A exp(-x)
  float decayArr[nSamples];
  for (int i=0; i<nSamples; i++) {
    decayArr[i] = exp(-i/decayConstant);
    limArr[i]*=decayArr[i];
  }
}
void decayFun(float* decayArr, const int nSamples, float decayConstant) {
  for (int i=0; i<nSamples; i++) {
    decayArr[i] = exp(-i/decayConstant);
  }
}
void convolute(float *C, float *A, float *B, int lenA, int lenB)
{
	int nconv;
	int i, j, i1;
	float tmp;
	//allocated convolution array
	nconv = lenA+lenB-1;
	//convolution process
	for (i=0; i<nconv; i++)
	{
		i1 = i;
		tmp = 0.0;
		for (j=0; j<lenB; j++)
		{
			if(i1>=0 && i1<lenA)
				tmp = tmp + (A[i1]*B[j]);

			i1 = i1-1;
			C[i] = tmp;
		}
	}
}
void convolve(float* Result,
              const float* Signal,
              const float* Kernel,
              size_t SignalLen,
              size_t KernelLen) {
  size_t n;

  for (n = 0; n < SignalLen + KernelLen - 1; n++)
  {
    size_t kmin, kmax, k;

    Result[n] = 0;

    kmin = (n >= KernelLen - 1) ? n - (KernelLen - 1) : 0;
    kmax = (n < SignalLen - 1) ? n : SignalLen - 1;

    for (k = kmin; k <= kmax; k++)
    {
      Result[n] += Signal[k] * Kernel[n - k];
    }
  }
}




int32_t main() {
  TApplication* app = new TApplication ("app",0,0);
  DrawFuns* dr = new DrawFuns();

  const int nSamples = 1000; //0.1/0.01ns
  const int endShape = 40; //  0.1/0.01ns
  float original[nSamples];
  float xAxis[nSamples];
  float maxHeight = 1;
  //TRIANGLE
  for (int i=0; i<endShape; i++) {
    original[i] = maxHeight - float(i)/(endShape-1);
    xAxis[i]=i;
  }
  //SQUARE
  // for (int i=0; i<endShape; i++) {
  //   original[i] = maxHeight;
  //   xAxis[i]=i;
  // }
  //rest of the array
  for (int i=endShape; i<nSamples; i++) {
    original[i] = 0;
    xAxis[i]=i;
  }
  //ADD NOISE
  // for (int i=0; i<nSamples; i++) {
  //   random();
  //   original[i] = original[i] + random()/1e10*2;
  // }

  //DEFINITION OF THE sampling Time
  float dt = 0.01; //0.1 ns per sample

  TGraph *gOriginal = new TGraph(nSamples, xAxis, original);
  rescaleaxis(gOriginal,dt);
  dr->prettify(gOriginal);
  gOriginal->SetLineColor(kRed+1);
  gOriginal->GetXaxis()->SetRangeUser(0,nSamples);
  gOriginal->GetXaxis()->SetTitle("Time [ns]");
  gOriginal->GetYaxis()->SetTitle("Voltage [a.u.]");

  //create a bandwidth limited response
  int nLimited = 5;
  float limited[nLimited][nSamples];
  float Alpha[nSamples] = {0.1116,0.0305,0.005,0.002,0.0005};
  limitation(original, limited[0], nSamples, Alpha[0]);
  limitation(original, limited[1], nSamples, Alpha[1]);
  limitation(original, limited[2], nSamples, Alpha[2]);
  limitation(original, limited[3], nSamples, Alpha[3]);
  limitation(original, limited[4], nSamples, Alpha[4]);

  //calculation of RC constant and Bandwidth Limit
  float RC[nSamples], f[nSamples];
  for (int i=0; i<nLimited; i++) {
    //recalculation taken from https://en.wikipedia.org/wiki/Low-pass_filter
    RC[i] = dt * (1-Alpha[i]) / Alpha[i];
    f[i] = 1 / (2 * 3.14 * RC[i]) * 1000; //MHz
    cout<< " Alpha ["<<i<<"]="<<Alpha[i]<<"\t RC = "<<RC[i]<<"\t, f = "<<f[i]<<endl;
  }

  TLegend *leg = new TLegend(0.7,0.5,0.95,0.95);
  TGraph *gLimited[nLimited];
  stringstream ss;

  for (int i=0; i<nLimited; i++) {
    gLimited[i] = new TGraph(nSamples, xAxis, limited[i]);
    rescaleaxis(gLimited[i],dt);
    dr->prettify(gLimited[i]);
    gLimited[i]->SetLineColor(dr->clr[11-i]);
    // gLimited[i]->GetXaxis()->SetLimits(0,1000*dt);
    ss.str("");
    ss << "f_{BW} = " <<  int(f[i]) << " MHz";
    leg->AddEntry(gLimited[i], ss.str().c_str() ,"l");
  }


  TCanvas* can = new TCanvas("fwhm","fwhm",800,470);
  dr->prettify(can);
  gOriginal->Draw("AL");
  gOriginal->GetXaxis()->SetRangeUser(0,10);
  // gOriginal->Draw("AL");

  for (int i=0; i<nLimited; i++) {
    gLimited[i]->Draw("same");
  }
  leg->Draw();

  can->Update();
  can->WaitPrimitive();
  can->Update();
  can->Clear();
  //----------------------------------CHARGE AMP------------------------------
  dt =0.1;
  rescaleaxis(gOriginal,10);
  dr->prettify(gOriginal);
  gOriginal->SetLineColor(kRed+1);
  gOriginal->GetXaxis()->SetRangeUser(0,nSamples);
  gOriginal->GetXaxis()->SetTitle("Time [ns]");
  gOriginal->GetYaxis()->SetTitle("Voltage [a.u.]");

  // create an integration
  const int nIntegrations = 5;
  float integrate[nIntegrations][nSamples];
  for (int i=0; i<nIntegrations; i++) {
    clear(integrate[i], nSamples);
    integration(original, integrate[i], nSamples);
    normalise(integrate[i], nSamples, 20);
    limitation(integrate[i], nSamples, 0.05);
  }

  float RCcon[nIntegrations] = {10000,1000,400,100,40}; //dt = 0.1ns -> 1000ns,100ns,40ns,10ns,4ns
  float F[nIntegrations];
  for (int i=0; i<nIntegrations; i++) {
    //recalculation taken from https://en.wikipedia.org/wiki/Low-pass_filter

    F[i] = 1 / (2 * 3.14 * RCcon[i]*dt) * 1000; // MHz
    cout<< "RC = "<<RC[i]<<"\t, f = "<<F[i]<<endl;
  }

  decayProduct(integrate[0], nSamples, 10000);  //RC=1000 ns
  decayProduct(integrate[1], nSamples, 1000);   //RC=100 ns
  decayProduct(integrate[2], nSamples, 400);    //RC=40 ns
  decayProduct(integrate[3], nSamples, 100);    //RC=10 ns
  decayProduct(integrate[4], nSamples, 40);     //RC=4 ns


  leg->Clear();
  TGraph *gIntegrated[nIntegrations];
  gOriginal->Draw("AL");
  gOriginal->GetXaxis()->SetRangeUser(0,100);
  for (int i=0; i<nIntegrations; i++) {
    gIntegrated[i] = new TGraph(nSamples, xAxis, integrate[i]);
    rescaleaxis(gIntegrated[i],dt);
    dr->prettify(gIntegrated[i] );
    gIntegrated[i]->Draw("same");
    gIntegrated[i]->SetLineColor(dr->clr[11-i]);
    ss.str("");
    ss << "#tau = " <<  int(RCcon[i]/10) << " ns";
    leg->AddEntry(gIntegrated[i], ss.str().c_str() ,"l");

  }
  leg->Draw();


  can->Update();
  can->WaitPrimitive();
  can->Update();
  can->Clear();


  // create a decay function
  // int lenC=0;
  // float decay[nSamples];
  // const int nSamplesConv = 2*nSamples-1;
  // float conv[nSamplesConv];
  // float xAxisConv[nSamplesConv];
  // axis(xAxisConv,nSamplesConv);
  // decayFun(decay,nSamples,200); //make a decay function
  // convolve(conv, integrate, decay, nSamples, nSamples);
  // TGraph *gConvoluted = new TGraph(nSamplesConv, xAxisConv, conv);
  // dr->prettify(gConvoluted);
  // gConvoluted->Draw("AL");
  // can->Update();
  // can->WaitPrimitive();
  // can->Update();
  // can->Clear();







  delete(can);
  delete(dr);
  delete(app);
  cout<<"Done."<<endl;
  return 0;
}
