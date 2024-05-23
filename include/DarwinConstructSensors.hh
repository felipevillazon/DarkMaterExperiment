#ifndef DARWINCONSTRUCTSENSORS_H
#define DARWINCONSTRUCTSENSORS_H

//DARWIN header files
#include "LXeDetectorConstruction.hh"


//G4 Header Files
#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>

#include "G4Tubs.hh"
#include "G4PVPlacement.hh"

class DarwinConstructSensors{

public:
  DarwinConstructSensors(LXeMainVolume*);
  ~DarwinConstructSensors();

  G4LogicalVolume* Construct( G4double Sensordisk_thickness,
                              G4double Sensordisk_radius,
                              G4double SensorWindowThickness,
                              G4String SensorName);

private:



  //Materials
  G4Material *SensorDisk_material;

  //Parameters
  G4double Thickness_Sensor;
  G4double outer_RadiusSensor;
  G4double dWindowThickness;

  //Solids
  G4Tubs *Sensor_disk;

  //LogicalVolumes
  G4LogicalVolume* Sensor_LogicalVolume;
  G4LogicalVolume* pSensorWindowLogical;
  //G4LogicalVolume* pSensorSensVolume_Logical;

  // Physical volumes
  //G4VPhysicalVolume *pSensorSensVolume_Physical;
  G4VPhysicalVolume *pSensorWindowPhysical;
  //Others

};

#endif
