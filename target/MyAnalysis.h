#ifndef analysis_MyAnalysis_h
#define analysis_MyAnalysis_h

#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>

#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

namespace MA5
{
class MyAnalysis : public AnalyzerBase
{
  INIT_ANALYSIS(MyAnalysis,"MyAnalysis")

 public:
  virtual bool Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters);
  virtual void Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files);
  virtual bool Execute(SampleFormat& sample, const EventFormat& event);

 private:
  int nPhoton;
  int nSignal;
  int nEvent;

  TCanvas *c;
  TH1F * diPhotonMass;
  TH1F * missingET;
};
}

#endif
