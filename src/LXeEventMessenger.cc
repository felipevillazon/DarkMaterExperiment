
#include "LXeEventMessenger.hh"

#include "LXeEventAction.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeEventMessenger::LXeEventMessenger(LXeEventAction* event)
  : fLXeEvent(event)
{
  fVerboseCmd = new G4UIcmdWithAnInteger("/LXe/eventVerbose", this);
  fVerboseCmd->SetGuidance("Set the verbosity of event data.");
  fVerboseCmd->SetParameterName("verbose", true);
  fVerboseCmd->SetDefaultValue(1);

  fPmtThresholdCmd = new G4UIcmdWithAnInteger("/LXe/pmtThreshold", this);
  fPmtThresholdCmd->SetGuidance("Set the pmtThreshold (in # of photons)");

  fForceDrawPhotonsCmd = new G4UIcmdWithABool("/LXe/forceDrawPhotons", this);
  fForceDrawPhotonsCmd->SetGuidance("Force drawing of photons.");
  fForceDrawPhotonsCmd->SetGuidance(
    "(Higher priority than /LXe/forceDrawNoPhotons)");

  fForceDrawNoPhotonsCmd =
    new G4UIcmdWithABool("/LXe/forceDrawNoPhotons", this);
  fForceDrawNoPhotonsCmd->SetGuidance("Force no drawing of photons.");
  fForceDrawNoPhotonsCmd->SetGuidance(
    "(Lower priority than /LXe/forceDrawPhotons)");
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeEventMessenger::~LXeEventMessenger()
{
  delete fVerboseCmd;
  delete fPmtThresholdCmd;
  delete fForceDrawPhotonsCmd;
  delete fForceDrawNoPhotonsCmd;
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeEventMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == fVerboseCmd)
  {
    fLXeEvent->SetEventVerbose(fVerboseCmd->GetNewIntValue(newValue));
  }
  else if(command == fPmtThresholdCmd)
  {
    fLXeEvent->SetPMTThreshold(fPmtThresholdCmd->GetNewIntValue(newValue));
  }
  else if(command == fForceDrawPhotonsCmd)
  {
    fLXeEvent->SetForceDrawPhotons(
      fForceDrawPhotonsCmd->GetNewBoolValue(newValue));
  }
  else if(command == fForceDrawNoPhotonsCmd)
  {
    fLXeEvent->SetForceDrawNoPhotons(
      fForceDrawNoPhotonsCmd->GetNewBoolValue(newValue));
  }
}
