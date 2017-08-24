#include "SampleAnalyzer/User/Analyzer/cms_sus_16_033.h"
using namespace MA5;
using namespace std;

// Define =====================================================
// =====some utilities to be used in the analysis ======
// ========================================================

double calcSumPt(const RecLeptonFormat* mylepton, double coneSize)
{
  double sumPt_ = 0;
  for(unsigned int c=0; c<mylepton->isolCones().size(); c++)
    {
      if(!(fabs(mylepton->isolCones()[c].deltaR() - coneSize)<0.001)) continue;
      sumPt_ = mylepton->isolCones()[c].sumPT();
    }
  return sumPt_;
}

double deltaR_lepton(double pt)
{

      if (pt > 200.) {
          return .05;
      } else if ( pt > 50 ) {
          return 10./pt;
      }
      // Default radius:
      return 0.2;
}

// -----------------------------------------------------------------------------
// Initialize
// function called one time at the beginning of the analysis
// -----------------------------------------------------------------------------
bool cms_sus_16_033::Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters)
{
  cout << "BEGIN Initialization" << endl;
  // initialize variables, histos
  cout << "END   Initialization" << endl;
  // Information on the analysis, authors, ...
  // VERY IMPORTANT FOR DOCUMENTATION, TRACEABILITY, BUG REPORTS
  INFO << "        <><><><><><><><><><><><><><><><><><><><><><><><>" << endmsg;
  INFO << "        <>    Analysis: CMS-SUS-16-033                <>" << endmsg;
  INFO << "        <>    Multijet+MHT @sqrt(s) = 13 TeV, 35.9 fb^-1 luminosity  <>" << endmsg;
  INFO << "        <>    Recasted by: F> Ambrogi, J. Sonneveld <>" << endmsg;
  INFO << "        <>    Contact: federico.ambrogi@cern.ch, jory.sonneveld@desy.de           <>" << endmsg;
  INFO << "        <>    Based on MadAnalysis 5 v1.4 and above        <>" << endmsg;
  INFO << "        <>    For more information, see               <>" << endmsg;
  INFO << "        <>    http://madanalysis.irmp.ucl.ac.be/wiki/PublicAnalysisDatabase <>" << endmsg;
  INFO << "        <>    Validated on cutflow given at " << endmsg;
  INFO << "        <>    Notes: " << endmsg;
  INFO << "        <><><><><><><><><><><><><><><><><><><><><><><><>" << endmsg;

// =====Declare the 12 signal regions in this analysis=====
  Manager()->AddRegionSelection( "Nbjets0, Njets>=2, HT>500, MHT>500");
  Manager()->AddRegionSelection( "Nbjets0, Njets>=3, HT>1500, MHT>750");
  Manager()->AddRegionSelection( "Nbjets0, Njets>=5, HT>500, MHT>500");
  Manager()->AddRegionSelection( "Nbjets0, Njets>=5, HT>1500, MHT>750");
  Manager()->AddRegionSelection( "Nbjets0, Njets>=9, HT>1500, MHT>750");

  Manager()->AddRegionSelection( "Nbjets>=1, Njets>=3, HT>750, MHT>750");
  Manager()->AddRegionSelection( "Nbjets>=1, Njets>=5, HT>750, MHT>750");
  Manager()->AddRegionSelection( "Nbjets>=1, Njets>=7, HT>300, MHT>300");


  Manager()->AddRegionSelection( "Nbjets>=2, Njets>=2, HT>500, MHT>500");
  Manager()->AddRegionSelection( "Nbjets>=2, Njets>=5, HT>1500, MHT>750");

  Manager()->AddRegionSelection( "Nbjets>=3, Njets>=5, HT>500, MHT>500");
  Manager()->AddRegionSelection( "Nbjets>=3, Njets>=9, HT>750, MHT>750");


// =====Define the 12 signal regions in this analysis=====
  //Manager()->AddCut(  "SR1", "Nbjets0, Njets>=2, HT>500, MHT>500");
  //Manager()->AddCut(  "SR2", "Nbjets0, Njets>=3, HT>1500, MHT>750");
  //Manager()->AddCut(  "SR3", "Nbjets0, Njets>=5, HT>500, MHT>500");
  //Manager()->AddCut(  "SR4", "Nbjets0, Njets>=5, HT>1500, MHT>750");
  //Manager()->AddCut(  "SR5", "Nbjets0, Njets>=9, HT>1500, MHT>750");

  //Manager()->AddCut(  "SR7", "Nbjets>=1, Njets>=3, HT>750, MHT>750");
  //Manager()->AddCut( "SR12", "Nbjets>=1, Njets>=5, HT>750, MHT>750");
  //Manager()->AddCut( "SR11", "Nbjets>=1, Njets>=7, HT>300, MHT>300");


  //Manager()->AddCut(  "SR6",  "Nbjets>=2, Njets>=2, HT>500, MHT>500");
  //Manager()->AddCut(  "SR9",  "Nbjets>=2, Njets>=5, HT>1500, MHT>750");

  //Manager()->AddCut(  "SR8",  "Nbjets>=3, Njets>=5, HT>500, MHT>500");
  //Manager()->AddCut( "SR10",  "Nbjets>=3, Njets>=9, HT>750, MHT>750");

//=============Baseline Cuts=================//
 Manager()->AddCut("Njets>=2");
 Manager()->AddCut("HT>300 GeV");
 Manager()->AddCut("MHT>300 GeV");
 Manager()->AddCut("muon_veto");
 Manager()->AddCut("muon_isoTrack_veto");
 Manager()->AddCut("electron_veto");
 Manager()->AddCut("electron_isoTrack_veto");
 Manager()->AddCut("hadron_isoTrack_veto");
 Manager()->AddCut("dphi_j1_mht > 0.5");
 Manager()->AddCut("dphi_j2_mht > 0.5");
 Manager()->AddCut("dphi_j3_mht > 0.3");
 Manager()->AddCut("dphi_j4_mht > 0.3");

//======Signal Region Cuts====AggregatedRegions==========


 //Nbjet regions
  string SR_NbJet0[] =
    {
      "Nbjets0, Njets>=2, HT>500, MHT>500",
      "Nbjets0, Njets>=3, HT>1500, MHT>750",
      "Nbjets0, Njets>=5, HT>500, MHT>500",
      "Nbjets0, Njets>=5, HT>1500, MHT>750",
      "Nbjets0, Njets>=9, HT>1500, MHT>750",

    };
  Manager()->AddCut("Nbjets0",SR_NbJet0);

  string SR_NbJet1[] =
    {
      "Nbjets>=1, Njets>=3, HT>750, MHT>750",
      "Nbjets>=1, Njets>=5, HT>750, MHT>750",
      "Nbjets>=1, Njets>=7, HT>300, MHT>300",
    };
  Manager()->AddCut("NbJets>=1",SR_NbJet1);

  string SR_NbJet2[] =
    {
      "Nbjets>=2, Njets>=2, HT>500, MHT>500",
      "Nbjets>=2, Njets>=5, HT>1500, MHT>750",
    };
  Manager()->AddCut("NbJets>=2",SR_NbJet2);

  string SR_NbJet3[] =
    {
      "Nbjets>=3, Njets>=5, HT>500, MHT>500",
      "Nbjets>=3, Njets>=9, HT>750, MHT>750",
    };
  Manager()->AddCut("NbJets>=3",SR_NbJet3);

 //Njet regions
  string SR_NJet2[] =
    {
      "Nbjets0, Njets>=2, HT>500, MHT>500",
      "Nbjets>=2, Njets>=2, HT>500, MHT>500",

    };
  Manager()->AddCut("NJets>=2",SR_NJet2);
  string SR_NJet3[] =
    {
      "Nbjets0, Njets>=3, HT>1500, MHT>750",
      "Nbjets>=1, Njets>=3, HT>750, MHT>750",

    };
  Manager()->AddCut("NJets>=3",SR_NJet3);

  string SR_NJet5[] =
    {
      "Nbjets0, Njets>=5, HT>500, MHT>500",
      "Nbjets0, Njets>=5, HT>1500, MHT>750",
      "Nbjets>=1, Njets>=5, HT>750, MHT>750",
      "Nbjets>=2, Njets>=5, HT>1500, MHT>750",
      "Nbjets>=3, Njets>=5, HT>500, MHT>500",
    };
  Manager()->AddCut("NJets>=5",SR_NJet5);

  string SR_NJet7[] =
    {
      "Nbjets>=1, Njets>=7, HT>300, MHT>300",
    };
  Manager()->AddCut("NJets>=7",SR_NJet7);

  string SR_NJet9[] =
    {
      "Nbjets0, Njets>=9, HT>1500, MHT>750",
      "Nbjets>=3, Njets>=9, HT>750, MHT>750",
    };
  Manager()->AddCut("NJets>=9",SR_NJet9);

  //HT regions
  string SR_HTgt300[] =
    {
      "Nbjets>=1, Njets>=7, HT>300, MHT>300",
    };
  Manager()->AddCut("HT>300",SR_HTgt300);
  string SR_HTgt500[] =
    {
      "Nbjets0, Njets>=2, HT>500, MHT>500",
      "Nbjets0, Njets>=5, HT>500, MHT>500",
      "Nbjets>=2, Njets>=2, HT>500, MHT>500",
      "Nbjets>=3, Njets>=5, HT>500, MHT>500",
    };
  Manager()->AddCut("HT>500",SR_HTgt500);

  string SR_HTgt750[] =
    {
      "Nbjets>=1, Njets>=3, HT>750, MHT>750",
      "Nbjets>=1, Njets>=5, HT>750, MHT>750",
      "Nbjets>=3, Njets>=9, HT>750, MHT>750",
    };
  Manager()->AddCut("HT>750",SR_HTgt750);
  string SR_HTgt1500[] =
    {
      "Nbjets0, Njets>=3, HT>1500, MHT>750",
      "Nbjets0, Njets>=5, HT>1500, MHT>750",
      "Nbjets0, Njets>=9, HT>1500, MHT>750",
      "Nbjets>=2, Njets>=5, HT>1500, MHT>750",
    };
  Manager()->AddCut("HT>1500",SR_HTgt1500);

  //HT regions
  string SR_MHTgt300[] =
    {
      "Nbjets>=1, Njets>=7, HT>300, MHT>300",
    };
  Manager()->AddCut("MHT>300",SR_MHTgt300);
  string SR_MHTgt500[] =
    {
      "Nbjets0, Njets>=2, HT>500, MHT>500",
      "Nbjets0, Njets>=5, HT>500, MHT>500",
      "Nbjets>=2, Njets>=2, HT>500, MHT>500",
      "Nbjets>=3, Njets>=5, HT>500, MHT>500",
    };
  Manager()->AddCut("MHT>500",SR_MHTgt500);

  string SR_MHTgt750[] =
    {
      "Nbjets0, Njets>=3, HT>1500, MHT>750",
      "Nbjets0, Njets>=5, HT>1500, MHT>750",
      "Nbjets0, Njets>=9, HT>1500, MHT>750",
      "Nbjets>=1, Njets>=3, HT>750, MHT>750",
      "Nbjets>=1, Njets>=5, HT>750, MHT>750",
      "Nbjets>=2, Njets>=5, HT>1500, MHT>750",
      "Nbjets>=3, Njets>=9, HT>750, MHT>750",
    };
  Manager()->AddCut("MHT>750",SR_MHTgt750);


  //histograms
  Manager()->AddHisto("NJets_METCleaning",16,-0.5,15.5);
  Manager()->AddHisto("NJets_NoElectron",16,-0.5,15.5);
  Manager()->AddHisto("NJets_NoMuon",16,-0.5,15.5);
  Manager()->AddHisto("NJets_NoElectronIsoTrack",16,-0.5,15.5);
  Manager()->AddHisto("NJets_NoMuonIsoTrack",16,-0.5,15.5);
  Manager()->AddHisto("NJets_NoHadronIsoTrack",16,-0.5,15.5);
  Manager()->AddHisto("NJets_NJets>=2",16,-0.5,15.5);
  Manager()->AddHisto("NJets_HT>300",16,-0.5,15.5);
  Manager()->AddHisto("NJets_MHT>300",16,-0.5,15.5);
  Manager()->AddHisto("NJets_MinDeltaPhi1",16,-0.5,15.5);
  Manager()->AddHisto("NJets_MinDeltaPhi2",16,-0.5,15.5);
  Manager()->AddHisto("NJets_MinDeltaPhi3",16,-0.5,15.5);
  Manager()->AddHisto("NJets_MinDeltaPhi4",16,-0.5,15.5);

  Manager()->AddHisto("NbJets_METCleaning",16,-0.5,15.5);
  Manager()->AddHisto("NbJets_NoElectron",16,-0.5,15.5);
  Manager()->AddHisto("NbJets_NoMuon",16,-0.5,15.5);
  Manager()->AddHisto("NbJets_NoElectronIsoTrack",16,-0.5,15.5);
  Manager()->AddHisto("NbJets_NoMuonIsoTrack",16,-0.5,15.5);
  Manager()->AddHisto("NbJets_NoHadronIsoTrack",16,-0.5,15.5);
  Manager()->AddHisto("NbJets_NJets>=2",16,-0.5,15.5);
  Manager()->AddHisto("NbJets_HT>300",16,-0.5,15.5);
  Manager()->AddHisto("NbJets_MHT>300",16,-0.5,15.5);
  Manager()->AddHisto("NbJets_MinDeltaPhi1",16,-0.5,15.5);
  Manager()->AddHisto("NbJets_MinDeltaPhi2",16,-0.5,15.5);
  Manager()->AddHisto("NbJets_MinDeltaPhi3",16,-0.5,15.5);
  Manager()->AddHisto("NbJets_MinDeltaPhi4",16,-0.5,15.5);

  Manager()->AddHisto("HT_METCleaning",40,0,4000);
  Manager()->AddHisto("HT_NoElectron",40,0,4000);
  Manager()->AddHisto("HT_NoMuon",40,0,4000);
  Manager()->AddHisto("HT_NoElectronIsoTrack",40,0,4000);
  Manager()->AddHisto("HT_NoMuonIsoTrack",40,0,4000);
  Manager()->AddHisto("HT_NoHadronIsoTrack",40,0,4000);
  Manager()->AddHisto("HT_NJets>=2",40,0,4000);
  Manager()->AddHisto("HT_HT>300",40,0,4000);
  Manager()->AddHisto("HT_MHT>300",40,0,4000);
  Manager()->AddHisto("HT_MinDeltaPhi1",40,0,4000);
  Manager()->AddHisto("HT_MinDeltaPhi2",40,0,4000);
  Manager()->AddHisto("HT_MinDeltaPhi3",40,0,4000);
  Manager()->AddHisto("HT_MinDeltaPhi4",40,0,4000);

  Manager()->AddHisto("MHT_METCleaning",30,0,1500);
  Manager()->AddHisto("MHT_NoMuon",30,0,1500);
  Manager()->AddHisto("MHT_NoElectron",30,0,1500);
  Manager()->AddHisto("MHT_NoElectronIsoTrack",30,0,1500);
  Manager()->AddHisto("MHT_NoMuonIsoTrack",30,0,1500);
  Manager()->AddHisto("MHT_NoHadronIsoTrack",30,0,1500);
  Manager()->AddHisto("MHT_NJets>=2",30,0,1500);
  Manager()->AddHisto("MHT_HT>300",30,0,1500);
  Manager()->AddHisto("MHT_MHT>300",30,0,1500);
  Manager()->AddHisto("MHT_MinDeltaPhi1",30,0,1500);
  Manager()->AddHisto("MHT_MinDeltaPhi2",30,0,1500);
  Manager()->AddHisto("MHT_MinDeltaPhi3",30,0,1500);
  Manager()->AddHisto("MHT_MinDeltaPhi4",30,0,1500);





//================================================
  return true;
}

// -----------------------------------------------------------------------------
// Finalize
// function called one time at the end of the analysis
// -----------------------------------------------------------------------------
void cms_sus_16_033::Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files)
{
  cout << "BEGIN Finalization" << endl;
  // saving histos
  cout << "END   Finalization" << endl;
}

// -----------------------------------------------------------------------------
// Execute
// function called each time one event is read
// -----------------------------------------------------------------------------
bool cms_sus_16_033::Execute(SampleFormat& sample, const EventFormat& event)
{





  double JEscale = 1.;
  double myEventWeight;
  if(Configuration().IsNoEventWeight()) myEventWeight=1.;
  else if(event.mc()->weight()!=0.) myEventWeight=event.mc()->weight();
  else
    {
      INFO << "Found one event with a zero weight. Skipping...\n";
      return false;
    }
  Manager()->InitializeForNewEvent(myEventWeight);

  //the loop start
  if (event.rec()==0) {return true;}
       EventFormat myEvent;
       myEvent = event;
  // ==========================================
  // Define collections of objects =============
  // ==========================================

    // =====first declare the empty containers:=====
    vector<const RecLeptonFormat*> isoElectron, isoMuon;
    vector<const RecTrackFormat*> electronIsoTracks, muonIsoTracks, hadronIsoTracks;
    vector<const RecJetFormat*> looseJet, tightJet, bJet;

    // =====fill the muons container:=====
    for(unsigned int m=0; m<event.rec()->muons().size(); m++)
    {
      const RecLeptonFormat *CurrentMuon = &(event.rec()->muons()[m]);
      double pt = CurrentMuon->momentum().Pt();
      if(!(pt > 10.)) continue;
      double eta = CurrentMuon->momentum().Eta();
      if(!(fabs(eta) < 2.4)) continue;
      double radius = deltaR_lepton(pt);
      double sumpt = PHYSICS->Isol->eflow->sumIsolation(CurrentMuon, event.rec(), radius, 0., IsolationEFlow::ALL_COMPONENTS);
      //double trackcomponent = PHYSICS->Isol->eflow->sumIsolation(CurrentMuon, event.rec(), radius, 0., IsolationEFlow::TRACK_COMPONENT);
      //double photoncomponent = PHYSICS->Isol->eflow->sumIsolation(CurrentMuon, event.rec(), radius, 0., IsolationEFlow::PHOTON_COMPONENT);
      //double neutralcomponent = PHYSICS->Isol->eflow->sumIsolation(CurrentMuon, event.rec(), radius, 0., IsolationEFlow::NEUTRAL_COMPONENT);
      //double sumpt = calcSumPt(CurrentMuon, radius);
      //cout << "We have a muon: SumPT " << sumpt << " PT " << pt << " radius " << radius << " eta " << eta << endl;
      //cout << "We have a muon: SumPT " << sumpt;
      //cout << " photon component " << photoncomponent;
      //cout << " neutral component " << neutralcomponent;
      //cout << " track component " << trackcomponent;
      //cout << endl;
      //double newsumpt = trackcomponent + photoncomponent + neutralcomponent;
      //cout << "New SumPT: " << newsumpt;
      //cout << endl;
      if(!(sumpt/pt < 0.2)) continue;
      isoMuon.push_back(CurrentMuon);
    }


     // =====fill the electrons container:=====
    for(unsigned int e=0; e<event.rec()->electrons().size(); e++)
    {
      const RecLeptonFormat *CurrentElectron = &(event.rec()->electrons()[e]);
      double pt = CurrentElectron->momentum().Pt();
      if(!(pt > 10.)) continue;
      double eta = CurrentElectron->momentum().Eta();
      if(!(fabs(eta) < 2.5)) continue;
      double radius = deltaR_lepton(pt);
      double sumpt = PHYSICS->Isol->eflow->sumIsolation(CurrentElectron, event.rec(), radius, 0., IsolationEFlow::ALL_COMPONENTS);
      //double sumpt = calcSumPt(CurrentElectron, radius);
      //cout << "We have a electron: SumPT " << sumpt << "PT " << pt << "radius " << radius << "eta " << eta << endl;
      if(!(sumpt/pt < 0.1)) continue;
      isoElectron.push_back(CurrentElectron);
    }



    // =====fill the tracks container:=====
    // TRACKS require isolation as well, but the criteria depend on the nature of the track.
    // If the track is a particle flow (here, in MA5, I ask for the pdgid of the particle)  electron or muon, then the isolation value is 0.2;
    // else, it is 0.1. The eta requirement is the standard 2.4
    // NB I am not sure if the tracks now defined contain also isolated electrons and muons as defined above,
    // but it should make no difference since we have two different cuts for them.

    for(unsigned int i=0; i<event.rec()->tracks().size(); i++)
    {
        const RecTrackFormat *thisTrack = &(event.rec()->tracks()[i]);
        double pt = thisTrack->momentum().Pt();
        double eta =  thisTrack->momentum().Eta();
        double mT = fabs(thisTrack->mt_met(event.rec()->MET().momentum()));
        if(!(fabs(eta) < 2.4)) continue;  // selecting tracks pT,eta
        if(!( pt > 5.)) continue;
        if(!( mT < 100 )) continue;
        int particleId =  thisTrack->pdgid() ; // here I identify if the track is an electron or a muon, since this gives different iso requirements
        bool isElectron   = ( particleId == 11 or particleId == -11) ;
        bool isMuon       = ( particleId == 13 or particleId == -13) ;
        double IsoCone    = 0.3 ;
        double ChargedSum = fabs(PHYSICS->Isol->eflow->sumIsolation(thisTrack,event.rec() , IsoCone ,0.,IsolationEFlow::TRACK_COMPONENT)) / pt;
        if      ( isElectron && (ChargedSum < 0.2) )       // selecting isolated electrons tracks
            { electronIsoTracks.push_back(thisTrack); }
        if      (( isMuon) && (ChargedSum < 0.2) )
            { muonIsoTracks.push_back(thisTrack); }
        if      ( !(isMuon || isElectron) && ( pt > 10. ) && ( ChargedSum < 0.1))
          {hadronIsoTracks.push_back(thisTrack); }
    }


    // =====fill the jet containers and order=====
    for(unsigned int j=0; j<event.rec()->jets().size(); j++)
    {
      const RecJetFormat *CurrentJet = &(event.rec()->jets()[j]);
      double pt = JEscale*CurrentJet->momentum().Pt();
      double eta = CurrentJet->momentum().Eta();
      if(!(pt > 30 && fabs(eta) < 5))continue;
      looseJet.push_back(CurrentJet);
      if(!(pt>30. && fabs(eta) < 2.4))continue;
      if(CurrentJet->btag()) bJet.push_back(CurrentJet);
      tightJet.push_back(CurrentJet);
    }
    SORTER->sort(looseJet);
    SORTER->sort(tightJet);

//======Overlap removal of jets===================//
//     SignalJets = PHYSICS->Isol->JetCleaning(SignalJets, SignalElectrons, 0.4);
//     SignalJets = PHYSICS->Isol->JetCleaning(SignalJets, SignalMuons,     0.4);
    // =====Get the missing ET=====
    //MALorentzVector pTmiss = event.rec()->MET().momentum();
    //double MET = pTmiss.Pt();
    MALorentzVector pTmiss = event.rec()->MET().momentum();
    //double MET = pTmiss.Pt();

    // =====Calculate the HT=====
    double HT = 0;
    for(unsigned int j = 0; j < tightJet.size(); j++)
      {
        HT += JEscale*tightJet[j]->momentum().Pt();
      }

    // =====Calculate the missing HT=====
    double MHT = 0;
    MALorentzVector MHT_vec = MALorentzVector();
    MALorentzVector CurrentJet;
    for(unsigned int j = 0; j < looseJet.size(); j++)
      {
        CurrentJet.SetPtEtaPhiE
          (looseJet[j]->momentum().Pt()*JEscale, looseJet[j]->momentum().Eta(),
           looseJet[j]->momentum().Phi(), looseJet[j]->momentum().E()*JEscale);
        MHT_vec -= CurrentJet;
      }
    MHT = MHT_vec.Pt();
    int NJets = tightJet.size();
    int NbJets = bJet.size();


    // ==========================================
    // Apply the "baseline selection" cuts =======
    // ==========================================


    // =====Before Any Cuts=====
    Manager()->FillHisto("NJets_METCleaning",      NJets);
    Manager()->FillHisto("HT_METCleaning",            HT);
    Manager()->FillHisto("MHT_METCleaning",          MHT);

    // =====Apply NJets jut============
    if(!Manager()->ApplyCut((tightJet.size() >= 2),"Njets>=2")) return true;
    Manager()->FillHisto("NJets_NJets>=2",          NJets);
    Manager()->FillHisto("NbJets_NJets>=2",          NbJets);
    Manager()->FillHisto("HT_NJets>=2",                HT);
    Manager()->FillHisto("MHT_NJets>=2",              MHT);

    // =====Apply HT300 cut============
    if(!Manager()->ApplyCut((HT > 300),"HT>300 GeV")) return true;
    Manager()->FillHisto("NJets_HT>300",           NJets);
    Manager()->FillHisto("NbJets_HT>300",           NbJets);
    Manager()->FillHisto("HT_HT>300",                 HT);
    Manager()->FillHisto("MHT_HT>300",               MHT);

    // =====Apply MHT300 cut===========
    if(!Manager()->ApplyCut((MHT > 300),"MHT>300 GeV")) return true;
    Manager()->FillHisto("NJets_MHT>300",          NJets);
    Manager()->FillHisto("NbJets_MHT>300",          NbJets);
    Manager()->FillHisto("HT_MHT>300",                HT);
    Manager()->FillHisto("MHT_MHT>300",              MHT);


    // =====Apply no-muon cut========
    if(!Manager()->ApplyCut((isoMuon.size() == 0),"muon_veto")) return true;
    Manager()->FillHisto("NJets_NoMuon",         NJets);
    Manager()->FillHisto("NbJets_NoMuon",         NbJets);
    Manager()->FillHisto("HT_NoMuon",               HT);
    Manager()->FillHisto("MHT_NoMuon",             MHT);
    //
    // =====Apply muon isotrack cut========
    if(!Manager()->ApplyCut((muonIsoTracks.size() == 0),"muon_isoTrack_veto")) return true;
    Manager()->FillHisto("NJets_NoMuonIsoTrack",         NJets);
    Manager()->FillHisto("NbJets_NoMuonIsoTrack",         NbJets);
    Manager()->FillHisto("HT_NoMuonIsoTrack",               HT);
    Manager()->FillHisto("MHT_NoMuonIsoTrack",             MHT);

    // =====Apply no-electron cut========
    if(!Manager()->ApplyCut((isoElectron.size() == 0),"electron_veto")) return true;
    Manager()->FillHisto("NJets_NoElectron",         NJets);
    Manager()->FillHisto("NbJets_NoElectron",         NbJets);
    Manager()->FillHisto("HT_NoElectron",               HT);
    Manager()->FillHisto("MHT_NoElectron",             MHT);

    // =====Apply electron isotrack cut========
    if(!Manager()->ApplyCut((electronIsoTracks.size() == 0),"electron_isoTrack_veto")) return true;
    Manager()->FillHisto("NJets_NoElectronIsoTrack",         NJets);
    Manager()->FillHisto("NbJets_NoElectronIsoTrack",         NbJets);
    Manager()->FillHisto("HT_NoElectronIsoTrack",               HT);
    Manager()->FillHisto("MHT_NoElectronIsoTrack",             MHT);


    // =====Apply hadron isotrack cut========
    if(!Manager()->ApplyCut((hadronIsoTracks.size() == 0),"hadron_isoTrack_veto")) return true;
    Manager()->FillHisto("NJets_NoHadronIsoTrack",         NJets);
    Manager()->FillHisto("NbJets_NoHadronIsoTrack",         NbJets);
    Manager()->FillHisto("HT_NoHadronIsoTrack",               HT);
    Manager()->FillHisto("MHT_NoHadronIsoTrack",             MHT);

    // =====Calculate the deltaPhi(jet(1,2,3,4), MHT)=====
    MALorentzVector jet1 = MALorentzVector();
    jet1.SetPtEtaPhiE
      (tightJet[0]->momentum().Pt(), tightJet[0]->momentum().Eta(),
       tightJet[0]->momentum().Phi(), tightJet[0]->momentum().E());
    MALorentzVector jet2 = MALorentzVector();
    jet2.SetPtEtaPhiE
      (tightJet[1]->momentum().Pt(), tightJet[1]->momentum().Eta(),
       tightJet[1]->momentum().Phi(), tightJet[1]->momentum().E());
    MALorentzVector jet3 = MALorentzVector();

    double dPhi1 = fabs(MHT_vec.DeltaPhi(jet1));
    double dPhi2 = fabs(MHT_vec.DeltaPhi(jet2));
    double dPhi3 = 9999.;
    double dPhi4 = 9999.;
    if (tightJet.size() > 2){
        jet3.SetPtEtaPhiE
          (tightJet[2]->momentum().Pt(), tightJet[2]->momentum().Eta(),
           tightJet[2]->momentum().Phi(), tightJet[2]->momentum().E());
        dPhi3 = fabs(MHT_vec.DeltaPhi(jet3));
    }
    if (tightJet.size() > 3){
        MALorentzVector jet4 = MALorentzVector();
        jet4.SetPtEtaPhiE
          (tightJet[3]->momentum().Pt(), tightJet[3]->momentum().Eta(),
           tightJet[3]->momentum().Phi(), tightJet[3]->momentum().E());
        dPhi4 = fabs(MHT_vec.DeltaPhi(jet4));
    }

    // =====Apply min dPhi cut======
    if(!Manager()->ApplyCut( dPhi1>0.5, "dphi_j1_mht > 0.5")) return true;
    Manager()->FillHisto("NJets_MinDeltaPhi1",          NJets);
    Manager()->FillHisto("NbJets_MinDeltaPhi1",          NbJets);
    Manager()->FillHisto("HT_MinDeltaPhi1",                HT);
    Manager()->FillHisto("MHT_MinDeltaPhi1",              MHT);

    if(!Manager()->ApplyCut( dPhi2>0.5, "dphi_j2_mht > 0.5")) return true;
    Manager()->FillHisto("NJets_MinDeltaPhi2",          NJets);
    Manager()->FillHisto("NbJets_MinDeltaPhi2",          NbJets);
    Manager()->FillHisto("HT_MinDeltaPhi2",                HT);
    Manager()->FillHisto("MHT_MinDeltaPhi2",              MHT);

    if(!Manager()->ApplyCut( dPhi3>0.3, "dphi_j3_mht > 0.3")) return true;
    Manager()->FillHisto("NJets_MinDeltaPhi3",          NJets);
    Manager()->FillHisto("NbJets_MinDeltaPhi3",          NbJets);
    Manager()->FillHisto("HT_MinDeltaPhi3",                HT);
    Manager()->FillHisto("MHT_MinDeltaPhi3",              MHT);

    if(!Manager()->ApplyCut( dPhi4>0.3, "dphi_j4_mht > 0.3")) return true;
    Manager()->FillHisto("NJets_MinDeltaPhi4",          NJets);
    Manager()->FillHisto("NbJets_MinDeltaPhi4",          NbJets);
    Manager()->FillHisto("HT_MinDeltaPhi4",                HT);
    Manager()->FillHisto("MHT_MinDeltaPhi4",              MHT);
    // ==========================================
    // Fill Histograms for extra checking =======
    // ==========================================


    // ==========================================
    // Apply the Signal Region - dependent cuts =======
    // ==========================================
    // Nbjets 0

    //if(!Manager()->ApplyCut(HT > 500  && MHT > 500 && NJets >= 2 && NbJets == 0, "SR1" ) ) return true;
    //if(!Manager()->ApplyCut(HT > 1500 && MHT > 750 && NJets >= 3 && NbJets == 0, "SR2" ) ) return true;
    //if(!Manager()->ApplyCut(HT > 500  && MHT > 500 && NJets >= 5 && NbJets == 0, "SR3" ) ) return true;
    //if(!Manager()->ApplyCut(HT > 1500 && MHT > 750 && NJets >= 5 && NbJets == 0, "SR4" ) ) return true;
    //if(!Manager()->ApplyCut(HT > 1500 && MHT > 750 && NJets >= 9 && NbJets == 0, "SR5" ) ) return true;


    //if(!Manager()->ApplyCut(HT > 500  && MHT > 500 && NJets >= 2 && NbJets >= 2, "SR6" ) ) return true;
    //if(!Manager()->ApplyCut(HT > 750  && MHT > 750 && NJets >= 3 && NbJets >= 1, "SR7" ) ) return true;
    //if(!Manager()->ApplyCut(HT > 500  && MHT > 500 && NJets >= 5 && NbJets >= 3, "SR8" ) ) return true;
    //if(!Manager()->ApplyCut(HT > 1500 && MHT > 750 && NJets >= 5 && NbJets >= 2, "SR9" ) ) return true;
    //if(!Manager()->ApplyCut(HT > 750  && MHT > 750 && NJets >= 9 && NbJets >= 3, "SR10") ) return true;
    //if(!Manager()->ApplyCut(HT > 300  && MHT > 300 && NJets >= 7 && NbJets >= 1, "SR11") ) return true;
    //if(!Manager()->ApplyCut(HT > 750  && MHT > 750 && NJets >= 5 && NbJets >= 1, "SR12") ) return true;
    Manager()->ApplyCut(1 <= NbJets, "NbJets>=1");
    Manager()->ApplyCut(2 <= NbJets, "NbJets>=2");
    Manager()->ApplyCut(3 <= NbJets, "NbJets>=3");
    Manager()->ApplyCut(2 <= NJets, "NJets>=2");
    Manager()->ApplyCut(3 <= NJets, "NJets>=3");
    Manager()->ApplyCut(5 <= NJets, "NJets>=5");
    Manager()->ApplyCut(7 <= NJets, "NJets>=7");
    Manager()->ApplyCut(9 <= NJets, "NJets>=9");
    Manager()->ApplyCut(300 < HT, "HT>300");
    Manager()->ApplyCut(500 < HT, "HT>500");
    Manager()->ApplyCut(750 < HT, "HT>750");
    Manager()->ApplyCut(1500 < HT, "HT>1500");
    Manager()->ApplyCut(300 < MHT, "MHT>300");
    Manager()->ApplyCut(500 < MHT, "MHT>500");
    Manager()->ApplyCut(750 < MHT, "MHT>750");
    Manager()->ApplyCut(0 == NbJets, "Nbjets0");

//==========End===================//

    return true;




}

