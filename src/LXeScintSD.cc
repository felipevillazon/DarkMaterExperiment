#include "LXeScintSD.hh"

#include "LXeScintHit.hh"

#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VProcess.hh"
#include "G4VTouchable.hh"

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeScintSD::LXeScintSD(G4String name)
  : G4VSensitiveDetector(name)
  , fHitsCID(-1)
{
  fScintCollection = nullptr;
  collectionName.insert("scintCollection");
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeScintSD::~LXeScintSD() {}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//
void LXeScintSD::Initialize(G4HCofThisEvent* hitsCE)
{
  fScintCollection =
    new LXeScintHitsCollection(SensitiveDetectorName, collectionName[0]);

  if(fHitsCID < 0)
  {
    fHitsCID = G4SDManager::GetSDMpointer()->GetCollectionID(fScintCollection);
  }
  hitsCE->AddHitsCollection(fHitsCID, fScintCollection);
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

G4bool LXeScintSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
 /* G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep == 0.)
    return false;  // No edep so don't count as hit

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable =
    (G4TouchableHistory*) (aStep->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();

  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

  // Get the average position of the hit
  G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
  pos /= 2.;

  LXeScintHit* scintHit = new LXeScintHit(thePrePV);

  scintHit->SetEdep(edep);
  scintHit->SetPos(pos);

  fScintCollection->insert(scintHit);*/

  return false;
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//
