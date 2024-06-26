#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4String.hh"
#include "G4TouchableHistory.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

class LXeSensitiveDetector : public G4VSensitiveDetector
{
public:
	LXeSensitiveDetector(G4String);
	~LXeSensitiveDetector();
private:
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif
