
#ifndef LXeScintSD_h
#define LXeScintSD_h 1

#include "LXeScintHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class LXeScintSD : public G4VSensitiveDetector
{
 public:
  LXeScintSD(G4String name);
  ~LXeScintSD();

  void Initialize(G4HCofThisEvent*) override;
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*) override;

 private:
  LXeScintHitsCollection* fScintCollection;
  G4int fHitsCID;
};

#endif
