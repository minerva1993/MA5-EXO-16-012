#include <iostream>
#include <vector>

#include "SampleAnalyzer/User/Analyzer/tutorial2.h"

using namespace MA5;
using namespace std;

// -----------------------------------------------------------------------------
// Initialize
// function called one time at the beginning of the analysis
// -----------------------------------------------------------------------------
bool tutorial2::Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters){
  cout << "BEGIN Initialization" << endl;
  // initialize variables, histos
  cout << "END   Initialization" << endl;

  nPhoton = 0;
  nEvent = 0;
  nSignal = 0;

  //Declaring signal area
  Manager()->AddRegionSelection("Hgg");

  /* 1. Asymmetric Transverse Energy Threshold */
  Manager()->AddCut("Photon1.pt() > 30 GeV, Photon2.pt() > 18 GeV", "Hgg");
  /* 2. Loose Photon ID (H/E < 0.1)  */
  Manager()->AddCut("Hadronic/Electronic < 0.1","Hgg");
  /* 3. Loose Photon Isolation (?) */
  Manager()->AddCut("isolation","Hgg");
  /* 4. EB(ECAL Barrel) :|eta| < 1.44, EE(ECAL Endcap) : 1.566 < |eta| < 2.5 */
  // --> HOW TO ADDCUT?
  /* 5. |dPhi(Diphoton, MET)| >= 2.1  */
  Manager()->AddCut("dPhi(Diphoton,MET) > 2.1 ","Hgg");
  /* 6. min(|dPhi(jet, MET)|) > 0.5 */
  Manager()->AddCut("min(dPhi(jet,MET)) > 0.5", "Hgg");
  /* 7. Photon1.pT/Diphoton.M > 0.5, Photon2.pT/Diphoton.M > 0.25 */
  Manager()->AddCut("photon1.pT/diphoton.m > 0.5 && photon2.pT/diphoton.m > 0.25");
  /* 8. Diphoton.pT > 90 GeV , MET > 105 GeV */
  Manager()->AddCut("diphoton.pT > 90 GeV && MET > 105 GeV","Hgg");
  /* 9. 120 GeV < Diphoton.m < 130 GeV */
  Manager()->AddCut("120 GeV < Diphoton.m() < 130 GeV","Hgg");
  
  return true;
}

// -----------------------------------------------------------------------------
// Finalize
// function called one time at the end of the analysis
// -----------------------------------------------------------------------------
void tutorial2::Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files){
  cout << "BEGIN Finalization" << endl;
  cout << "NUMBER OF SINGLE PHOTON EVENT : " << nPhoton << endl;
  cout << "NUMBER OF DIPHOTON EVENT : " << nEvent << endl;
  cout << " Number of signal : " << nSignal << endl;
  // saving histos
  cout << "END   Finalization" << endl;
}

// -----------------------------------------------------------------------------
// Execute
// function called each time one event is read
// -----------------------------------------------------------------------------
bool tutorial2::Execute(SampleFormat& sample, const EventFormat& event){
  // ***************************************************************************
  // Example of analysis with reconstructed objects
  // Concerned samples : 
  //   - LHCO samples
  //   - LHE/STDHEP/HEPMC samples after applying jet-clustering algorithm
  // ***************************************************************************

  if (event.rec()==0) return true;
  //cout << "---------------NEW EVENT-------------------" << endl;

  vector<const RecPhotonFormat *> v_signalPhotons;
  vector<const RecJetFormat *> v_signalJets;
  vector<const RecLeptonFormat *> v_signalLeptons;

  MALorentzVector pTmiss = event.rec()->MET().momentum();
  double MET = pTmiss.Pt();

  for(unsigned int i=0; i<event.rec()->photons().size(); ++i){
    const RecJetFormat * jet = &(event.rec()->jets()[i]);
    v_signalJets.push_back(jet);
  }

  //Diphoton selection
  /* 1. Asymmetric transverse energy threshold */
  if( event.rec()->photons().size() != 0)
    cout << "PHOTON SIZE : " << event.rec()->photons().size() << endl;
  if( event.rec()->photons().size() == 1) ++nPhoton;
  if( event.rec()->photons().size() >= 2) ++nEvent;
  for(unsigned int i=0; i<event.rec()->photons().size(); ++i){
    const RecPhotonFormat * myphoton1 = &(event.rec()->photons()[i]);
    MALorentzVector photon1;
    photon1.SetPtEtaPhiE(myphoton1->pt(), myphoton1->eta(), myphoton1->phi(), myphoton1->e());
    for(unsigned int j=0; j<event.rec()->photons().size(); ++j){
      if( i == j ) continue;
      const RecPhotonFormat * myphoton2 = &(event.rec()->photons()[j]);
      MALorentzVector photon2;
      photon2.SetPtEtaPhiE(myphoton2->pt(), myphoton2->eta(), myphoton2->phi(), myphoton2->e());
      cout << "Diphoton InvMass : " <<(photon1 + photon2).M() << endl;
      if( (photon1 + photon2).M() > 95.0 ){
        v_signalPhotons.push_back(myphoton1);
	v_signalPhotons.push_back(myphoton2);
      }
    }
  }

  std::sort(v_signalPhotons.begin(),v_signalPhotons.end());
  vector<const RecPhotonFormat *>::iterator v_remove = std::unique(v_signalPhotons.begin(),v_signalPhotons.end());
  if( v_remove != v_signalPhotons.end() )
    v_signalPhotons.erase(v_remove, v_signalPhotons.end());

  /* 2. Loose Photon ID */
  /* 3. Loose Photon Isolation*/
  for(int i = v_signalPhotons.size()-1 ; i>=0; --i){
    const RecPhotonFormat * photon = v_signalPhotons[i];
    double myPhotonPt = photon->pt();
    double chargePt = PHYSICS->Isol->eflow->sumIsolation(photon, event.rec(), 0.3, 0., IsolationEFlow::TRACK_COMPONENT);
    double neutralPt = PHYSICS->Isol->eflow->sumIsolation(photon, event.rec(), 0.3, 0., IsolationEFlow::NEUTRAL_COMPONENT);
    double photonPt = PHYSICS->Isol->eflow->sumIsolation(photon, event.rec(), 0.3, 0., IsolationEFlow::PHOTON_COMPONENT);
    double photonplt = photonPt-myPhotonPt;
    double photonID = photon->HEoverEE();
    if(chargePt > 1.5 || neutralPt > (1.0+0.04*myPhotonPt) || photonplt > (0.7+0.005*myPhotonPt) || photonID > 0.1 ){
      v_signalPhotons.erase(v_signalPhotons.begin()+i);
    }
  }
  if( !Manager()->ApplyCut(v_signalPhotons.size() > 0, "isolation")) return true;
  
  /* 4. EB(ECAL Barrel) :|eta| < 1.44, EE(ECAL Endcap) : 1.566 < |eta| < 2.5 */
  //RecPhotonFormat * diPhoton = v_signalPhotons[0]+v_signalPhotons[1];
  MALorentzVector diPhoton = {0.0,0.0,0.0,0.0};
  if( v_signalPhotons.size() >= 2)
    diPhoton.SetPtEtaPhiE(v_signalPhotons[0]->pt()+v_signalPhotons[1]->pt(),
	v_signalPhotons[0]->eta()+v_signalPhotons[1]->eta(),
	v_signalPhotons[0]->phi()+v_signalPhotons[1]->phi(),
	v_signalPhotons[0]->e()+v_signalPhotons[1]->e());
  else return true;

  /* 5. |dPhi(Diphoton, MET)| >= 2.1 */
  if( !Manager()->ApplyCut(diPhoton.DeltaPhi(pTmiss) > 2.1, "dPhi(Diphoton,Met) > 2.1")) return true;
  
  /* 6. min(|dPhi(Jet, MET)|) > 0.5 */
  std::sort(v_signalJets.begin(),v_signalJets.end());
  for(vector<const RecJetFormat *>::iterator v_itr = v_signalJets.begin(); v_itr != v_signalJets.end(); ++v_itr){
    const RecJetFormat * myjet = *v_itr;
    //MALorentsVector jet;
    //jet.SetPtEtaPhiE(myjet.pt(),myjet.eta(),myjet.phi(),myjet.e());
    if( !Manager()->ApplyCut(myjet->dphi_0_pi(pTmiss) > 0.5,"min(dPhi(jet,MET)) > 0.5")) return true;
  }

  /* 7. Photon1.pT/Diphoton.M > 0.5 Photon2.pT/Diphoton.M > 0.25 */
  //RecPhotonFormat * photon1 = v_signalPhotons[0];
  //RecPhotonFormat * photon2 = v_signalPhotons[1];
  if( !Manager()->ApplyCut((v_signalPhotons[0]->pt()/diPhoton.M()) > 0.5 && (v_signalPhotons[1]->pt()/diPhoton.M()) > 0.25, "photon1.pT/diphoton.m > 0.5 && photon2.pT/diphoton.m > 0.25")) return true;
  
  /* 8. Diphoton.pT > 90 GeV, MET > 105 GeV */
  if( !Manager()->ApplyCut(diPhoton.Pt() > 90.0 && MET > 105.0, "diphoton.pT > 90 GeV && MET 105 GeV")) return true;

  /* 9. 120 GeV < Diphoton.m < 130 GeV */
  if( !Manager()->ApplyCut(diPhoton.M() > 120.0 && diPhoton.M() < 130.0, "120 GeV < Diphoton.m() < 130 GeV")) return true;

  ++nSignal;
  return true;
}

