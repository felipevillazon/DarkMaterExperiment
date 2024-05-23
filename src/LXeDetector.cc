#include "LXeDetector.hh"
#include "G4OpticalPhoton.hh"
#include "G4Colour.hh"
#include "G4VVisManager.hh"
LXeSensitiveDetector::LXeSensitiveDetector(G4String name): G4VSensitiveDetector(name)
{}


LXeSensitiveDetector::~LXeSensitiveDetector(){}

G4bool LXeSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *R0hist)
{
  
  G4Track *track = aStep->GetTrack();  // to keep track of the photon
  track->SetTrackStatus(fStopAndKill);  //  once the photons enter the detectors, the track is no propagated any further
	
  G4ParticleDefinition* particleType = track->GetDefinition();

  if (particleType == G4OpticalPhoton::OpticalPhotonDefinition()){
      
	
     G4StepPoint *preStepPoint = aStep->GetPreStepPoint();  // We define presetep when the photon get inot the detector
     G4StepPoint *postStepPoint = aStep->GetPostStepPoint();  // Post step when the photon go out of the detector
	
     G4ThreeVector posPhoton = preStepPoint->GetPosition();    // We find the position of the photon when it gets into the detector
     G4cout << "Photon Position in PMT: " << posPhoton << G4endl;   // printing the position of the photon and a text  
	
     const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
     G4int copyNo = touchable->GetCopyNumber();
	
     G4VPhysicalVolume *physVol = touchable->GetVolume();
     G4ThreeVector posDetector = physVol->GetTranslation();
     
     G4cout << "Detector position " << posDetector << G4endl;
     G4cout << "Detector number: " << copyNo << G4endl;
	
     G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	
     G4double photonEnergy = preStepPoint->GetTotalEnergy();
     G4cout << "Energy deposited by optical photon in MeV: " << photonEnergy << G4endl;
	
     G4double time = preStepPoint->GetLocalTime();
     G4cout << "Time: " << time << G4endl;
	
	
	
	//G4AnalysisManager *man = G4AnalysisManager::Instance();
	//man->FillNtupleIColumn(0,evt);
	//man->FillNtupleDColumn(1,posPhoton[0]);
	//man->FillNtupleDColumn(2,posPhoton[1]);
	//man->FillNtupleDColumn(3,posPhoton[2]);
	//man->FillNtupleDColumn(4,copyNo);
	//man->FillNtupleDColumn(5,photonEnergy);
	//man->FillNtupleDColumn(6,time);
	//man->AddNtupleRow(0);
	}
    
  
  return false;
}	
