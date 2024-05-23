#include "DarwinConstructSensors.hh"

DarwinConstructSensors::DarwinConstructSensors(LXeMainVolume*){;}

DarwinConstructSensors::~DarwinConstructSensors() {;}

G4LogicalVolume* DarwinConstructSensors::Construct(
                              G4double Sensordisk_thickness,
                              G4double Sensordisk_radius,
                              G4double SensorWindowThickness,
                              G4String SensorName){


    Thickness_Sensor = Sensordisk_thickness;
    outer_RadiusSensor = Sensordisk_radius;
    dWindowThickness = SensorWindowThickness;

    SensorDisk_material  = G4Material::GetMaterial("SensorDisk_material");

    // Objects
    Sensor_disk = new G4Tubs("Sensor_disk", 0.*cm, outer_RadiusSensor, Thickness_Sensor/2, 0.*deg, 360.*deg);
    G4cout << "DarwinConstructSensors::Construct making sensor "<<SensorName<<" with window thickness: " << dWindowThickness << " mm"<<G4endl;
    G4Tubs* pSensorWindow = new G4Tubs("SensorWindow_disk", 0.*cm, outer_RadiusSensor,
                                           dWindowThickness/2, 0.*deg, 360.*deg);
    //G4Tubs* pSensorSensVolume = new G4Tubs("SensorSens_disk", 0.*cm, outer_RadiusSensor,
    //                                       (Thickness_Sensor -dWindowThickness)/2,
    //                                       0.*deg, 360.*deg);
    //LogicalVolumes
    Sensor_LogicalVolume = new G4LogicalVolume(Sensor_disk, SensorDisk_material, "Sensor_Logical", 0, 0, 0);  
    pSensorWindowLogical = new G4LogicalVolume(pSensorWindow, SensorDisk_material, "SensorWindow_Logical", 0, 0, 0); 
    //pSensorSensVolume_Logical = new G4LogicalVolume(pSensorSensVolume, SensorDisk_material, "SensorSens_Logical", 0, 0, 0);
    
 
    pSensorWindowPhysical = new G4PVPlacement(0,G4ThreeVector(0.,0., -Thickness_Sensor/2 + dWindowThickness/2),
                                                                 pSensorWindowLogical, SensorName+"Window", Sensor_LogicalVolume,
                                                                     false, 0);
    //pSensorSensVolume_Physical = new G4PVPlacement(0,G4ThreeVector(0.,0., dWindowThickness/2),
    //                                                                 pSensorSensVolume_Logical, SensorName+"Sensitive", Sensor_LogicalVolume,
    //                                                                 false, 0);
    //Visualization Settings

    
    return Sensor_LogicalVolume;
    
}
