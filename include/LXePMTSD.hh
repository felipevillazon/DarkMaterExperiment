#ifndef LXePMTSD_h
#define LXePMTSD_h 1

#include "LXePMTHit.hh"
#include "LXeDetectorConstruction.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include <vector>

class G4DataVector;
class G4HCofThisEvent;
class G4Step;

class LXePMTSD : public G4VSensitiveDetector
{
 public:
   
   LXePMTSD(G4String name);
  ~LXePMTSD();

  

  // A version of processHits active on boundary
  G4bool ProcessHits_boundary(const G4Step*, G4TouchableHistory*);
 private:
 virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);
};

#endif
