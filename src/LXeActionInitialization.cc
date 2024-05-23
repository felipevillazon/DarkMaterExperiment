#include "LXeActionInitialization.hh"

#include "LXeDetectorConstruction.hh"
#include "LXeEventAction.hh"
#include "LXePrimaryGeneratorAction.hh"
#include "LXeRunAction.hh"
#include "LXeStackingAction.hh"
#include "LXeSteppingAction.hh"
#include "LXeTrackingAction.hh"

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeActionInitialization::LXeActionInitialization(
  const LXeDetectorConstruction* det)
  : G4VUserActionInitialization()
  , fDetector(det)
{}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeActionInitialization::~LXeActionInitialization() {}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeActionInitialization::BuildForMaster() const
{
  SetUserAction(new LXeRunAction());
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeActionInitialization::Build() const
{
  SetUserAction(new LXePrimaryGeneratorAction());

  LXeEventAction* eventAction = new LXeEventAction(fDetector);
  SetUserAction(eventAction);
  SetUserAction(new LXeStackingAction(eventAction));

  SetUserAction(new LXeRunAction());
  SetUserAction(new LXeTrackingAction());
  SetUserAction(new LXeSteppingAction(eventAction));
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//
