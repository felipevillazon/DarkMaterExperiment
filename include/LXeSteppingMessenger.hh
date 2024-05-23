
#ifndef LXeSteppingMessenger_h
#define LXeSteppingMessenger_h 1

#include "G4UImessenger.hh"

class LXeSteppingAction;

class G4UIcmdWithABool;

class LXeSteppingMessenger : public G4UImessenger
{
 public:
  LXeSteppingMessenger(LXeSteppingAction*);
  ~LXeSteppingMessenger();

  void SetNewValue(G4UIcommand*, G4String) override;

 private:
  LXeSteppingAction* fStepping;
  G4UIcmdWithABool* fOneStepPrimariesCmd;
};

#endif
