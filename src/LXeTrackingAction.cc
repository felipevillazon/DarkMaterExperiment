
#include "LXeTrackingAction.hh"

#include "LXeDetectorConstruction.hh"
#include "LXeTrajectory.hh"
#include "LXeUserTrackInformation.hh"

#include "G4OpticalPhoton.hh"
#include "G4Track.hh"
#include "G4TrackingManager.hh"

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeTrackingAction::LXeTrackingAction() {}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  // Let this be up to the user via vis.mac
  //  fpTrackingManager->SetStoreTrajectory(true);

  // Use custom trajectory class
 /* fpTrackingManager->SetTrajectory(new LXeTrajectory(aTrack));

  // This user track information is only relevant to the photons
  fpTrackingManager->SetUserTrackInformation(new LXeUserTrackInformation);*/
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
 /* LXeTrajectory* trajectory =
    (LXeTrajectory*) fpTrackingManager->GimmeTrajectory();
  LXeUserTrackInformation* trackInformation =
    (LXeUserTrackInformation*) aTrack->GetUserInformation();

  // Let's choose to draw only the photons that hit the sphere and a pmt
  if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
  {
    const G4VProcess* creator = aTrack->GetCreatorProcess();
    if(creator && creator->GetProcessName() == "OpWLS")
    {
      trajectory->WLS();
      trajectory->SetDrawTrajectory(true);
    }

    else
    {
      if(trackInformation->GetTrackStatus() & hitPMT)
        trajectory->SetDrawTrajectory(true);
    }
  }
  // draw all other (not optical photon) trajectories
  else
    trajectory->SetDrawTrajectory(true);

  if(trackInformation->GetForceDrawTrajectory())
    trajectory->SetDrawTrajectory(true);*/
}
