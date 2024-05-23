#include "LXeDetectorMessenger.hh"

#include "LXeDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Scintillation.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"

#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeDetectorMessenger::LXeDetectorMessenger(LXeDetectorConstruction* detector)
  : fLXeDetector(detector)
{
  // Setup a command directory for detector controls with guidance
  fDetectorDir = new G4UIdirectory("/LXe/detector/");
  fDetectorDir->SetGuidance("Detector geometry control");

  fVolumesDir = new G4UIdirectory("/LXe/detector/volumes/");
  fVolumesDir->SetGuidance("Enable/disable volumes");

  // Various commands for modifying detector geometry

  fReflectivityCmd = new G4UIcmdWithADouble("/LXe/detector/reflectivity", this);
  fReflectivityCmd->SetGuidance("Set the reflectivity of the housing.");
  fReflectivityCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fReflectivityCmd->SetToBeBroadcasted(false);

  fNFibersCmd = new G4UIcmdWithAnInteger("/LXe/detector/nfibers", this);
  fNFibersCmd->SetGuidance("Set the number of WLS fibers in the WLS slab.");
  fNFibersCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fNFibersCmd->SetToBeBroadcasted(false);
  
  fNClad2Cmd = new G4UIcmdWithADouble("/LXe/detector/nindex_clad2", this);
  fNClad2Cmd->SetGuidance("Set the refraction index of cladding material 2.");
  fNClad2Cmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fNClad2Cmd->SetToBeBroadcasted(false);
  
  fNClad1Cmd = new G4UIcmdWithADouble("/LXe/detector/nindex_clad1", this);
  fNClad1Cmd->SetGuidance("Set the refraction index of cladding material 1.");
  fNClad1Cmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fNClad1Cmd->SetToBeBroadcasted(false);
  
  fNCoreCmd = new G4UIcmdWithADouble("/LXe/detector/nindex_core", this);
  fNCoreCmd->SetGuidance("Set the refraction index of core material.");
  fNCoreCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fNCoreCmd->SetToBeBroadcasted(false);
  
  fAbsLengthCoreCmd = new G4UIcmdWithADoubleAndUnit("/LXe/detector/abslength_core", this);
  fAbsLengthCoreCmd->SetGuidance("Set the absorption length of the core material.");
  fAbsLengthCoreCmd->SetParameterName("AbsLength", false);
  fAbsLengthCoreCmd->SetDefaultUnit("mm");
  fAbsLengthCoreCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fAbsLengthCoreCmd->SetToBeBroadcasted(false);
  
  fScattLengthLXeCmd = new G4UIcmdWithADoubleAndUnit("/LXe/detector/scattlength_lxe", this);
  fScattLengthLXeCmd->SetGuidance("Set the scattering length of LXe");
  fScattLengthLXeCmd->SetParameterName("ScattLength", false);
  fScattLengthLXeCmd->SetDefaultUnit("mm");
  fScattLengthLXeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fScattLengthLXeCmd->SetToBeBroadcasted(false);

  fWLSTimeCmd = new G4UIcmdWithADoubleAndUnit("/LXe/detector/WLSTime", this);
  fWLSTimeCmd->SetGuidance("Set the WLS time of the core material.");
  fWLSTimeCmd->SetParameterName("WLSTime", false);
  fWLSTimeCmd->SetDefaultUnit("ns");
  fWLSTimeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fWLSTimeCmd->SetToBeBroadcasted(false);

  fDefaultsCmd = new G4UIcommand("/LXe/detector/defaults", this);
  fDefaultsCmd->SetGuidance("Set all detector geometry values to defaults.");
  fDefaultsCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fDefaultsCmd->SetToBeBroadcasted(false);
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeDetectorMessenger::~LXeDetectorMessenger()
{
  delete fNFibersCmd;
  delete fReflectivityCmd;
  delete fDefaultsCmd;
  delete fDetectorDir;
  delete fVolumesDir;
  delete fNClad2Cmd;
  delete fNClad1Cmd;
  delete fNCoreCmd;
  delete fAbsLengthCoreCmd;
  delete fScattLengthLXeCmd;
  delete fWLSTimeCmd;
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
 
  if(command == fReflectivityCmd)
  {
    fLXeDetector->SetHousingReflectivity(
      fReflectivityCmd->GetNewDoubleValue(newValue));
  }
  else if(command == fNFibersCmd)
  {
    fLXeDetector->SetNFibers(fNFibersCmd->GetNewIntValue(newValue));
  }
  else if(command == fNClad2Cmd)
  {
    fLXeDetector->SetNClad2(fNClad2Cmd->GetNewDoubleValue(newValue));
    G4cout << "The refraction index for cladding2 has been changed" << G4endl;
    G4cout << "The new refraction index is: " << fLXeDetector->GetNClad2() << G4endl;
  }
  else if(command == fNClad1Cmd)
  {
    fLXeDetector->SetNClad1(fNClad1Cmd->GetNewDoubleValue(newValue));
    G4cout << "The refraction index for cladding1 has been changed" << G4endl;
  }
  else if(command == fNCoreCmd)
  {
    fLXeDetector->SetNCore(fNCoreCmd->GetNewDoubleValue(newValue));
    G4cout << "The refraction index for Core has been changed" << G4endl;
  }
  else if(command == fAbsLengthCoreCmd)
  {
    fLXeDetector->SetAbsLengthCore(fAbsLengthCoreCmd->GetNewDoubleValue(newValue));
    G4cout << "The absorption length of the PMMA has been changed" << G4endl;
  }
  else if(command == fScattLengthLXeCmd)
  {
    fLXeDetector->SetScattLengthLXe(fScattLengthLXeCmd->GetNewDoubleValue(newValue));
    G4cout << "The scattering length of LXe has been changed" << G4endl;
  }
  else if(command == fWLSTimeCmd)
  {
    fLXeDetector->SetWLSTime(fWLSTimeCmd->GetNewDoubleValue(newValue));
    G4cout << "The WLS time of the PMMA has been changed" << G4endl;
  }
  else if(command == fDefaultsCmd)
  {
    fLXeDetector->SetDefaults();
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
}
