#include "LXeSteppingAction.hh"

#include "LXeEventAction.hh"
#include "LXePMTSD.hh"
#include "LXeSteppingMessenger.hh"
#include "LXeTrajectory.hh"
#include "LXeUserTrackInformation.hh"

#include "G4OpticalPhoton.hh"
#include "G4ProcessManager.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeSteppingAction::LXeSteppingAction(LXeEventAction* ea)
  : fOneStepPrimaries(false)
  , fEventAction(ea)
{
  fSteppingMessenger = new LXeSteppingMessenger(this);

  fExpectedNextStatus = Undefined;
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeSteppingAction::~LXeSteppingAction() { delete fSteppingMessenger; }

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeSteppingAction::UserSteppingAction(const G4Step* theStep)
{
  /*G4Track* theTrack = theStep->GetTrack();
  const G4ParticleDefinition* part = theTrack->GetDefinition();
  G4int pdg = part->GetPDGEncoding();


  if(theTrack->GetCurrentStepNumber() == 1)
    fExpectedNextStatus = Undefined;

  LXeUserTrackInformation* trackInformation = static_cast<LXeUserTrackInformation*>(theTrack->GetUserInformation());

  G4StepPoint* thePrePoint    = theStep->GetPreStepPoint();
  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume(); //Physical volume before the step

  G4StepPoint* thePostPoint    = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume(); //Physical volume after the step
  
  const G4VProcess* lastProcess = thePostPoint->GetProcessDefinedStep();

  G4OpBoundaryProcessStatus boundaryStatus = Undefined;

  // find the boundary process only once
  if(nullptr == fBoundary && pdg == -22)
  {
    G4ProcessManager* pm = part->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    for(G4int i = 0; i < nprocesses; ++i)
    {
      if(nullptr != (*pv)[i] && (*pv)[i]->GetProcessName() == "OpBoundary")
      {
        fBoundary = dynamic_cast<G4OpBoundaryProcess*>((*pv)[i]);
        break;
      }
    }
  }

  if(theTrack->GetParentID() == 0)
  {
    // This is a primary track
    auto secondaries = theStep->GetSecondaryInCurrentStep();
    // If we haven't already found the conversion position and there were
    // secondaries generated, then search for it
    // since this is happening before the secondary is being tracked,
    // the vertex position has not been set yet (set in initial step)
    if(nullptr != secondaries && !fEventAction->IsConvPosSet())
    {
      if(!secondaries->empty()) 
      {
	for(auto & tr : *secondaries)
	{
	  const G4VProcess* creator = tr->GetCreatorProcess();
	  if(nullptr != creator)
	  {
	    G4int type = creator->GetProcessSubType();
	    // 12 - photoeffect
	    // 13 - Compton scattering
	    // 14 - gamma conversion
	    if(type >= 12 && type <= 14)
            {
	      fEventAction->SetConvPos(tr->GetPosition());
	    }
	  }
	}
      }
    }
    //if(fOneStepPrimaries && thePrePV->GetName() == "scintillator")
      //theTrack->SetTrackStatus(fStopAndKill);
  }

  if(nullptr == thePostPV)
  {  // out of world
    fExpectedNextStatus = Undefined;
    return;
  }

  // Optical photon only
  if(pdg == -22)
  {
  
 // if (lastProcess) {
 //           G4String processName = lastProcess->GetProcessName();
 //           G4cout << "Last process: " << processName << G4endl;
 //       }
        
    if(thePrePV->GetName() == "Cladding2")
      // force drawing of photons in WLS slab
      trackInformation->SetForceDrawTrajectory(true);
   // else if(thePostPV->GetName() == "expHall")
      // Kill photons entering expHall from something other than Slab
     // theTrack->SetTrackStatus(fStopAndKill);

    // Was the photon absorbed by the absorption process
    auto proc = thePostPoint->GetProcessDefinedStep(); 
    if(nullptr != proc && proc->GetProcessName() == "OpAbsorption")
    {
      fEventAction->IncAbsorption();
      trackInformation->AddTrackStatusFlag(absorbed);
      //G4cout << "Last process: " << proc->GetProcessName() << G4endl;
    }
    if(nullptr != fBoundary)
      boundaryStatus = fBoundary->GetStatus();

    if(thePostPoint->GetStepStatus() == fGeomBoundary)
    {
      // Check to see if the particle was actually at a boundary
      // Otherwise the boundary status may not be valid
      //if(fExpectedNextStatus == StepTooSmall)
      //{	
      //  if(boundaryStatus != StepTooSmall)
      //  {
         // G4cout << "LXeSteppingAction::UserSteppingAction(): "
	//	 << "trackID=" << theTrack->GetTrackID() 
	//	 << " parentID=" << theTrack->GetParentID()
	//	 << " " << part->GetParticleName()
	//	 << " E(MeV)=" << theTrack->GetKineticEnergy()
	//	 << "n/ at " << theTrack->GetPosition()
	//	 << " prePV: " << thePrePV->GetName()
	//	 << " postPV: " << thePostPV->GetName()
	//	 << G4endl;
          //G4ExceptionDescription ed;
         // ed << "LXeSteppingAction: "
         //    << "No reallocation step after reflection!"
	 //    << "Something is wrong with the surface normal or geometry";
         // G4Exception("LXeSteppingAction:", "LXeExpl01", JustWarning, ed, "");
	 // return;
       // }
      //}
      fExpectedNextStatus = Undefined;
      switch(boundaryStatus)
      {
        case Absorption:
          trackInformation->AddTrackStatusFlag(boundaryAbsorbed);
          fEventAction->IncBoundaryAbsorption();
          break;
        case Detection:  // Note, this assumes that the volume causing detection
                         // is the photocathode because it is the only one with
                         // non-zero efficiency
        {
          // Trigger sensitive detector manually since photon is
          // absorbed but status was Detection
          G4SDManager* SDman = G4SDManager::GetSDMpointer();
          G4String sdName    = "/LXeDet/pmtSD";
          LXePMTSD* pmtSD    = (LXePMTSD*) SDman->FindSensitiveDetector(sdName);
          if(pmtSD)
            pmtSD->ProcessHits_boundary(theStep, nullptr);
          trackInformation->AddTrackStatusFlag(hitPMT);
          break;
        }
        case FresnelReflection:
        case TotalInternalReflection:
        case LambertianReflection:
        case LobeReflection:
        case SpikeReflection:
        case BackScattering:
          trackInformation->IncReflections();
          fExpectedNextStatus = StepTooSmall;
          break;
        default:
          break;
      }
    }
  }*/
}
