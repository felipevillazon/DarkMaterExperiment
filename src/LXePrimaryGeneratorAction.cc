#include "LXePrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXePrimaryGeneratorAction::LXePrimaryGeneratorAction()
{
 
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  G4double gammaEnergy = 7*eV; // set the energy of the gamma particle
  
  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="opticalphoton");
  fParticleGun->SetParticleDefinition(particle);
  
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  fParticleGun->SetParticleEnergy(gammaEnergy);
  
  
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXePrimaryGeneratorAction::~LXePrimaryGeneratorAction() { delete fParticleGun; }

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  
  

  
  CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
  CLHEP::HepRandom::setTheSeed((unsigned)clock());  
  
  rmin = 13.*mm+1.3*m+5.*mm+2.*cm+1.975*mm*3*mm;
  rmax = 1.4*m;
  G4double min = 3.2*m/2-350.*mm-2.6*m-20.*mm-6.*cm-3.0*mm+4*mm;
  G4double max = 3.2*m/2 -1*mm-350.*mm;
  G4double h = max-min;
  G4double sec  = h/6;
  
  zmax = max ;
  zmin = min ;
  
  
  
  G4int numPhotons = 10000;

  G4double theta = 2.0 * CLHEP::pi * 0.5;//G4UniformRand();
  G4double r = rmin + (rmax- rmin) * 0.5; //G4UniformRand();
  G4double x = std::cos(theta) *r;
  G4double y = std::sin(theta) *r;
  G4double z = zmin + (zmax - zmin) * 0.5;
  
  //G4cout << "xreal: " << x << G4endl;

for (G4int i = 0; i < numPhotons; ++i) {
  // Generate random azimuthal and polar angles
  //G4double theta = acos(2.0 * G4UniformRand() - 1.0);
  G4double phi = 2.0 * CLHEP::pi * G4UniformRand();
  G4double theta2 = 1.0*CLHEP::pi * G4UniformRand();

  // Calculate Cartesian coordinates from spherical coordinates
  G4double px = sin(theta2) * cos(phi);
  G4double py = sin(theta2) * sin(phi);
  G4double pz = cos(theta2);

  // Set the momentum direction isotropically
  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));

  // Set the position at the surface of the sphere
 
  fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));
  //fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 0));


  // Generate a single particle for each event
  fParticleGun->GeneratePrimaryVertex(anEvent);

}
/*for (G4int i = 0; i < numPhotons; ++i) {
  // Generate random azimuthal and polar angles
  //G4double theta = acos(2.0 * G4UniformRand() - 1.0);
  //G4double phi = 2.0 * CLHEP::pi * G4UniformRand();

  // Calculate Cartesian coordinates from spherical coordinates
  G4double px = 1;
  G4double py = 0;
  G4double pz = 0;

  // Set the momentum direction isotropically
  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));

  // Set the position at the surface of the sphere
 
  fParticleGun->SetParticlePosition(G4ThreeVector(1.37*m, 0, 0));

  // Generate a single particle for each event
  fParticleGun->GeneratePrimaryVertex(anEvent);
  
  }*/
  
 
}


