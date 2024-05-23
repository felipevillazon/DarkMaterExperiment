
#include "G4UserRunAction.hh"

#ifndef LXeRunAction_h
#  define LXeRunAction_h 1

class LXeRun;
class LXeHistoManager;

class G4Run;

class LXeRunAction : public G4UserRunAction
{
 public:
  LXeRunAction();
  ~LXeRunAction();

  G4Run* GenerateRun() override;
  void BeginOfRunAction(const G4Run*) override;
  void EndOfRunAction(const G4Run*) override;

 private:
  LXeRun* fRun;
  LXeHistoManager* fHistoManager;
};

#endif
