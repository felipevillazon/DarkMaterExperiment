
#ifndef LXeEventMessenger_h
#define LXeEventMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class LXeEventAction;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;

class LXeEventMessenger : public G4UImessenger
{
 public:
  LXeEventMessenger(LXeEventAction*);
  ~LXeEventMessenger();

  void SetNewValue(G4UIcommand*, G4String) override;

 private:
  LXeEventAction* fLXeEvent;
  G4UIcmdWithAnInteger* fVerboseCmd;
  G4UIcmdWithAnInteger* fPmtThresholdCmd;
  G4UIcmdWithABool* fForceDrawPhotonsCmd;
  G4UIcmdWithABool* fForceDrawNoPhotonsCmd;
};

#endif
