////author: Grecia Castelazo
#include <iostream>
#include "TFile.h"
#include <string>
#include "TTree.h"
#include "TH1F.h"
#include "TPad.h"
#include "TMath.h"

void readtree(std::string filename)
{
  std::cout << filename << std::endl;
  TFile *f = new TFile(filename.c_str(),"read");
  TTree *t1 = (TTree*)f->Get("recoAndGenTreeR0p4EScheme");

  const Int_t nmaxJt=500;
  Int_t nJt_;
  Float_t genJtPt_[nmaxJt];
  Float_t genJtPhi_[nmaxJt];
  Float_t genJtEta_[nmaxJt];
  t1->SetBranchAddress("nJt",&nJt_);
  t1->SetBranchAddress("genJtPt",genJtPt_);
  t1->SetBranchAddress("genJtPhi",genJtPhi_);
  t1->SetBranchAddress("genJtEta",genJtEta_);

  //create four  histograms
  TH1F* hist_p = new TH1F("hnJt", "nJt distribution",20,0,20);
  TH1F* hist_p2 = new TH1F("hgenJtPt", "JtPt distribution",100,0,100);
  TH1F* hist_p3 = new TH1F("hgenJtPhi", "JtPhi distribution",100,-4,4);
  TH1F* hist_p4 = new TH1F("hgenJtEta", "JtEta distribution",100,-4,4);
  TH1F* hist_p5 = new TH1F("aj_h", ";A_{J};Counts", 100, 0, 1);

  //read all entries and fill the histograms
  Int_t nentries = (Int_t)t1->GetEntries();
  for (Int_t i=0; i<nentries; i++) {
    t1->GetEntry(i);
    hist_p->Fill(nJt_);

    Float_t tempLeadingJtPt_ = -999.;
    Float_t tempSubleadingJtPt_ = -999.;

    Float_t tempLeadingJtPhi_ = -999.;
    Float_t tempSubleadingJtPhi_ = -999.;

    for(Int_t jI = 0; jI < nJt_; jI++){
      hist_p2->Fill(genJtPt_[jI]);
      hist_p3->Fill(genJtPhi_[jI]);
      hist_p4->Fill(genJtEta_[jI]);

      if(genJtPt_[jI] > tempLeadingJtPt_){
	tempSubleadingJtPt_ = tempLeadingJtPt_;
	tempSubleadingJtPhi_ = tempLeadingJtPhi_;

	tempLeadingJtPt_ = genJtPt_[jI];
	tempLeadingJtPhi_ = genJtPhi_[jI];
      }
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

    hist_p5->Fill(aj);
  }

  TFile *w = new TFile("outfile.root","recreate");
  hist_p->Write();
  hist_p2->Write();
  hist_p3->Write();
  hist_p4->Write();
  hist_p5->Write("", TObject::kOverwrite);

  w->Close();
  delete w;

  delete hist_p;
  delete hist_p2;
  delete hist_p3;
  delete hist_p4;
  delete hist_p5;

  f->Close();
  delete f;

  return;
}
