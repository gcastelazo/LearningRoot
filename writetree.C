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
  t1->Branch("aj",&aj,"aj/F");
  t1->Branch("deltaPhi",&deltaPhi,"deltaPhi/F");
  t1->Branch("ev",&ev,"ev/I");
  // fill the tree
  for (Int_t i=0; i<10000; i++){
    gRandom->Rannor(aj,deltaPhi);
    trew = aj*aj + deltaPhi*deltaPhi;
    random = gRandom->Rndm();
    ev = i;
    t1.Fill();
  }
  // save the Tree head; the file will be automatically closed
  // when going out of the function scope
  t1.Write();
}
