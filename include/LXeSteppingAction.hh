
#ifndef LXeSteppingAction_h
#define LXeSteppingACtion_h 1

#include "globals.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4UserSteppingAction.hh"

class LXeEventAction;
class LXeTrackingAction;
class LXeSteppingMessenger;

class LXeSteppingAction : public G4UserSteppingAction
{
 public:
  LXeSteppingAction(LXeEventAction*);
  ~LXeSteppingAction();

  void UserSteppingAction(const G4Step*) override;

  void SetOneStepPrimaries(G4bool b) { fOneStepPrimaries = b; }
  G4bool GetOneStepPrimaries() { return fOneStepPrimaries; }

 private:
  G4bool fOneStepPrimaries;
  LXeSteppingMessenger* fSteppingMessenger;
  LXeEventAction* fEventAction;
  G4OpBoundaryProcess* fBoundary = nullptr;

  G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif
