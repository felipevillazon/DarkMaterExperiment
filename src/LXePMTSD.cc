#include "LXePMTSD.hh"

#include "LXeDetectorConstruction.hh"
#include "LXePMTHit.hh"
#include "LXeUserTrackInformation.hh"

#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4VVisManager.hh"


LXePMTSD::LXePMTSD(G4String name): G4VSensitiveDetector(name)
{}

LXePMTSD::~LXePMTSD()
{}


G4bool LXePMTSD::ProcessHits(G4Step* aStep, G4TouchableHistory*R0hist) { 

  G4Track *track = aStep->GetTrack(); 
  
  G4ParticleDefinition* particleType = track->GetDefinition();
 
 
         // to keep track of the photon
	
	
	
	track->SetTrackStatus(fStopAndKill);  //  once the photons enter the detectors, the track is no propagated any further
	

	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();  // We define presetep when the photon get inot the detector
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();  // Post step when the photon go out of the detector
	
	//G4double photonEnergy = preStepPoint->GetTotalEnergy();
	
	//if(photonEnergy < 6.9e-6){
	G4ThreeVector posPhoton = preStepPoint->GetPosition();    // We find the position of the photon when it gets into the detector
	 
        //G4cout << "Photon Position in PMT: " << posPhoton << G4endl;
	
	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
	
	G4int copyNo = touchable->GetCopyNumber();
	
	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();
	
	//G4cout << "Detector position " << posDetector << G4endl;
	//G4cout << "Detector number: " << copyNo << G4endl;
	
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	
	G4double photonEnergy = preStepPoint->GetTotalEnergy();
	
	//G4cout << "Energy deposited by optical photon in MeV: " << photonEnergy << G4endl;
	
	
	G4double time = preStepPoint->GetLocalTime();
	
	//G4cout << "Time: " << time << G4endl;
	
	G4double xVertex = (track->GetVertexPosition()).x();
	G4double yVertex = (track->GetVertexPosition()).y();
	G4double zVertex = (track->GetVertexPosition()).z();
	
	//G4cout << "xVertex: " << xVertex << G4endl;
	
	
		
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->FillNtupleIColumn(0,0,evt);
	man->FillNtupleDColumn(0,1,posPhoton[0]);
	man->FillNtupleDColumn(0,2,posPhoton[1]);
	man->FillNtupleDColumn(0,3,posPhoton[2]);
	man->FillNtupleDColumn(0,4,copyNo);
	man->FillNtupleDColumn(0,5,photonEnergy);
	man->FillNtupleDColumn(0,6,time);
	man->FillNtupleDColumn(0,7,zVertex);
	man->FillNtupleDColumn(0,8,yVertex);
	man->FillNtupleDColumn(0,9,xVertex);
	man->AddNtupleRow(0);
	
	//}
	
	/*if(photonEnergy > 6.9e-6){
	G4ThreeVector posPhoton = preStepPoint->GetPosition();    // We find the position of the photon when it gets into the detector
	 
	//G4cout << "Photon Position in PMT: " << posPhoton << G4endl;
	
	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
	
	G4int copyNo = touchable->GetCopyNumber();
	
	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();
	
	//G4cout << "Detector position " << posDetector << G4endl;
	G4cout << "Detector number: " << copyNo << G4endl;
	
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	
	G4double photonEnergy = preStepPoint->GetTotalEnergy();
	
	G4cout << "Energy deposited by optical photon in MeV: " << photonEnergy << G4endl;
	
	
	G4double time = preStepPoint->GetLocalTime();
	
	//G4cout << "Time: " << time << G4endl;
	
	
	
		
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->FillNtupleIColumn(1,0,evt);
	man->FillNtupleDColumn(1,1,posPhoton[0]);
	man->FillNtupleDColumn(1,2,posPhoton[1]);
	man->FillNtupleDColumn(1,3,posPhoton[2]);
	man->FillNtupleDColumn(1,4,copyNo);
	man->FillNtupleDColumn(1,5,photonEnergy);
	man->FillNtupleDColumn(1,6,time);
	man->AddNtupleRow(1);
	
	}*/
  return true; }


G4bool LXePMTSD::ProcessHits_boundary(const G4Step* aStep, G4TouchableHistory*)
{
 // need to know if this is an optical photon
/*  if(aStep->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
    return false;

  // User replica number 1 since photocathode is a daughter volume
  // to the pmt which was replicated
  G4int pmtNumber = aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber();
  G4VPhysicalVolume* physVol = aStep->GetPostStepPoint()->GetTouchable()->GetVolume();

  // Find the correct hit collection
  size_t n       = fPMTHitCollection->entries();
  LXePMTHit* hit = nullptr;
  for(size_t i = 0; i < n; ++i)
  {
    if((*fPMTHitCollection)[i]->GetPMTNumber() == pmtNumber)
    {
      hit = (*fPMTHitCollection)[i];
      break;
    }
  }

  if(hit == nullptr)
  {                         // this pmt wasn't previously hit in this event
    hit = new LXePMTHit();  // so create new hit
    hit->SetPMTNumber(pmtNumber);
    hit->SetPMTPhysVol(physVol);
    fPMTHitCollection->insert(hit);
  }

  hit->IncPhotonCount();  // increment hit for the selected pmt	*/

  return false;

}
