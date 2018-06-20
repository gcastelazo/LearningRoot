// 1D histogram first data file e+e-

#include <iostream>
#include "TFile.h"
#include <string>
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"

void exercise06192018(std::string filename)
{
  std::cout << filename << std::endl;
  TFile *f = new TFile(filename.c_str(),"read");
  TTree *t1 = (TTree*)f->Get("t");
  
  Int_t EventNo_;
  // get branches
  t1->SetBranchAddress("EventNo",&EventNo_);
  
  // create histogram
  TH1F* hist_p0 = new TH1F("hEventNo","EventNo distribution",20,0,20);

  hist_p0->SetMarkerColorAlpha(kRed,0.35);

  // read all entries and fill the histograms
  Int_t n_entries = (Int_t)t1->GetEntries();
  for (Int_t i=0; i<n_entries; i++) {
    t1->GetEntry(i);
    hist_p0->Fill(EventNo_);
  }

  TFile *w = new TFile("outfile.root","recreate");
  hist_p0->Write("", TObject::kOverwrite);

  w->Close();
  delete w;
  
  f->Close();
  delete f;

  return;
}
