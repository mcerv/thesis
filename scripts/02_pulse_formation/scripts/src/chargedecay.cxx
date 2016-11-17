#include "drawfuns.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TF1.h"
#include "TGraph.h"
#include "Tlegend.h"
#include <iostream>
#include <sstream>
#include "math.h"

using namespace std;

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

  //create infinite bandwidth response
  const int nSamples = 1000;
  const int endShape = 40;
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

  TGraph *gOriginal = new TGraph(nSamples, xAxis, original);
  dr->prettify(gOriginal);
  gOriginal->SetLineColor(kRed+1);
  gOriginal->GetXaxis()->SetRangeUser(0,nSamples);
  gOriginal->GetXaxis()->SetTitle("Time [a.u.]");
  gOriginal->GetYaxis()->SetTitle("Voltage [a.u.]");

  //create a bandwidth limited response
  int nLimited = 5;
  float limited[nLimited][nSamples];
  limitation(original, limited[0], nSamples, 0.08);
  limitation(original, limited[1], nSamples, 0.01);
  limitation(original, limited[2], nSamples, 0.005);
  limitation(original, limited[3], nSamples, 0.002);
  limitation(original, limited[4], nSamples, 0.0005);


  TGraph *gLimited[nLimited];
  for (int i=0; i<nLimited; i++) {
    gLimited[i] = new TGraph(nSamples, xAxis, limited[i]);
    dr->prettify(gLimited[i]);
    gLimited[i]->SetLineColor(dr->clr[11-i]);
  }


  TCanvas* can = new TCanvas("fwhm","fwhm",800,470);
  dr->prettify(can);
  gOriginal->Draw("AL");
  for (int i=0; i<nLimited; i++) {
    gLimited[i]->Draw("same");
  }

  can->Update();
  can->WaitPrimitive();
  can->Update();
  can->Clear();

  // create an integration
  const int nIntegrations = 5;
  float integrate[nIntegrations][nSamples];
  for (int i=0; i<nIntegrations; i++) {
    clear(integrate[i], nSamples);
    integration(original, integrate[i], nSamples);
    normalise(integrate[i], nSamples, 20);
    limitation(integrate[i], nSamples, 0.02);
  }
  decayProduct(integrate[0], nSamples, 10000);
  decayProduct(integrate[1], nSamples, 1000);
  decayProduct(integrate[2], nSamples, 400);
  decayProduct(integrate[3], nSamples, 100);
  decayProduct(integrate[4], nSamples, 40);



  TGraph *gIntegrated[nIntegrations];
  gOriginal->Draw("AL");
  for (int i=0; i<nIntegrations; i++) {
    gIntegrated[i] = new TGraph(nSamples, xAxis, integrate[i]);
    dr->prettify(gIntegrated[i] );
    gIntegrated[i]->Draw("same");
    gIntegrated[i]->SetLineColor(dr->clr[11-i]);

  }

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
