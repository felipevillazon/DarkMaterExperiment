#include "LXeWLSSD.hh"
#include "globals.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

G4LogicalVolume* LXeWLSSD::fSiPM_log = nullptr;

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeWLSSD::LXeWLSSD(G4RotationMatrix* pRot, const G4ThreeVector& tlate, G4LogicalVolume* pMotherLogical, G4bool pMany, G4int pCopyNo, LXeDetectorConstruction* c)
         : G4PVPlacement(pRot, tlate, new G4LogicalVolume(new G4Box("temp", 1., 1., 1.), G4Material::GetMaterial("Vacuum"), "temp", 0, 0, 0), "Sensitive", pMotherLogical, pMany, pCopyNo), fConstructor(c)
{
  CopyValues();
  
  
// In this class, we are creating SiPMs, so we can use the G4PVPlacement class to place them in the simulation
// Here we are creating a class that should return the placement of the defined geometry in the class
  
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//
  
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
  //new G4PVPlacement(0, G4ThreeVector(0., 0., - height_SiPM / 2.), fSensitive_log, "Sensitive", fSiPM_log, false, 0);

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

  // OPTICAL SURFACE PROPERTIES
  /*std::vector<G4double> ephoton = { 7.0 * eV, 7.14 * eV };
  std::vector<G4double> sipm_EFF     = { 1., 1. };
  std::vector<G4double> sipm_ReR     = { 1.92, 1.92 };
  std::vector<G4double> sipm_ImR     = { 1.69, 1.69 };
  G4MaterialPropertiesTable* sipm_mt = new G4MaterialPropertiesTable();
  sipm_mt->AddProperty("EFFICIENCY", ephoton, sipm_EFF);
  sipm_mt->AddProperty("REALRINDEX", ephoton, sipm_ReR);
  sipm_mt->AddProperty("IMAGINARYRINDEX", ephoton, sipm_ImR);
  G4OpticalSurface* sipm_opsurf = new G4OpticalSurface("SiPM_opsurf", glisur, polished, dielectric_metal);
  sipm_opsurf->SetMaterialPropertiesTable(sipm_mt);
  
  new G4LogicalSkinSurface("SIPM_surf", fSensitive_log, sipm_opsurf);*/

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

  SetLogicalVolume(fSiPM_log);
  
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeWLSSD::CopyValues()
{
  /*fFiber_rmin = 0.0 * cm;
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
  fClad2_ephi = fFiber_ephi;*/
}
