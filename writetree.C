#include <iostream>
#include "TFile.h"
#include <string>
#include "TTree.h"
#include "TH1F.h"
#include "TPad.h"
#include "TMath.h"
#include "TCanvas.h"

void writetree(std::string writename, std::string readname)
{
  std::cout << writename << std::endl;
  std::cout << readname << std::endl;
  TFile *f1 = new TFile(writename.c_str(),"recreate");
  TTree *t1 = new TTree("t1","a root tree"); //tree to write
  TFile *f2 = new TFile(readname.c_str(),"read");
  TTree *t2 = (TTree*)f2->Get("recoAndGenTreeR0p4EScheme"); //tree to read
  const Int_t nmaxJt=500;
  Int_t nJt_;
  Float_t genJtPt_[nmaxJt];
  Float_t genJtPhi_[nmaxJt];
  Float_t aj;
  Float_t deltaPhi;
  Float_t treew;
  Double_t random;
  Int_t ev;

  t1->Branch("aj",&aj,"aj/F"); //first branch
  t1->Branch("deltaPhi",&deltaPhi,"deltaPhi/F"); //second branch
  t1->Branch("ev",&ev,"ev/I"); //third branch
  t2->SetBranchAddress("nJt",&nJt_);
  t2->SetBranchAddress("genJtPt",genJtPt_);
  t2->SetBranchAddress("genJtPhi",genJtPhi_);
  // fill the tree
  Int_t nentry = (Int_t)t2->GetEntries();
  for(Int_t jI = 0; jI < nentry; jI++){
    t2->GetEntry(jI);

    Float_t tempLeadingJtPt_ = -999.;
    Float_t tempSubleadingJtPt_ = -999.;
    Float_t tempLeadingJtPhi_ = -999.;
    Float_t tempSubleadingJtPhi_ = -999.;

    //secondary loop
    for(Int_t kI = 0; kI < nJt_; kI++){ 
      //get leading jet
      if(genJtPt_[kI] > tempLeadingJtPt_){
	tempSubleadingJtPt_ = tempLeadingJtPt_;
	tempSubleadingJtPhi_ = tempLeadingJtPhi_;

	tempLeadingJtPt_ = genJtPt_[kI];
	tempLeadingJtPhi_ = genJtPhi_[kI];
      }
      //get subleading jet
      else if(genJtPt_[kI] >  tempSubleadingJtPt_){
	tempSubleadingJtPt_ = genJtPt_[kI];
	tempSubleadingJtPhi_ = genJtPhi_[kI];
      }
    }
  if(tempLeadingJtPt_ < 120.) continue;
  if(tempSubleadingJtPt_ < 30.) continue;

  deltaPhi = tempLeadingJtPhi_ - tempSubleadingJtPhi_;

  if(deltaPhi > TMath::Pi()) deltaPhi -= 2*TMath::Pi();
  else if(deltaPhi < -TMath::Pi()) deltaPhi += 2*TMath::Pi();

  if(TMath::Abs(deltaPhi) < 7.*TMath::Pi()/8.) continue;
  aj = (tempLeadingJtPt_ - tempSubleadingJtPt_)/(tempLeadingJtPt_ + tempSubleadingJtPt_);
    ev = jI;
    t1->Fill();
  }
  // save the Tree head; the file will be automatically closed
  // when going out of the function scope
  f1->cd();
  t1->Write();

}
