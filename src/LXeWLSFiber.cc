#include "LXeWLSFiber.hh"
#include "globals.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"

G4LogicalVolume* LXeWLSFiber::fClad2_log = nullptr;

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeWLSFiber::LXeWLSFiber(G4RotationMatrix* pRot, const G4ThreeVector& tlate, G4LogicalVolume* pMotherLogical, G4bool pMany, G4int pCopyNo, LXeDetectorConstruction* c)
            : G4PVPlacement(pRot, tlate, new G4LogicalVolume(new G4Box("temp", 1., 1., 1.), G4Material::GetMaterial("Vacuum"), "temp", 0, 0, 0), "Cladding2", pMotherLogical, pMany, pCopyNo), fConstructor(c)
{
  CopyValues();

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

  // We can create the whole fiber here, we choose to create one with a core and two cladding surfaces

  // CREATE CORE FIBER
  G4Tubs* fiber_tube = new G4Tubs("Fiber", fFiber_rmin, fFiber_rmax, fFiber_z, fFiber_sphi, fFiber_ephi);
  G4LogicalVolume* fiber_log = new G4LogicalVolume(fiber_tube, G4Material::GetMaterial("PMMA"), "Fiber", 0, 0, 0);

  // CLADDING(first layer)
  G4Tubs* clad1_tube = new G4Tubs("Cladding1", fClad1_rmin, fClad1_rmax, fClad1_z, fClad1_sphi, fClad1_ephi);
  G4LogicalVolume* clad1_log = new G4LogicalVolume(clad1_tube, G4Material::GetMaterial("Pethylene1"), "Cladding1", 0, 0, 0);

  // CLADDING(second layer)
  G4Tubs* clad2_tube = new G4Tubs("Cladding2", fClad2_rmin, fClad2_rmax, fClad2_z, fClad2_sphi, fClad2_ephi);
  fClad2_log = new G4LogicalVolume(clad2_tube, G4Material::GetMaterial("Pethylene2"), "Cladding2", 0, 0, 0);

  // PLACE CORE AND CLADDING 1 INTO CLADDING 2
  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), fiber_log, "Fiber", clad1_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), clad1_log, "Cladding1", fClad2_log, false, 0);
  
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

  // Here we build the SiPM to be place inside the fibers so that I can place the fibers and SiPM together from the same class, it is an alternative way to implement the SiPM
  
  // DIMENSIONS SiPM
  G4double innerRadius_SiPM   = 0.;
  G4double height_SiPM        = 0.0635 * cm / 2.;
  G4double startAngle_SiPM    = 0.;
  G4double spanningAngle_SiPM = 360. * deg;
  G4double fOuterRadius_SiPM  = 0.1 * cm + 2*0.1*0.015* cm;	

  // CREATE SiPM GEOMETRY
  fSiPM_tube = new G4Tubs("SiPM", innerRadius_SiPM, fOuterRadius_SiPM, height_SiPM, startAngle_SiPM, spanningAngle_SiPM);
  fSensitive_tube = new G4Tubs("Sensitive", innerRadius_SiPM, fOuterRadius_SiPM, height_SiPM / 2., startAngle_SiPM, spanningAngle_SiPM);

  fSiPM_log = new G4LogicalVolume(fSiPM_tube, G4Material::GetMaterial("Glass"), "SiPM");
  fSensitive_log = new G4LogicalVolume(fSensitive_tube, G4Material::GetMaterial("Al"), "Senstive");

  // PLACE SENSITIVE PART INSIDE THE VOLUME CALLED SiPM
  new G4PVPlacement(0, G4ThreeVector(0., 0., - height_SiPM / 2.), fSensitive_log, "Sensitive", fSiPM_log, false, 0);
  
  //in this case, below you can see that I am no placing the SiPMs in the fibers, probably using another method
  //new G4PVPlacement(0, G4ThreeVector(0., 0., fFiber_z + 2 * height_sipm), fSiPM_log, "SiPM", fClad2_log, false, 0);
  
  SetLogicalVolume(fClad2_log);
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeWLSFiber::CopyValues()
{
  fFiber_rmin = 0.0 * cm;
  fFiber_rmax = 0.1 * cm;
  fFiber_z    = 1.3 * m;
  fFiber_sphi = 0.0 * deg;
  fFiber_ephi = 360. * deg;

  fClad1_rmin = 0.;  // fFiber_rmax;
  fClad1_rmax = fFiber_rmax + 0.015 * fFiber_rmax;

  fClad1_z    = fFiber_z;
  fClad1_sphi = fFiber_sphi;
  fClad1_ephi = fFiber_ephi;

  fClad2_rmin = 0.;  // fClad1_rmax;
  fClad2_rmax = fClad1_rmax + 0.015 * fFiber_rmax;

  fClad2_z    = fFiber_z;
  fClad2_sphi = fFiber_sphi;
  fClad2_ephi = fFiber_ephi;
}
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//
