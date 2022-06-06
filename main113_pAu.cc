// main113.cc is a part of the PYTHIA event generator.
// Copyright (C) 2020 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Authors: Leif Lonnblad <leif.lonnblad@thep.lu.se>.

// Keywords: heavy ions; charged multiplicity; centrality;
// angantyr;

// This test program will generate Pb-Pb collisions at
// sqrt(S_NN)=2.76TeV using the Angantyr model for Heavy Ion
// collisions. The analysis will divide the event in centrality
// classes using the same observable as was used for p-Pb in the ATLAS
// analysis in arXiv:1508.00848 [hep-ex] (see main112.cc). The
// centrality classes are same as in the ALICE analysis in
// arXiv:1012.1657 [nucl-ex] although the actual observable used is
// not the same. Histograms of multiplicity distributions are measured
// for each centrality percentile.

// Note that heavy ion collisions are computationally quite CPU
// intensive and generating a single event will take around a second
// on a reasonable desktop. To get reasonable statistics, this program
// will take a couple of hours to run.

#include <TH1D.h>
#include <TFile.h>
#include <TH2D.h>
#include "Pythia8/Pythia.h"


// You need to include this to get access to the HIInfo object for
// HeavyIons.
#include "Pythia8/HeavyIons.h"

using namespace Pythia8;
using namespace std;

int main(int argc, char* argv[]) {


  string str_seed = argv[1];
  string n_seeds = argv[3];
  string projectile = argv[6];
  string target     = argv[7];
  int n_seeds_int = std::stoi(n_seeds);
  string nEv = argv[4];
  int nEvents = std::stoi(nEv);
  string iftrig_str = argv[8];
  int iftrig = stoi(iftrig_str);
  string N_opt_str = argv[9];
  int N_opt = stoi(N_opt_str);
  cout << str_seed << "  " << n_seeds <<"  "<<nEv<<"  "<< N_opt <<endl;
  cout << projectile << "  " << target <<"  "<<argv[5]<<"  "<< argv[2] <<endl;
  for (int i = 0; i < N_opt; ++i) {
      cout<<argv[10+i]<<endl;
  }


  int params[] = {500, 0, 500, 200, 0, 20};
  //double pt_bins[13] = {1.4, 1.7, 1.9, 2.1, 2.3, 2.6, 2.9, 3.4, 4.0, 4.5, 5., 6.5, 8.5};
  TH2D *pt_kstar = new TH2D("pt_kstar", "pt_kstar", params[0], params[1], params[2], params[3], params[4], params[5]);
  TH2D *pt_phi = new TH2D("pt_phi", "pt_phi", params[0], params[1], params[2], params[3], params[4], params[5]);
  TH2D *pt_pi = new TH2D("pt_pi", "pt_pi", params[0], params[1], params[2], params[3], params[4], params[5]);
  TH1D *hist = new TH1D("hist", "hist", 100, 0, 10);
  TH1D* sumch = new TH1D("sumch", "sumch", 500, 0, 500);
  TH2D* nwhist = new TH2D("nwhist", "nwhist", 500, 0, 500,100,0,100);
  TH2D* ncollhist = new TH2D("ncollhist", "ncollhist", 500, 0, 500,100,0,100);
  TH2D* ncollhist1 = new TH2D("ncollhist1", "ncollhist1", 500, 0, 500,100,0,100);
  TH2D* h_weights_centrality = new TH2D("h_weights_centrality", "h_weights_centrality", params[0], params[1], params[2], 5, 0, 5);
  TH1D* nchhist = new TH1D("nchhist", "nchhist", 500, 0, 500);
  TH1D* ncoll2hist = new TH1D("ncoll2hist", "ncoll2hist", 500, 0, 500);
  TH2D* y_charg = new TH2D("y_charg", "y_charg", 1000, -5, 5,200,0,20);
  TH2D* y_phi = new TH2D("y_phi", "y_phi", 1000, -5, 5,200,0,20);
  TH2D* y_pi = new TH2D("y_pi", "y_pi", 1000, -5, 5,200,0,20);
  TH1D* sigmahist = new TH1D("sigmahist", "sigmahist", 100000, 0, 10000);

  Pythia pythia;

  pythia.readString("Random:setSeed = on");
  pythia.readString("Random:seed = " + str_seed);

  /// Setup the beams.
  pythia.readString("Beams:idA = " + projectile); // The coper ion
  pythia.readString("Beams:idB = " + target); // The lead ions.
  pythia.readString("Beams:eCM = 200.");
  pythia.readString("Beams:frameType = 1");

    if (stoi(target)!=2212)
    {
        // Initialize the Angantyr model to fit the total and semi-includive
        // cross sections in Pythia within some tolerance.
        pythia.readString("HeavyIon:SigFitErr = 0.02,0.02,0.1,0.05,0.05,0.0,0.1,0.0");
        // These parameters are typicall suitable for sqrt(S_NN)=5TeV
        pythia.readString("HeavyIon:SigFitDefPar = 9.82,1.69,0.29,0.0,0.0,0.0,0.0,0.0");
        // A simple genetic algorithm is run for 20 generations to fit the
        // parameters.
        pythia.readString("HeavyIon:SigFitNGen = 0");
    }


  for (int i = 0; i < N_opt; ++i) {
      cout<<argv[10+i]<<endl;
      pythia.readString(argv[10+i]);
  }


    string file_name;
    file_name.append("");
    file_name.append(argv[5]);
    file_name.append("_");
    file_name.append(argv[2]);
    file_name.append(".root");

    TFile *outFile = TFile::Open(file_name.c_str(), "RECREATE");


  // Initialise Pythia.
  pythia.init();

  // Loop over events.

  double sumw = 0.0;
  double sigmaALL = 0.0;


  for ( int iEvent = 0; iEvent < nEvents; ++iEvent ) {
    if (!pythia.next()) continue;
    if ((iEvent+1) % 10000 == 0) cout << "N_Event: " << iEvent << endl;

    double summcharge = 0.0;
    int nch = 0;
    bool fwd = false, bkw = false;
    for (int i = 0; i < pythia.event.size(); ++i) {
      Particle &p = pythia.event[i];
      if (p.isFinal() && p.isCharged()) {
        double eta = p.eta();
        if ( fabs(eta) > 3.1 && fabs(eta) < 4. ){
          summcharge += fabs(p.charge());
        }
        if ( fabs(eta) < 0.5 ){
            nch++;
        }
        if (eta > 3.1 && eta < 4.) fwd = true;
        if (eta < -3.1 && eta > -4.) bkw = true;
      }
    }
    if((!fwd || !bkw)&&iftrig==1) continue;

    nchhist->Fill(nch);

    // Keep track of the sum of waights
    double weight = pythia.info.weight();
    sumw += weight;
    sigmaALL += pythia.info.sigmaGen();
      sigmahist->Fill(pythia.info.sigmaGen(),weight);


    // Also fill the number of (absorptively and diffractively)
    // wounded nucleaons.
      int nw = 2, ncoll = 1, ncoll1 = 1;
      if (stoi(target)!=2212)
    {
        nw = pythia.info.hiInfo->nAbsTarg() +
              pythia.info.hiInfo->nDiffTarg() +
              pythia.info.hiInfo->nAbsProj() +
              pythia.info.hiInfo->nDiffProj();
            ///ncoll calc by Norbert
        ncoll = pythia.info.hiInfo->nCollNDTot();
        ncoll1 =pythia.info.hiInfo->nAbsProj() +
                pythia.info.hiInfo->nDiffProj() +
                pythia.info.hiInfo->nAbsTarg() +
                pythia.info.hiInfo->nDiffTarg() -
                pythia.info.hiInfo->nCollND() -
                pythia.info.hiInfo->nCollDD();
    }

    sumch->Fill(summcharge, weight);
    nwhist->Fill(summcharge,nw,weight);

    ncollhist->Fill(summcharge,ncoll,weight);
    ncollhist1->Fill(summcharge,ncoll1,weight);
    ncoll2hist->Fill(ncoll1,weight);

    for (int i = 0; i < pythia.event.size(); ++i) {
      Particle &p = pythia.event[i];
      if(fabs(p.eta()) < 0.5)
      {
          if (p.isFinal() && fabs(p.id()) == 2212) {
              pt_kstar->Fill(summcharge, p.pT(), weight);
              hist->Fill(p.pT());
          }

          if (fabs(p.id()) == 211) {
              pt_pi->Fill(summcharge, p.pT(), weight);
          }

          if (p.id() == 333) {
              pt_phi->Fill(summcharge, p.pT(), weight);
          }
      }
        if (p.isFinal() && fabs(p.id()) == 2212) {
            y_charg->Fill(p.eta(), p.pT(), weight);
        }

        if (fabs(p.id()) == 211) {
            y_pi->Fill(p.eta(), p.pT(), weight);
        }

        if (p.id() == 333) {
            y_phi->Fill(p.eta(), p.pT(), weight);
        }
    }
  }


  sumch->Write();
  pt_kstar->Write();
  pt_phi->Write();
  pt_pi->Write();
  hist->Write();
  nwhist->Write();
  nchhist->Write();
  ncollhist->Write();
  ncollhist1->Write();
  ncoll2hist->Write();
  y_charg->Write();
  y_phi->Write();
  y_pi->Write();
  sigmahist->Write();

  outFile->Close();

    std::cout << "SIGMA = " << sigmaALL/sumw*1000 <<" "<<sumw << std::endl;

  // And we're done!
  return 0;
}
