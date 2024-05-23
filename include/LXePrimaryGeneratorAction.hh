#ifndef LXePrimaryGeneratorAction_h
#define LXePrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "LXeMainVolume.hh"

class G4ParticleGun;
class G4Event;



class LXePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
 public:
  LXePrimaryGeneratorAction();
  ~LXePrimaryGeneratorAction();

  void GeneratePrimaries(G4Event* anEvent) override;

 private:
  G4ParticleGun* fParticleGun;
  
  G4double zmax;
  G4double zmin;
  G4double rmax;
  G4double rmin;
};

#endif
