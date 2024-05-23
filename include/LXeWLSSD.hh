#ifndef LXeWLSSD_h
#define LXeWLSSD_h 1

#include "LXeDetectorConstruction.hh"

#include "G4PVPlacement.hh"

class G4LogicalVolume;

class LXeWLSSD : public G4PVPlacement
{
 public: 
 
  LXeWLSSD(G4RotationMatrix* pRot, const G4ThreeVector& tlate, G4LogicalVolume* pMotherLogical, G4bool pMany, G4int pCopyNo, LXeDetectorConstruction* c);

  G4LogicalVolume* GetLogSensitive() { return fSensitive_log; }
 
 private:
 
  void CopyValues();

  static G4LogicalVolume* fSiPM_log;

  // Solid Volume
  //
  G4Tubs* fSiPM_tube;
  G4Tubs* fSensitive_tube;
  
  // Logical Volume
  //
  G4LogicalVolume* fSensitive_log;
  
  // Geometry
  //  
  G4double fFiber_rmin;
  G4double fFiber_rmax;
  G4double fFiber_z;
  G4double fFiber_sphi;
  G4double fFiber_ephi;

  G4double fClad1_rmin;
  G4double fClad1_rmax;
  G4double fClad1_z;
  G4double fClad1_sphi;
  G4double fClad1_ephi;

  G4double fClad2_rmin;
  G4double fClad2_rmax;
  G4double fClad2_z;
  G4double fClad2_sphi;
  G4double fClad2_ephi;

  LXeDetectorConstruction* fConstructor;
};

#endif
