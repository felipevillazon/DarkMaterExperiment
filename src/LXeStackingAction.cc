#include "LXeStackingAction.hh"

#include "LXeEventAction.hh"

#include "G4OpticalPhoton.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeStackingAction::LXeStackingAction(LXeEventAction* ea)
  : fEventAction(ea)
{}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeStackingAction::~LXeStackingAction() {}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

G4ClassificationOfNewTrack LXeStackingAction::ClassifyNewTrack(
  const G4Track* aTrack)
{
  // Count what process generated the optical photons
 /* if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
  {
    // particle is optical photon
    if(aTrack->GetParentID() > 0)
    {
      // particle is secondary
      if(aTrack->GetCreatorProcess()->GetProcessName() == "Scintillation")
        fEventAction->IncPhotonCount_Scint();
      else if(aTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov")
        fEventAction->IncPhotonCount_Ceren();
    }
  }*/
  return fUrgent;
}
