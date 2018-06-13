#include <iostream>
#include "TFile.h"
#include <string>
#include "TTree.h"
#include "TH1F.h"
#include "TPad.h"
#include "TMath.h"
#include "TCanvas.h"

void writetree(std::string filename)
{
  std::cout << filename << std::endl;
  TFile *f = new TFile(filename.c_str(),"recreate");
  TTree t1 = ("t1","a simple Tree with simple variables");
  const Int_t nmaxJt=500;
  Int_t nJt_;
  Float_t genJtPt_[nmaxJt];
  Float_t genJtPhi_[nmaxJt];
  Float_t genJtEta_[nmaxJt];
  Float_t aj;
  Float_t deltaPhi;
  Float_t treew;
  Double_t random;
  Int_t ev;
  t1->Branch("aj",&aj,"aj/F"); //first branch
  t1->Branch("deltaPhi",&deltaPhi,"deltaPhi/F"); //second branch
  t1->Branch("ev",&ev,"ev/I"); //third branch
  // fill the tree
  for(Int_t jI = 0; jI < nJt_; jI++){
    //get leading jet
    if(genJtPt_[jI] > tempLeadingJtPt_){
      tempSubleadingJtPt_ = tempLeadingJtPt_;
      tempSubleadingJtPhi_ = tempLeadingJtPhi_;

      tempLeadingJtPt_ = genJtPt_[jI];
      tempLeadingJtPhi_ = genJtPhi_[jI];
    }
    //get subleading jet
    else if(genJtPt_[jI] >  tempSubleadingJtPt_){
      tempSubleadingJtPt_ = genJtPt_[jI];
      tempSubleadingJtPhi_ = genJtPhi_[jI];
    }
  }

  if(tempLeadingJtPt_ < 120.) continue;
  if(tempSubleadingJtPt_ < 30.) continue;

  Float_t deltaPhi = tempLeadingJtPhi_ - tempSubleadingJtPhi_;

  if(deltaPhi > TMath::Pi()) deltaPhi -= 2*TMath::Pi();
  else if(deltaPhi < -TMath::Pi()) deltaPhi += 2*TMath::Pi();

  if(TMath::Abs(deltaPhi) < 7.*TMath::Pi()/8.) continue;

  Float_t aj = (tempLeadingJtPt_ - tempSubleadingJtPt_)/(tempLeadingJtPt_ + tempSubleadingJtPt_);

    ev = i;
    t1.Fill();
  }
  // save the Tree head; the file will be automatically closed
  // when going out of the function scope
  t1.Write();
}
