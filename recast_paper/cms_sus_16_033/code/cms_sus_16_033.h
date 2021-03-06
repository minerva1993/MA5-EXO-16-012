#ifndef analysis_cms_sus_16_033_h
#define analysis_cms_sus_16_033_h

#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

namespace MA5
{
class cms_sus_16_033 : public AnalyzerBase
{
  INIT_ANALYSIS(cms_sus_16_033,"cms_sus_16_033")

 public:
  virtual bool Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters);
  virtual void Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files);
  virtual bool Execute(SampleFormat& sample, const EventFormat& event);

 private:
};
}

#endif