#include <string.h>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include "drawfuns.h"
#include <vector>
#include "TFile.h"
#include "TF1.h"
#include "TApplication.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TLine.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TColor.h"
#include "TMath.h"

#define DEBUG 0
#define S52 0
#define S79 1
#define S37 2
#define S52_363e14 3
#define S79_1e14 4



TF1* decayfun;
TF1* bandfun;
TF1* risefun;
TF1* errfun;
TF1* genfun;
TF1* genfallfun;

double finter(double *x, double *par)
{
    // return /*bandfun->EvalPar(x,par) * */decayfun->EvalPar(x,par) * risefun->EvalPar(x,par) / maxCur;
    // return /*bandfun->EvalPar(x,par) * */decayfun->EvalPar(x,par) * errfun->EvalPar(x,par);
    // return decayfun->EvalPar(x,par) * errfun->EvalPar(x,par) * genfun->EvalPar(x,par);
    return decayfun->EvalPar(x,par) * (errfun->EvalPar(x,par) * risefun->EvalPar(x,par) );
}




// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {

  Double_t clrScheme[6][20];
  // Double_t clrBase[6] = {kAzure, kTeal, kViolet, kPink, kGreen, kOrange};
  Double_t clrBase[6] = {kRed, kBlue, kGreen, kOrange, kViolet, kBlack };
  for (int32_t sample=0; sample<6;sample++) {
    Double_t clr = clrBase[sample];
    for (int32_t temp=0; temp<20;temp++) {
      clrScheme[sample][temp] = clrBase[sample] + temp/4 - 4.0;
    }
  }


  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();

  //set global variables and objects
  stringstream ss;
  TCanvas* can = new TCanvas("can","can",800,600);

  double riseEdge = 0;
  double fallEdge = 7;
  double errSlope = 2;
  double maxCur = 1;
  double tauGen = 0.5;
  double dispGen = -1;

  TF1* squarefun = new TF1("square", " 0*(x<[0]) + [2]*(x==[0]) +  [2]*([0]<x)*(x<[1]) + [2]*(x==[1]) + 0*(x>[1]) ",-5, 15);
  squarefun->SetParName(0,"risingEdge");
  squarefun->SetParName(1,"fallingEdge");
  squarefun->SetParName(2,"amplitude");
  squarefun->SetParameter(0,riseEdge);
  squarefun->SetParameter(1,fallEdge);
  squarefun->SetParameter(2,maxCur);
  squarefun->SetLineColor(kGreen);
  squarefun->Draw();

  decayfun = new TF1("decaydouble","[0] + ([1]-[0])*exp(-(x+[3])/[2])",-5,15);
  decayfun->SetParName(0,"chgInf");     //charge at infinity
  decayfun->SetParName(1,"chgStart");   //starting charge
  decayfun->SetParName(2,"tauRec");     //recombination
  decayfun->SetParName(3,"xDisp");      //displacement in x direction
  decayfun->SetParameter(0,0);
  decayfun->SetParameter(1,maxCur);
  decayfun->SetParameter(2,10); //tau
  decayfun->SetParameter(3,riseEdge);
  decayfun->SetLineColor(kBlue);
  decayfun->Draw("same");


  risefun = new TF1 ("rise","[1] + [0]*(1-exp(-(x-[5])/[2]))  - [0]*(1-exp(-(x-[4])/[2]))*(x>[4])", -5, 15);
  risefun->SetParName(0,"maxChg");
  risefun->SetParName(1,"minChg");
  risefun->SetParName(2,"tau");
  // risefun->SetParName(3,"tauEvap");
  risefun->SetParName(4,"fallEdge");
  risefun->SetParName(5,"dispX");
  risefun->SetParameter(0,maxCur);
  risefun->SetParameter(1,0);
  risefun->SetParameter(2,tauGen);
  // risefun->SetParameter(3,0.1);
  risefun->SetParameter(4,fallEdge); //+dispGen
  risefun->SetParameter(5,dispGen);
  risefun->SetLineColor(kBlack);
  risefun->Draw("same");

  errfun = new TF1 ("err", "[0]* (  0.5*(ROOT::Math::erf([3]*(x-[1]))+1) - 0.5*(ROOT::Math::erf([3]*(x-[2]))+1) ) ", -5, 15);
  errfun->SetParName(0,"maxCur");
  errfun->SetParName(1,"riseEdge");
  errfun->SetParName(2,"fallEdge");
  errfun->SetParName(3,"slope");
  errfun->SetParameter(0,maxCur);
  errfun->SetParameter(1,riseEdge);
  errfun->SetParameter(2,fallEdge);
  errfun->SetParameter(3,errSlope);
  errfun->SetLineColor(kViolet);
  errfun->Draw("same");


  // genfun = new TF1 ("gen", "[0]*(1-exp(-(x-[2])/[1])) - ", -5, 15);
  // genfun->SetParName(0,"maxCur");
  // genfun->SetParName(1,"tauGen");
  // genfun->SetParName(2,"dispX");
  // genfun->SetParameter(0,maxCur);
  // genfun->SetParameter(1,tauGen);
  // genfun->SetParameter(2,dispGen);
  // genfun->SetLineColor(kPink);
  // genfun->Draw("same");







  // double bandlimit = 2e9; //Hz
  // double bandtau = 1.0/2.0/3.14/bandlimit * 1e9; //in ns
  // cout<<" Bandwidth time constant "<<bandtau<<endl;
  // bandfun = new TF1 ("rise","[1] + [0]*(1-exp(-x/[2]))  - [0]*(1-exp(-(x-[4])/[2]))*(x>[4])", -5, 15);
  // bandfun->SetParName(0,"maxChg");
  // bandfun->SetParName(1,"minChg");
  // bandfun->SetParName(2,"tau");
  // bandfun->SetParName(4,"fallEdge");
  // bandfun->SetParameter(0,maxCur);
  // bandfun->SetParameter(1,0);
  // bandfun->SetParameter(2,bandtau);
  // bandfun->SetParameter(4,fallEdge);
  // bandfun->SetLineColor(kViolet);
  // bandfun->Draw("same");


  TF1 *multifun = new TF1("fint",finter,-5,15,0);
  multifun->SetLineColor(kRed);
  // multifun->Draw("same");
  // multifun->Draw();
  //
  // can->Update();
  // can->WaitPrimitive();
  // can->Update();
  // can->Clear();
  // delete(can);

  // tauGen=0;
  for (int32_t i=0; i<20; i++) {
    multifun->Draw("same");
    can->Update();
    can->WaitPrimitive();
    tauGen+=0.5;
    risefun->SetParameter(2,tauGen);
  }


  can->Clear();
  delete can;




  // TF1* fallfun = new TF1 ("fall","[0]*[2]/[3]*(1-exp(-x/[2])) - [0]*[2]/[3]*(1-exp(-(x-[4])/[2]))", 7, 15);
  // fallfun->SetParName(0,"maxChg");
  // fallfun->SetParName(1,"minChg");
  // fallfun->SetParName(2,"tau");
  // fallfun->SetParName(3,"tauEvap");
  // fallfun->SetParName(4,"fallEdge");
  // fallfun->SetParameter(0,maxCur);
  // fallfun->SetParameter(1,0);
  // fallfun->SetParameter(2,1);
  // fallfun->SetParameter(3,1);
  // fallfun->SetParameter(4,fallEdge);
  // fallfun->SetLineColor(kBlack);
  // fallfun->Draw("same");






  // //create the fit function
  // TF1* risefun = new TF1("rise","[4] + [5]/[6]*exp(-(x+[3])/[5])",1.5,7.5);
  // TF1* decayfun = new TF1("decaydouble","[0] + ([1]-[0])*exp(-(x+[3])/[2])",1.5,7.5);
  // TF1* spacechgfun = new TF1("spacechg","[1]*x + [0]",1.5,7.5);
  //
  // decayfun->SetParName(0,"chgInf");     //charge at infinity
  // decayfun->SetParName(1,"chgStart");   //starting charge
  // decayfun->SetParName(2,"tauRec");     //recombination
  // decayfun->SetParName(3,"xDisp");      //displacement in x direction
  // decayfun->FixParameter(0,0);
  // decayfun->SetParameter(1,7);
  // decayfun->SetParameter(2,10);
  // decayfun->SetParameter(3,-1);
  // decayfun->SetParLimits(1,0,15);
  // decayfun->SetParLimits(2,0,400);
  // decayfun->SetParLimits(3,-2,1);


  delete(dr);
  delete(app);
  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
