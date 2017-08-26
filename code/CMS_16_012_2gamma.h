#ifndef analysis_CMS_16_012_2gamma_h
#define analysis_CMS_16_012_2gamma_h

#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

namespace MA5
{
class CMS_16_012_2gamma : public AnalyzerBase
{
  INIT_ANALYSIS(CMS_16_012_2gamma,"CMS_16_012_2gamma")

 public:
  virtual bool Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters);
  virtual void Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files);
  virtual bool Execute(SampleFormat& sample, const EventFormat& event);

 private:
// double Invariant_Mass(const RecPhotonFormat* jet1, const RecPhotonFormat* jet2);
 int AA,BB,CC,begin_before,begin_after;
};
}

#endif
