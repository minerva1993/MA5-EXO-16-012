#ifndef analysis_tutorial2_h
#define analysis_tutorial2_h

#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

namespace MA5
{
class tutorial2 : public AnalyzerBase
{
  INIT_ANALYSIS(tutorial2,"tutorial2")

 public:
  virtual bool Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters);
  virtual void Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files);
  virtual bool Execute(SampleFormat& sample, const EventFormat& event);

 private:
  int nPhoton;
  int nSignal;
  int nEvent;
};
}

#endif
