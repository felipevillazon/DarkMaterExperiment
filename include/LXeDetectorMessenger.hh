#ifndef LXeDetectorMessenger_h
#define LXeDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class LXeDetectorConstruction;

class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;
class G4UIcommand;
class G4UIdirectory;

class LXeDetectorMessenger : public G4UImessenger
{
 public:
  LXeDetectorMessenger(LXeDetectorConstruction*);
  ~LXeDetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String) override;

 private:
  LXeDetectorConstruction* fLXeDetector;
  G4UIdirectory* fDetectorDir;
  G4UIdirectory* fVolumesDir;
  G4UIcmdWith3VectorAndUnit* fDimensionsCmd;
  G4UIcmdWithADoubleAndUnit* fHousingThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fPmtRadiusCmd;
  G4UIcmdWithADouble* fReflectivityCmd;
  G4UIcmdWithAnInteger* fNFibersCmd;
  G4UIcmdWithADouble* fNClad2Cmd;
  G4UIcmdWithADouble* fNClad1Cmd;
  G4UIcmdWithADouble* fNCoreCmd;
  G4UIcmdWithADoubleAndUnit* fAbsLengthCoreCmd;
  G4UIcmdWithADoubleAndUnit* fScattLengthLXeCmd;
  G4UIcmdWithADoubleAndUnit* fWLSTimeCmd;
  G4UIcommand* fDefaultsCmd;
  G4UIcmdWithADouble* fMainScintYield;
  G4UIcmdWithADouble* fWLSScintYield;
};

#endif	
