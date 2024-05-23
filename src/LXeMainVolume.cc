#include "LXeMainVolume.hh"
#include "LXeWLSSD.hh"
#include "LXeWLSFiber.hh"
#include "globals.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "LXeWLSSD.hh" 
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Torus.hh" 
#include <G4SystemOfUnits.hh>
#include "G4LogicalBorderSurface.hh"


//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeMainVolume::LXeMainVolume(G4RotationMatrix* pRot, const G4ThreeVector& tlate, G4LogicalVolume* pMotherLogical, G4bool pMany, G4int pCopyNo, LXeDetectorConstruction* c)
              // Pass info to the G4PVPlacement constructor 
              : G4PVPlacement(pRot, tlate,
              // Temp logical volume must be created here
              new G4LogicalVolume(new G4Box("temp", 1, 1, 1), G4Material::GetMaterial("Vacuum"), "temp", 0, 0, 0), "MainWall", pMotherLogical, pMany, pCopyNo), fConstructor(c)
{
  CopyValues();

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

  // DIMESIONS WALL OF THE MAIN VOLUME OR DIMENSIONS ROOM WHERE WE PLACE OUR EXPERIMENT  
  G4double fMainWall_x = fMain_x + 2. * fMainWall_thick;
  G4double fMainWall_y = fMain_y + 2. * fMainWall_thick;
  G4double fMainWall_z = fMain_z + 2. * fMainWall_thick;
  
  using std::stringstream;
  stringstream name;
  
  // DEFINE MAIN AND MAINWALL VOLUME
  fMain_box = new G4Box("Main", fMain_x / 2., fMain_y / 2., fMain_z / 2.);
  fMainWall_box = new G4Box("MainWall", fMainWall_x / 2., fMainWall_y / 2., fMainWall_z / 2.);

  fMain_Logical   = new G4LogicalVolume(fMain_box, G4Material::GetMaterial("Vacuum"), "Main", 0, 0, 0);
  fMainWall_Logical = new G4LogicalVolume(fMainWall_box, G4Material::GetMaterial("Vacuum"), "MainWall", 0, 0, 0);
  
  // PLACE MAIN VOLUME INSIDE MAINWALL
  new G4PVPlacement(0, G4ThreeVector(), fMain_Logical, "Main", fMainWall_Logical, false, 0);

  
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//
  
  // An option is to create and place the SiPM in the Main Volume
  
  // DIMENSIONS SiPM
  G4double innerRadius_SiPM   = 0.;
  G4double height_SiPM        = 0.1 * cm / 2.;
  G4double startAngle_SiPM    = 0.;
  G4double spanningAngle_SiPM = 360. * deg;
  G4double fOuterRadius_SiPM  = 0.1 * cm + 2*0.1*0.015* cm;	

  // CREATE SiPM GEOMETRY
  fSiPM_tube = new G4Tubs("SiPM", innerRadius_SiPM, fOuterRadius_SiPM, height_SiPM, startAngle_SiPM, spanningAngle_SiPM);
  fSensitive_tube = new G4Tubs("Sensitive", innerRadius_SiPM, fOuterRadius_SiPM, height_SiPM / 8., startAngle_SiPM, spanningAngle_SiPM);

  fSiPM_log = new G4LogicalVolume(fSiPM_tube, G4Material::GetMaterial("Glass"), "SiPM");
  fSensitive_log = new G4LogicalVolume(fSensitive_tube, G4Material::GetMaterial("Air"), "Sensitive");

  // PLACE SENSTIVE PART OF SiPM INSIDE THE SiPM LOGICAL VOLUME
  //Sensitive = new G4PVPlacement(0, G4ThreeVector(0., 0., -height_SiPM+ height_SiPM / 8.), fSensitive_log, "Sensitive", fSiPM_log, false, 0);

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

  Tube1 = new G4Tubs("Tube1", 0., fOuterRadius_SiPM+fOuterRadius_SiPM/95, height_SiPM/4, startAngle_SiPM, spanningAngle_SiPM);
  fShield_tube = new G4SubtractionSolid("Shield", Tube1, fSensitive_tube,0,G4ThreeVector(0., 0., -height_SiPM/8 ));
  fShield_log = new G4LogicalVolume(fShield_tube, G4Material::GetMaterial("Teflon"), "Shield");
  
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

  k = 0;
  
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

  // Inside our Main Volume we place our InnerCryostat. For this, we could have also created another class to create the InnerCryostat and then place it here
  // Here, we decided to implement the geometry directly into the MainVolume class  
 
  // Construct inner Cryostat as solidly filled object: Top Torispheric, Cylinder, Bottom Torispheric, Flange, # of Hoop stiffener --> Solidunion InnerCryo
  G4UnionSolid *InnerCryostatVessel_Solid = BuildVesselShape( InnerCryo_InnerCylDiameter, InnerCryo_InnerCylHeight, InnerCryo_WallThickness);
  
  G4Tubs *InnerCryostatFlange = new G4Tubs("InnerCryostatFlange", InnerCryo_InnerCylDiameter/2, 
   					     InnerCryo_InnerCylDiameter/2 + InnerCryo_WallThickness + InnerCryo_FlangeThickness, InnerCryo_FlangeHeight/2 , 0.*deg, 360.*deg);
  InnerCryostatVessel_Solid = new G4UnionSolid("InnerCryostat", InnerCryostatVessel_Solid, InnerCryostatFlange, 0, G4ThreeVector(0.,0.,InnerCryo_InnerCylHeight/2-InnerCryo_FlangeHeight/2));
  G4Tubs *InnerCryostatStiffener = new G4Tubs("InnerCryostatStiffener", InnerCryo_InnerCylDiameter/2, 
   					        InnerCryo_InnerCylDiameter/2 + InnerCryo_WallThickness + InnerCryo_StiffenerThickness, InnerCryo_StiffenerHeight/2 , 0.*deg, 360.*deg);   
  for (int a=0; a < InnerCryo_StiffenerNumber; ++a)
   {  
     G4double InnerCryoStiffener_ZStep = InnerCryo_InnerCylHeight/(InnerCryo_StiffenerNumber+1);
     InnerCryostatVessel_Solid = new G4UnionSolid("InnerCryostat", InnerCryostatVessel_Solid, InnerCryostatStiffener, 0, 
					            G4ThreeVector(0.,0.,-InnerCryo_InnerCylHeight/2 + (a+1) * InnerCryoStiffener_ZStep - InnerCryo_StiffenerHeight/2));
   }

  InnerCryostatVessel_Logical = new G4LogicalVolume(InnerCryostatVessel_Solid, G4Material::GetMaterial("SS304LSteel"), "InnerCryostatVessel", 0, 0, 0);
 
 
  // Construct GXe in the Inner Cryostat, leaving the Inner Cryostat Vessel as a shell of given thickness		
  G4UnionSolid *InnerCryostatVolume_Solid = BuildVesselShape( InnerCryo_InnerCylDiameter, InnerCryo_InnerCylHeight, 0.);
  InnerCryostatVolume_Logical = new G4LogicalVolume(InnerCryostatVolume_Solid, G4Material::GetMaterial("GXe"), "InnerCryostatVolume", 0, 0, 0);
  InnerCryostatVolume_Physical = new G4PVPlacement(0, G4ThreeVector(), InnerCryostatVolume_Logical,"GXe_InnerCryostatVolume",  InnerCryostatVessel_Logical, false, 0);


  // Construct the (vacuumized) Bottom filler: Fill Bottom with LXe, Construct CryoMaterial Filler, Replace inner material with Vacuum	
  G4SubtractionSolid *InnerCryostatBottomVolume_Solid = BuildFillerShape( InnerCryo_InnerCylDiameter, InnerCryo_InnerCylHeight, 0.);
  InnerCryostatBottomVolume_Logical = new G4LogicalVolume(InnerCryostatBottomVolume_Solid, G4Material::GetMaterial("LXe"), "InnerCryostatBottomVolume", 0, 0, 0);
  InnerCryostatBottomVolume_Physical = new G4PVPlacement(0, G4ThreeVector(), InnerCryostatBottomVolume_Logical,"LXe_InnerCryostatBottomVolume",  InnerCryostatVolume_Logical, false, 0);

  G4SubtractionSolid *InnerCryostatBottomFillerVessel_Solid = BuildFillerShape( InnerCryo_InnerCylDiameter, InnerCryo_InnerCylHeight, - InnerCryo_WallThickness);
  InnerCryostatBottomFillerVessel_Logical = new G4LogicalVolume(InnerCryostatBottomFillerVessel_Solid, G4Material::GetMaterial("SS304LSteel"), "InnerCryostatBottomFillerVessel", 0, 0, 0);
  InnerCryostatBottomFillerVessel_Physical = new G4PVPlacement(0, G4ThreeVector(), InnerCryostatBottomFillerVessel_Logical,"CryoMat_InnerCryostatBottomFillerVessel",  InnerCryostatBottomVolume_Logical,    
                                                               false, 0);

  G4SubtractionSolid *InnerCryostatBottomFillerVolume_Solid = BuildFillerShape( InnerCryo_InnerCylDiameter, InnerCryo_InnerCylHeight, - InnerCryo_WallThickness - 5. *mm);
  InnerCryostatBottomFillerVolume_Logical = new G4LogicalVolume(InnerCryostatBottomFillerVolume_Solid, G4Material::GetMaterial("Vacuum"), "InnerCryostatBottomFillerVolume", 0, 0, 0);
  InnerCryostatBottomFillerVolume_Physical = new G4PVPlacement(0, G4ThreeVector(), InnerCryostatBottomFillerVolume_Logical,"Vacuum_InnerCryostatBottomFillerVolume",  
  InnerCryostatBottomFillerVessel_Logical, false, 0);
   
  InnerCryostatVessel_Physical = new G4PVPlacement(0, G4ThreeVector(), InnerCryostatVessel_Logical,"CryoMat_InnerCryostatVessel",fMain_Logical, false, 0);

 
      
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

  // We need to have at least the cover of the TPC so we can simulate what could happen between the TPC and the InnerCryostat
  
  // DIMENSIONS TPC
  
    // Parameters Cryostat, TPC and Teflon
  dActiveLXeRadius = 1.3*m; 
  dActiveLXeHeight = 2.6*m;
  dGXeHeight = 350.*mm;
  dLXeExtraFilling = 1.*mm;

  dReflectorThickness = 3.0*mm;  
  dReflectorBottomHeight = 6.*cm;
  dPillarThickness = 2.*cm; 
  dPillarWidth = 3.*cm;
  iPillarNb = 24;

  dFSRThickness = 5.*mm;
  dFSRHeight = 20.*mm;
  dFSRInnerRadius = dActiveLXeRadius + 13.*mm;
  iFSRNb = 92.;
  dFSRSeparation = 28.*mm;
  dFSROffsetZ = dFSRHeight/2 + 12*mm;

  dPMTRadiusBody = 26.65*mm; 
  dPMTHeight = 114.*mm;
  dPTFEArrayReflectorThickness = 4.0*mm;
  dPTFEArrayHolderThickness = 2.5*mm;
  dBasesRadius = 25*mm;
  dBasesThickness = 1.068*mm; 

  dCuSensorArrayThickness = 15.*mm;
  dCuSensorArrayHoleRadius = 35.5*mm;
  dSensorArrayRadius = dActiveLXeRadius + 5.*cm;

  dBasesArrayDiskThickness = 0.376*mm; 
  dCuSensorArrayDiskThickness = 5.5*mm;
  dPTFESensorArrayDiskThickness = 3.*mm;
  
  dElectrodesFrameThickness = 38.*mm;
  dElectrodesTopFrameHeight = 60.*mm;
  dCathodeFrameHeight = 20.*mm;
  dBottomscreenFrameHeight = 8.*mm;
  dTopScreeningFrameHeight = 8.0*mm;
  // Electrode parameters 
  dTopScreeningThickness = 0.2*mm;
  dAnodeThickness =  0.2*mm;
  dGateAnodeDistance = 8.0*mm;

  dGateThickness =0.2*mm;
  dCathodeThickness = 0.2*mm;
  dBottomScreeningThickness = 0.2*mm; 
  dSensorThickness = 20.*mm;  
  dSensorWindowThickness = 0.1*mm;  
  
  dCryostatInnerRadius = 1.4*m;
  dCryostatInnerHeight = 3.2*m;
//---------------------------------------------------CONSTRUCT-------------------------------------------------------

  pTPCCylinder = new G4Tubs("TPCCylinder", 0.*cm, dCryostatInnerRadius, dCryostatInnerHeight/2, 0.*deg, 360.*deg);  
  pGXeCylinder = new G4Tubs("GXeCylinder", 0.*cm, dCryostatInnerRadius, dGXeHeight/2, 0.*deg, 360.*deg);
  pLXeExtraFillingInside = new G4Tubs("LXeExtraFillingInside", 0.*cm, dActiveLXeRadius, dLXeExtraFilling/2, 0.*deg, 360.*deg);
  pLXeExtraFillingOutside = new G4Tubs("LXeExtraFillingOutside", dActiveLXeRadius + dElectrodesFrameThickness, dCryostatInnerRadius, dLXeExtraFilling/2, 0.*deg, 360.*deg);
  pLXeSensitive = new G4Tubs("LXeSensitive", 0.*cm, dActiveLXeRadius, dActiveLXeHeight/2, 0.*deg, 360.*deg);

  pPTFEReflector = new G4Tubs("PTFEReflector", dActiveLXeRadius, dActiveLXeRadius + dReflectorThickness, dActiveLXeHeight/2, 0.*deg, 360.*deg);
  pPTFEReflectorTop = new G4Tubs("PTFEReflectorTop", dActiveLXeRadius, dActiveLXeRadius + dReflectorThickness, dElectrodesTopFrameHeight/2, 0.*deg, 360.*deg);
  pPTFEReflectorBottom = new G4Tubs("PTFEReflectorBottom", dActiveLXeRadius, dActiveLXeRadius + dReflectorThickness, dReflectorBottomHeight/2, 0.*deg, 360.*deg); 
  pPTFEPillar = new G4Box("PTFEPillar", dPillarWidth/2, dPillarThickness/2, dActiveLXeHeight/2);
  pPTFEPillarBottom = new G4Box("PTFEPillarBottom", dPillarWidth/2, dPillarThickness/2, dReflectorBottomHeight/2);

  pTiElectrodesTopFrame = new G4Tubs("TiElectrodesTopFrame", dActiveLXeRadius + dReflectorThickness, dActiveLXeRadius + dReflectorThickness + dElectrodesFrameThickness - dReflectorThickness, 
  dElectrodesTopFrameHeight/2, 0.*deg, 360.*deg);  
  pTiCathodeFrame = new G4Tubs("TiCathodeFrame", dActiveLXeRadius, dActiveLXeRadius + dElectrodesFrameThickness, dCathodeFrameHeight/2, 0.*deg, 360.*deg); 
  pTiBottomscreenFrame = new G4Tubs("TiBottomscreenFrame", dActiveLXeRadius, dActiveLXeRadius + dElectrodesFrameThickness, dBottomscreenFrameHeight/2, 0.*deg, 360.*deg);
  
  pCuFSR = new G4Tubs("CuFSR", dFSRInnerRadius, dFSRInnerRadius + dFSRThickness, dFSRHeight/2, 0.*deg, 360.*deg);

  pCirlexBases = new G4Tubs("CirlexBases", 0.*cm, dBasesRadius, dBasesThickness/2, 0.*deg, 360.*deg);
  pCuSensorArray = new G4Tubs("CuSensorArray", 0.*cm, dSensorArrayRadius, dCuSensorArrayThickness/2, 0.*deg, 360.*deg);
  pPTFESensorArrayThin = new G4Tubs("PTFESensorArrayThin", 0.*cm, dSensorArrayRadius, dPTFEArrayReflectorThickness/2, 0.*deg, 360.*deg);
  pPTFESensorArrayThick = new G4Tubs("PTFESensorArrayThick", 0.*cm, dSensorArrayRadius, dPTFEArrayHolderThickness/2, 0.*deg, 360.*deg);

  // electrodes
  G4Tubs *pTopScreeningElectrode = new G4Tubs("TopScreeningElectrode", 0., dActiveLXeRadius, 0.5 *dTopScreeningThickness , 0, 2 * M_PI);
  G4Tubs *pAnodeElectrode = new G4Tubs("AnodeElectrode", 0., dActiveLXeRadius, 0.5 *dAnodeThickness , 0, 2 * M_PI);
  G4Tubs *pGateElectrode = new G4Tubs("GateElectrode", 0., dActiveLXeRadius, 0.5 *dGateThickness , 0, 2 * M_PI);
  G4Tubs *pCathodeElectrode = new G4Tubs("CathodeElectrode", 0., dActiveLXeRadius, 0.5 *dCathodeThickness , 0, 2 * M_PI);
  G4Tubs *pBottomScreeningElectrode = new G4Tubs("BottomScreeningElectrode", 0., dActiveLXeRadius, 0.5 *dBottomScreeningThickness , 0, 2 * M_PI);
  // 
  pCirlexBasesArrayDisk = new G4Tubs("CirlexBases", 0.*cm, dActiveLXeRadius, dBasesArrayDiskThickness/2, 0.*deg, 360.*deg);
  pCuSensorArrayDisk = new G4Tubs("CuSensorArray", 0.*cm, dActiveLXeRadius, dCuSensorArrayDiskThickness/2, 0.*deg, 360.*deg);
  pPTFESensorArrayDisk = new G4Tubs("PTFESensorArray", 0.*cm, dActiveLXeRadius, dPTFESensorArrayDiskThickness/2, 0.*deg, 360.*deg);

//-------------------------------------------------LOGICALVOLUMES----------------------------------------------------

  m_pTPCLogical = new G4LogicalVolume(pTPCCylinder, G4Material::GetMaterial("LXe"), "TPCVolume", 0, 0, 0);
  m_pLXeLogical = new G4LogicalVolume(pTPCCylinder, G4Material::GetMaterial("LXe"), "LXeVolume", 0, 0, 0);
  m_pGXeLogical = new G4LogicalVolume(pGXeCylinder, G4Material::GetMaterial("GXe"), "GXeVolume", 0, 0, 0);
  m_pLXeExtraFillingInsideLogical = new G4LogicalVolume(pLXeExtraFillingInside, G4Material::GetMaterial("LXe"), "LXeExtraFillingInsideVolume", 0, 0, 0);
  m_pLXeExtraFillingOutsideLogical = new G4LogicalVolume(pLXeExtraFillingOutside, G4Material::GetMaterial("LXe"), "LXeExtraFillingOutsideVolume", 0, 0, 0);
  m_pLXeSensitiveLogical = new G4LogicalVolume(pLXeSensitive, G4Material::GetMaterial("LXe"), "LXeSensitiveVolume", 0, 0, 0);

  m_pPTFEReflectorLogical = new G4LogicalVolume(pPTFEReflector, G4Material::GetMaterial("Teflon"), "PTFEReflectorVolume", 0, 0, 0);
  m_pPTFEReflectorTopLogical = new G4LogicalVolume(pPTFEReflectorTop, G4Material::GetMaterial("Teflon"), "PTFEReflectorTopVolume", 0, 0, 0);
  m_pPTFEReflectorBottomLogical = new G4LogicalVolume(pPTFEReflectorBottom, G4Material::GetMaterial("Teflon"), "PTFEReflectorBottomVolume", 0, 0, 0);
  m_pPTFEPillarLogical = new G4LogicalVolume(pPTFEPillar, G4Material::GetMaterial("Teflon"), "PTFEPillarVolume", 0, 0, 0);
  m_pPTFEPillarBottomLogical = new G4LogicalVolume(pPTFEPillarBottom, G4Material::GetMaterial("Teflon"), "PTFEPillarBottomVolume", 0, 0, 0);

  m_pTiElectrodesTopFrameLogical = new G4LogicalVolume(pTiElectrodesTopFrame, G4Material::GetMaterial("TiGrade1"), "TiElectrodesTopFrameVolume", 0, 0, 0);
  m_pTiCathodeFrameLogical = new G4LogicalVolume(pTiCathodeFrame, G4Material::GetMaterial("TiGrade1"), "TiCathodeFrameVolume", 0, 0, 0);
  m_pTiBottomscreenFrameLogical = new G4LogicalVolume(pTiBottomscreenFrame, G4Material::GetMaterial("TiGrade1"), "TiBottomscreenFrameVolume", 0, 0, 0);

  // electrodes 
  G4LogicalVolume* m_pTopScreeningLogical = new G4LogicalVolume(pTopScreeningElectrode, G4Material::GetMaterial("TopScreeningMesh"), "TopScreeningElectrodeLogicalVolume", 0, 0, 0);
  G4LogicalVolume* m_pAnodeLogical = new G4LogicalVolume(pAnodeElectrode, G4Material::GetMaterial("AnodeMesh"), "AnodeElectrodeLogicalVolume", 0, 0, 0);
  G4LogicalVolume* m_pGateLogical = new G4LogicalVolume(pGateElectrode, G4Material::GetMaterial("GateMesh"), "GateElectrodeLogicalVolume", 0, 0, 0);
  G4LogicalVolume* m_pCathodeLogical = new G4LogicalVolume(pCathodeElectrode, G4Material::GetMaterial("CathodeMesh"), "CathodeElectrodeLogicalVolume", 0, 0, 0);
  G4LogicalVolume* m_pBottomScreeningLogical = new G4LogicalVolume(pBottomScreeningElectrode, G4Material::GetMaterial("BottomScreeningMesh"), "BottomScreeningElectrodeLogicalVolume", 0, 0, 0);
  // 
  m_pCirlexBasesArrayDiskLogical = new G4LogicalVolume(pCirlexBasesArrayDisk, G4Material::GetMaterial("Cirlex"), "CirlexBasesVolume", 0, 0, 0);
  m_pCuArrayDiskLogical = new G4LogicalVolume(pCuSensorArrayDisk, G4Material::GetMaterial("Copper"), "CuArrayVolume", 0, 0, 0);
  m_pPTFEArrayDiskLogical = new G4LogicalVolume(pPTFESensorArrayDisk, G4Material::GetMaterial("Teflon"), "PTFEArrayVolume", 0, 0, 0);
  
  m_pCuFSRLogical = new G4LogicalVolume(pCuFSR, G4Material::GetMaterial("Copper"), "CuFSRVolume", 0, 0, 0);

  m_pCirlexBasesLogical = new G4LogicalVolume(pCirlexBases, G4Material::GetMaterial("Cirlex"), "CirlexBasesVolume", 0, 0, 0);
  m_pCuArrayBottomLogical = new G4LogicalVolume(pCuSensorArray, G4Material::GetMaterial("Copper"), "CuArrayThinBottomVolume", 0, 0, 0);



//-------------------------------------------------PHYSICALVOLUME-----------------------------------------------------

  // Filling TPC Volume with LXe
  m_pLXePhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., 0.), m_pLXeLogical,"LXeTPC", m_pTPCLogical, false, 0);
  
  m_pGXePhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCryostatInnerHeight/2 - dGXeHeight/2), m_pGXeLogical,"GXeTPC", m_pLXeLogical, false, 0);
  
  // Placing all TPC components fully emerged in LXe
  m_pPTFEReflectorPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight/2), m_pPTFEReflectorLogical, "PTFEReflector", m_pLXeLogical, false, 0);
  m_pPTFEReflectorBottomPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight - dCathodeFrameHeight - dReflectorBottomHeight/2), m_pPTFEReflectorBottomLogical, "PTFEReflectorBottom", m_pLXeLogical, false, 0);
  m_pTiCathodeFramePhysical = new G4PVPlacement(0, G4ThreeVector(0., 0., dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight - dCathodeFrameHeight/2), m_pTiCathodeFrameLogical, "TiCathodeFrame", m_pLXeLogical, false, 0);
  m_pTiBottomscreenFramePhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight - dCathodeFrameHeight- dReflectorBottomHeight - dBottomscreenFrameHeight/2), m_pTiBottomscreenFrameLogical, "TiBottomscreenFrame", m_pLXeLogical, false, 0);

  // Placing Field Shaping Rings (FSR)
  for(int a=0; a < iFSRNb; ++a)
  {
    G4double dFSRZStep = dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight + dFSROffsetZ + a*dFSRSeparation;
    name.str("");
    name << "CuFSR_" << a;
    m_pCuFSRPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0.,dFSRZStep), m_pCuFSRLogical, name.str(), m_pLXeLogical, false, 0);
  }

  // Placing Teflon pillars around active TPC and around bottom part
  stringstream PTFEPillarMain;
  stringstream PTFEPillarBottom;
  for (int a=0; a < iPillarNb; ++a)
  { 
    G4double dRotateAngle = 90.0 + (a*(360./iPillarNb)); 
    G4RotationMatrix* dPillarRotation = new G4RotationMatrix; 
    dPillarRotation->rotateZ(dRotateAngle*deg); 

    G4double dDistanceRadial = dFSRInnerRadius + dFSRThickness + dPillarThickness/2;
    G4double dPillarsXStep = sin(dRotateAngle*deg)*dDistanceRadial; 
    G4double dPillarsYStep = cos(dRotateAngle*deg)*dDistanceRadial; 

    PTFEPillarMain.str("");
    PTFEPillarMain << "PTFEPillarMain_" << a;
    m_pPTFEPillarPhysical = new G4PVPlacement(dPillarRotation, G4ThreeVector(dPillarsXStep, dPillarsYStep, dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight/2), m_pPTFEPillarLogical, PTFEPillarMain.str(), m_pLXeLogical, false, 0);

    PTFEPillarBottom.str("");
    PTFEPillarBottom << "PTFEPillarBottom_" << a;
    m_pPTFEPillarBottomPhysical = new G4PVPlacement(dPillarRotation, G4ThreeVector(dPillarsXStep, dPillarsYStep, dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight - dCathodeFrameHeight - dReflectorBottomHeight/2), m_pPTFEPillarBottomLogical, PTFEPillarBottom.str(), m_pLXeLogical, false, 0);
  }

  // Placing all TPC components in GXe
  m_pPTFEReflectorTopPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., -dGXeHeight/2 + dElectrodesTopFrameHeight/2),	m_pPTFEReflectorTopLogical, "PTFEReflectorTop", m_pGXeLogical, false, 0);
  m_pTiElectrodesTopFramePhysical = new G4PVPlacement(0, G4ThreeVector(0., 0., -dGXeHeight/2 + dElectrodesTopFrameHeight/2), m_pTiElectrodesTopFrameLogical, "TiElectrodesTopFrame", m_pGXeLogical, false, 0);

  // Adding extra LXe filling above gate-level (replacing GXe) 
  m_pLXeExtraFillingInsidePhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., - dGXeHeight/2 + dLXeExtraFilling/2), m_pLXeExtraFillingInsideLogical, "LXeExtraFillingInside", m_pGXeLogical, false, 0);
  m_pLXeExtraFillingOutsidePhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., - dGXeHeight/2 + dLXeExtraFilling/2), m_pLXeExtraFillingOutsideLogical, "LXeExtraFillingOutside", m_pGXeLogical, false, 0);

  // Adding sensitive volume between cathode and gate
  m_pLXeSensitivePhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight/2), m_pLXeSensitiveLogical, "LXeSensitive", m_pLXeLogical, false, 0);
  
  G4RotationMatrix* rotX180 = new G4RotationMatrix; 
  rotX180->rotateX(180.0*deg);   
  //Placing components in top sensor array (emerged in GXe)
  //We need to set number of PMTs here
  
    G4double dCuArrayDiskTopZOffset = - dGXeHeight/2 + dElectrodesTopFrameHeight + dSensorThickness 
                                      + dBasesArrayDiskThickness + dCuSensorArrayDiskThickness/2;
    m_pCuArrayDiskTopPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCuArrayDiskTopZOffset), 
                                                  m_pCuArrayDiskLogical, "CuArrayTop", m_pGXeLogical, 
                                                  false, 0); 

    G4double dPTFEArrayDiskTopZOffset = - dGXeHeight/2 + dElectrodesTopFrameHeight + dSensorThickness 
                                        + dBasesArrayDiskThickness + dCuSensorArrayDiskThickness 
                                        + dPTFESensorArrayDiskThickness/2;
    m_pPTFEArrayDiskTopPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dPTFEArrayDiskTopZOffset), 
                                                    m_pPTFEArrayDiskLogical, "PTFEArrayTop", m_pGXeLogical, 
                                                    false, 0);

    G4double dCirlexArrayDiskTopZOffset = - dGXeHeight/2 + dElectrodesTopFrameHeight 
                                          + dSensorThickness + dBasesArrayDiskThickness/2; 
    m_pCirlexBasesDiskTopPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCirlexArrayDiskTopZOffset), 
                                                      m_pCirlexBasesArrayDiskLogical, "CirlexBasesTop", m_pGXeLogical, 
                                                      false, 0);

    DarwinConstructSensors *sensorsTop = new DarwinConstructSensors(this);
    m_pPMTArrayTopLogical = sensorsTop->Construct(dSensorThickness, dActiveLXeRadius, 
                                                  dSensorWindowThickness, "PMTArrayTop");
    G4double dPMTArrayTopZOffset = -dGXeHeight/2 + dElectrodesTopFrameHeight + dSensorThickness/2;
    m_pPMTArrayTopPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dPMTArrayTopZOffset),
                                               m_pPMTArrayTopLogical, "PMTArrayTop", m_pGXeLogical,false, 0);
 

    G4double dCuArrayDiskBottomZOffset = dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight 
                                       - dCathodeFrameHeight - dReflectorBottomHeight - dBottomscreenFrameHeight 
                                       - dSensorThickness - dBasesArrayDiskThickness - dCuSensorArrayDiskThickness/2; 
    m_pCuArrayDiskBottomPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCuArrayDiskBottomZOffset), 
                                                     m_pCuArrayDiskLogical, "CuArrayBottom", m_pLXeLogical, 
                                                     false, 0);
    G4double dPTFEArrayDiskBottomZOffset = dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight 
                                         - dCathodeFrameHeight - dReflectorBottomHeight - dBottomscreenFrameHeight 
                                         - dSensorThickness - dBasesArrayDiskThickness - dCuSensorArrayDiskThickness
                                         - dPTFESensorArrayDiskThickness/2;
    m_pPTFEArrayDiskBottomPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dPTFEArrayDiskBottomZOffset), 
                                                       m_pPTFEArrayDiskLogical, "PTFEArrayBottom", m_pLXeLogical, 
                                                       false, 0);
    G4double dCirlexArrayDiskBottomZOffset = dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight
                                           - dCathodeFrameHeight - dReflectorBottomHeight - dBottomscreenFrameHeight 
                                           - dSensorThickness - dBasesArrayDiskThickness/2; 
    m_pCirlexBasesDiskBottomPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCirlexArrayDiskBottomZOffset), 
                                                         m_pCirlexBasesArrayDiskLogical, "CirlexBasesBottom", m_pLXeLogical, 
                                                         false, 0);

    DarwinConstructSensors *sensorsBottom = new DarwinConstructSensors(this);
    m_pPMTArrayBottomLogical = sensorsBottom->Construct(dSensorThickness, dActiveLXeRadius, dSensorWindowThickness, "PMTArrayBottom");
    G4double dPMTArrayBottomZOffset =  dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight 
                                     - dCathodeFrameHeight - dReflectorBottomHeight - dBottomscreenFrameHeight 
                                     - dSensorThickness/2;
    m_pPMTArrayBottomPhysical = new G4PVPlacement(rotX180, G4ThreeVector(0.,0.,dPMTArrayBottomZOffset ), 
                                                  m_pPMTArrayBottomLogical, "PMTArrayBottom", m_pLXeLogical, 
                                                  false, 0);
 

  // Adding electrodes
  G4double dTopScreeningOffsetZ = (- dGXeHeight/2 + dElectrodesTopFrameHeight 
                                   - dTopScreeningFrameHeight + dTopScreeningThickness / 2);

  m_pTopScreeningPhysical = new G4PVPlacement(0,   G4ThreeVector(0.,0.,dTopScreeningOffsetZ),
                            m_pTopScreeningLogical,"TopScreeningDisk", m_pGXeLogical,
                            false, 0);
  
  G4double dAnodeOffsetZ = (- dGXeHeight/2 + dGateThickness  + dGateAnodeDistance + dAnodeThickness/2 ); 
  m_pAnodePhysical = new G4PVPlacement(0, G4ThreeVector(0.,0.,dAnodeOffsetZ),
                                       m_pAnodeLogical, "AnodeDisk", m_pGXeLogical, 
                                       false, 0);
  
  m_pGatePhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., -dLXeExtraFilling/2 + 0.5*dGateThickness), 
                                      m_pGateLogical , "GateDisk", m_pLXeExtraFillingInsideLogical, 
                                      false, 0);
  
  G4double dCathodeOffsetZ = (dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight - dCathodeThickness/2);
  m_pCathodePhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCathodeOffsetZ), 
                                         m_pCathodeLogical , "CathodeDisk", m_pLXeLogical, 
                                         false, 0);  
  
  G4double dBottomScreeningOffsetZ = (dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight 
                                                             - dCathodeFrameHeight - dReflectorBottomHeight
                                                             - dBottomScreeningThickness / 2); 
  m_pBottomScreeningPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dBottomScreeningOffsetZ),  
                                                 m_pBottomScreeningLogical, "BottomScreeningDisk", m_pLXeLogical, 
                                                 false, 0) ;

  new G4PVPlacement(0, G4ThreeVector(),m_pTPCLogical ,"CryoMat",InnerCryostatVolume_Logical, false, 0);
      

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//\

   G4double PTFEReflectorOutTPCRadius = dFSRInnerRadius + dFSRThickness + dPillarThickness;
   
   
   pPTFEReflectorOutTPC = new G4Tubs("PTFEReflector2", PTFEReflectorOutTPCRadius+1.975*mm,PTFEReflectorOutTPCRadius+ dReflectorThickness+1.975*mm, dActiveLXeHeight/2 , 0.*deg, 360.*deg);
   m_pPTFEReflectorOutTPCLogical = new G4LogicalVolume(pPTFEReflectorOutTPC, G4Material::GetMaterial("Teflon"), "PTFEReflector2", 0, 0, 0);
   m_pPTFEReflectorPhysical2 = new G4PVPlacement(0, G4ThreeVector(0.,0., dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight/2), m_pPTFEReflectorOutTPCLogical, "PTFEReflector2", m_pLXeLogical, false, 0);

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

   G4double InnerRadiusCrysotatTeflon = InnerCryo_InnerCylDiameter/2 - dReflectorThickness - 0.01*mm ;
   pPTFEReflectorCryostat = new G4Tubs("PTFEReflectorCryostat", InnerRadiusCrysotatTeflon, InnerRadiusCrysotatTeflon + dReflectorThickness, dActiveLXeHeight/2 +  dReflectorBottomHeight/2 + 
                                       dCathodeFrameHeight/2, 0.*deg, 360.*deg);
   m_pPTFEReflectorCryostatLogical = new G4LogicalVolume(pPTFEReflectorCryostat, G4Material::GetMaterial("Teflon"), "PTFEReflectorCryostat", 0, 0, 0);
   m_pPTFEReflectorCryostatPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight/2-+  dReflectorBottomHeight/2- dCathodeFrameHeight/2), 
                                                        m_pPTFEReflectorCryostatLogical, "PTFEReflectorCryostat", m_pLXeLogical, false, 0);
   
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

   pPTFEReflectorBottomOutTPC= new G4Tubs("pPTFEReflectorBottomOutTPC", PTFEReflectorOutTPCRadius+1.975*mm, PTFEReflectorOutTPCRadius + dReflectorThickness+1.975*mm, dReflectorBottomHeight/2 
                                          +dCathodeFrameHeight/2 , 0.*deg, 360.*deg);
   m_pPTFEReflectorBottomOutTPCLogical = new G4LogicalVolume(pPTFEReflectorBottomOutTPC, G4Material::GetMaterial("Teflon"), "pPTFEReflectorBottomOutTPC", 0, 0, 0);
   m_pPTFEReflectorBottomOutTPCPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight - dCathodeFrameHeight - dReflectorBottomHeight/2+ 
                                                            dCathodeFrameHeight/2), m_pPTFEReflectorBottomOutTPCLogical, "m_pPTFEReflectorBottomOutTPCPhysical", m_pLXeLogical, false, 0);

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

   pPTFEReflectorGXeLXe = new G4Tubs("pPTFEReflectorGXeLXe", PTFEReflectorOutTPCRadius +1.975*mm, dCryostatInnerRadius,  dReflectorThickness , 0.*deg, 
                                      360.*deg);
   m_pPTFEReflectorGXeLXeLogical = new G4LogicalVolume(pPTFEReflectorGXeLXe, G4Material::GetMaterial("Teflon"), "pPTFEReflectorGXeLXe", 0, 0, 0);
   m_pPTFEReflectorGXeLXePhysical = new G4PVPlacement(0, G4ThreeVector(0.,0.,- dGXeHeight/2  + dReflectorThickness ), m_pPTFEReflectorGXeLXeLogical, "pPTFEReflectorGXeLXe", m_pGXeLogical, false, 0);

                                                         
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

   pPTFEReflectorBottomLXe = new G4Tubs("pPTFEReflectorBottomLXe", PTFEReflectorOutTPCRadius +1.975*mm, dCryostatInnerRadius,  dReflectorThickness , 0.*deg, 
                                         360.*deg);
   m_pPTFEReflectorBottomLXeLogical = new G4LogicalVolume(pPTFEReflectorBottomLXe, G4Material::GetMaterial("Teflon"), "pPTFEReflectorBottomLXe", 0, 0, 0);
   m_pPTFEReflectorBottomLXePhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight - dCathodeFrameHeight - dReflectorBottomHeight - 
                                                         dReflectorThickness), m_pPTFEReflectorBottomLXeLogical, "pPTFEReflectorBottomLXe", m_pLXeLogical, false, 0);
 
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//


  PositiveTargetVolumeZ =  dCryostatInnerHeight/2 - dGXeHeight-1*mm;
  NegativeTargetVolumeZ =  dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight - dCathodeFrameHeight - dReflectorBottomHeight -  dReflectorThickness+4*mm;
  
  pSensitiveSiPMOutTPC = new G4Tubs("SensitiveSiPMOutTPC", PTFEReflectorOutTPCRadius +1.975*mm, dCryostatInnerRadius,  1*mm , 0.*deg, 360.*deg); 
  m_pSensitiveSiPMOutTPCLogical = new G4LogicalVolume(pSensitiveSiPMOutTPC, G4Material::GetMaterial("PhotoCathodeAluminium"), "SensitiveSiPMOutTPC", 0, 0, 0);
  
  //SensitiveDown = new G4PVPlacement(0, G4ThreeVector(0.,0.,NegativeTargetVolumeZ ), m_pSensitiveSiPMOutTPCLogical, "SensitiveSiPMOutTPC", m_pLXeLogical, false, 1);
  //SensitiveUp = new G4PVPlacement(0, G4ThreeVector(0.,0., PositiveTargetVolumeZ), m_pSensitiveSiPMOutTPCLogical, "SensitiveSiPMOutTPC",  m_pLXeLogical, false, 0);
  
  InnerRadiusTargetVolume =  PTFEReflectorOutTPCRadius +1.975*mm + dReflectorThickness;
  OuterRadiusTargetVolume =  dCryostatInnerRadius;

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//
   G4cout << "The thinkness of the volume is: " <<  dCryostatInnerRadius- (PTFEReflectorOutTPCRadius +1.975*mm) - dReflectorThickness << " [mm]" << G4endl;
   G4cout << "The inner radius of the target volume is: " <<   PTFEReflectorOutTPCRadius +1.975*mm + dReflectorThickness << " [mm]" << G4endl;
   G4cout << "The outer radius of the target volume is: " <<  dCryostatInnerRadius << " [mm]" << G4endl;
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//
  G4cout << (InnerRadiusCrysotatTeflon - PTFEReflectorOutTPCRadius+ dReflectorThickness+1.975*mm)/2 << G4endl;

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

   
  // We can create the whole fiber here, we choose to create one with a core and two cladding surfaces

  // CREATE CORE FIBER
  G4Tubs* fiber_tube = new G4Tubs("Fiber", fFiber_rmin, fFiber_rmax, fFiber_z, fFiber_sphi, fFiber_ephi);
  Fiber_Logical = new G4LogicalVolume(fiber_tube, G4Material::GetMaterial("Polystyrene"), "Fiber", 0, 0, 0);

  // CLADDING(first layer)
  G4Tubs* clad1_tube = new G4Tubs("Cladding1", fClad1_rmin, fClad1_rmax, fClad1_z, fClad1_sphi, fClad1_ephi);
  Clad1_Logical = new G4LogicalVolume(clad1_tube, G4Material::GetMaterial("PMMA"), "Cladding1", 0, 0, 0);

  // CLADDING(second layer)
  G4Tubs* clad2_tube = new G4Tubs("Cladding2", fClad2_rmin, fClad2_rmax, fClad2_z, fClad2_sphi, fClad2_ephi);
  Clad2_Logical = new G4LogicalVolume(clad2_tube, G4Material::GetMaterial("PVDF"), "Cladding2", 0, 0, 0);

  // PLACE CORE AND CLADDING 1 INTO CLADDING 2
  Fiber_Physical = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), Fiber_Logical, "Fiber", Clad1_Logical, false, 0);
  //Clad1_Physical = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), Clad1_Logical, "Cladding1", Clad2_Logical, false, 0);
  
  // PLACING FIBERS
  G4double fiber_Rpos = 1371.4850000000001 * mm ;



//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

G4bool placefibers = true;
G4double angleBetweenBalls = 360.0 / fNfibers;


if (placefibers) {

   G4cout << "Fibers are going to be placed." << G4endl;
  // Above we have created our sensitive detector or our SiPM. To place them, we can use two ways, using a for loop directly in the main code or by creating a plaing function
  
  
  // Place SiPM using a defined function of the class, We call it PlaceSiPMs  
  //
  // DEFINE THE ROTATION MATRIX
  G4RotationMatrix* rmu = new G4RotationMatrix();
  G4RotationMatrix* rmd = new G4RotationMatrix();
  rmd->rotateY(180. * deg);
  // DEFINE THE Z POSITION FOR THE SiPM, THE X AND Y POSITION ARE ALREADY CALCULATED INSIDE THE FUNCTION
  G4double zu = 1.3 * m +  height_SiPM + 0.01 * cm ;
  G4double zd = -zu;
  // CALL THE FUNCTION
  //G4int k = 0;
  //PlaceSiPMs(fSiPM_log, rmu,fNfibers, zu,k);
  //PlaceSiPMs(fSiPM_log, rmd,fNfibers, zd,k);

  G4double center_fiber = (dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight/2-+  dReflectorBottomHeight/2- dCathodeFrameHeight/2);
  
  // A different way is to place the SiPM directly using a for loop, wihtout defining a function
  for(G4int i = 0; i < fNfibers; ++i){
	   	
      		   
      G4double zu = fFiber_z + center_fiber  ;
      G4double zu2 = fFiber_z + center_fiber + height_SiPM/8 ;
      G4double zd = center_fiber- fFiber_z  ;
      G4double zd2 = center_fiber- fFiber_z-height_SiPM/8;
      G4double angle = i * angleBetweenBalls;

      // CONVERT THE ANGLE TO RADIANS
      G4double radians = angle * M_PI / 180.0;

      // CALCULATE THE X AND Y COORDINATES FOR THE FIBER
      G4double xf = fiber_Rpos * cos(radians);
      G4double yf = fiber_Rpos * sin(radians);
	    
      // CREATE A NEW G4PVPLACEMENT FOR THIS FIBER AT THE CALCULATED POSITION
      G4ThreeVector positionu(xf, yf, zu);
      G4ThreeVector positionu2(xf, yf, zu2);
      G4ThreeVector positiond(xf, yf, zd);
      G4ThreeVector positiond2(xf, yf, zd2);
      G4RotationMatrix* rotationu = new G4RotationMatrix();
      G4RotationMatrix* rotationd = new G4RotationMatrix();
      // WE NEED TO COMPLETLY ROTATE THE SiPM FOR PLACING IT IN THE OTHER SIDE OF THE FIBER
      rotationd->rotateY(180. * deg);
      
      // PLACE FIBERS IN THE UPPER AND LOWER EXTREMES OF THE FIBER
       new G4PVPlacement(rotationu, positionu, fSensitive_log, "SiPM0u", m_pLXeLogical, true, 2*i);
       new G4PVPlacement(rotationu, positiond, fSensitive_log, "SiPM0d", m_pLXeLogical, true, 2*i+1);
      //G4VPhysicalVolume *down = new G4PVPlacement(rotationd, positiond, fSensitive_log, "SiPM0d", LXe_log , true, 2*i+1);
	    
       new G4PVPlacement(rotationu, positionu2, fShield_log, "Shieldu", m_pLXeLogical, true, 2*i);
       new G4PVPlacement(rotationd, positiond2, fShield_log, "Shieldd", m_pLXeLogical, true, 2*i+1);
	    
	
      }
  
  
  // Another way is by placing the SiPM created in the class LXeWLSSD
  for(G4int i = 0; i < fNfibers; ++i){
	   	
      		   
      G4double zu = 1.3 * m ;
      G4double zd = -zu;
      G4double angle = i * angleBetweenBalls;

      // CONVERT THE ANGLE TO RADIANS
      G4double radians = angle * M_PI / 180.0;

      // CALCULATE THE X AND Y COORDINATES FOR THE FIBER
      G4double xf = fiber_Rpos * cos(radians);
      G4double yf = fiber_Rpos * sin(radians);
	    
      // CREATE A NEW G4PVPLACEMENT FOR THIS FIBER AT THE CALCULATED POSITION
      G4ThreeVector positionu(xf, yf, zu);
      G4ThreeVector positiond(xf, yf, zd);
      G4RotationMatrix* rotationu = new G4RotationMatrix();
      G4RotationMatrix* rotationd = new G4RotationMatrix();
      // WE NEED TO COMPLETLY ROTATE THE SiPM FOR PLACING IT IN THE OTHER SIDE OF THE FIBER
      rotationd->rotateY(180. * deg);
      
      // PLACE FIBERS IN THE UPPER AND LOWER EXTREMES OF THE FIBER
      //new LXeWLSSD(rotationu, positionu, LXe_log, true, 2*i, fConstructor);
      //new LXeWLSSD(rotationd, positiond, LXe_log, true, 2*i+1, fConstructor);
	    
      }
      

} else { G4cout << "Fibers are not going to be placed." << G4endl; }

 
  //------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//
  
  //Now for placing the fibers, we call the class LXeWLSFiber which return a fiber, to create many of them we make used of a for loop

  // FOR LOOP, GOES FROM 0 TO N, WHERE N IS THE NUMBER OF FIBERS WE WANT



if (placefibers) {
  for(G4int i = 0; i < fNfibers; ++i){
	   
      G4double angle = i * angleBetweenBalls;

      // CONVERT THE ANGLE TO RADIANS
      G4double radians = angle * M_PI / 180.0;

      // CALCULATE THE X AND Y COORDINATES FOR THIS FIBER
      G4double x = fiber_Rpos * cos(radians);
      G4double y = fiber_Rpos * sin(radians);
	    
      // CREATE A NEW G4PVPLACEMENT FOR THIS FIBER AT THE CALCULATED POSITION
      G4ThreeVector position(x, y, dCryostatInnerHeight/2 - dGXeHeight - dActiveLXeHeight/2-+  dReflectorBottomHeight/2- dCathodeFrameHeight/2);
      G4RotationMatrix* rotation = new G4RotationMatrix();

      //new LXeWLSFiber(rotation, position, LXe_log, false, i, fConstructor);
      
      
      new G4PVPlacement(rotation, position, Clad1_Logical, "Cladding1", m_pLXeLogical, false, i);
      }
}

      
      
      
      
      
      
      
  VisAttributes();
  BorderSurfaceProperties();
  SkinSurfaceProperties();
  PrintGeometryInformation();

  SetLogicalVolume(fMainWall_Logical);   
}


//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//


void LXeMainVolume::CopyValues()
{

  // We call the default values from the DetectorConstructor Class
  
  fMain_x          = fConstructor->GetScintX();
  fMain_y          = fConstructor->GetScintY();
  fMain_z          = fConstructor->GetScintZ();
  fSiPM_thick      = fConstructor->GetMainWallThickness();
  fMainWall_thick  = fConstructor->GetMainWallThickness();
  fRefl            = fConstructor->GetHousingReflectivity();
  fNfibers         = fConstructor->GetNFibers();
  
  fFiber_rmin = 0.0 * cm;
  fFiber_rmax = 0.1 * cm;
  fFiber_z    = 1330* mm;
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
  

  // Common Parameter used to build the Inner Cryostat
  InnerCryo_InnerCylDiameter = 2.8*m; 
  InnerCryo_InnerCylHeight = 3.2 *m; 
  InnerCryo_FlangeHeight = 150.0 *mm;
  InnerCryo_FlangeThickness = 60.0 *mm;
  InnerCryo_StiffenerHeight = 30.0 *mm;
  InnerCryo_StiffenerThickness = 60.0 *mm;
  InnerCryo_WallThickness = 6.0 *mm;
    
  InnerCryo_StiffenerNumber = 3.;
}



//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//


void LXeMainVolume::PlaceSiPMs(G4LogicalVolume* pmt_log, G4RotationMatrix* rot, G4int n, G4double& z, G4int k)
{
    for(G4int i = 0; i <= n; ++i)
    {
      
      G4double fiber_Rpos = 1.35 * m ;
      G4double angleBetweenBalls = 360.0 / n;
       
      G4double angle = i * angleBetweenBalls;

      // CONVERT THE ANGLE TO RADIANS
      G4double radians = angle * M_PI / 180.0;

      // CALCULATE THE X AND Y COORDINATES FOR THE FIBER
      G4double x = fiber_Rpos * cos(radians);
      G4double y = fiber_Rpos * sin(radians);
	   	        
      //new G4PVPlacement(rot, G4ThreeVector(x, y, z), fSiPM_log, "SiPM", LXe_log , false, k);
      //k++;
    
  }
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//


void LXeMainVolume::VisAttributes()
{
  G4VisAttributes* mainwall_va = new G4VisAttributes(G4Colour(0.8, 0.8, 0.8));
  fMainWall_Logical->SetVisAttributes(mainwall_va); 
}


//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeMainVolume::BorderSurfaceProperties()
{



    std::vector<G4double> pdTeflonPhotonMomentum = { 2.00 * eV, 2.87 * eV, 2.90 * eV, 3.47 * eV, 7.0 * eV, 7.07 * eV, 7.14 * eV  };
    std::vector<G4double> pdTeflonRefractiveIndex = {1.3, 1.306 , 1.306 , 1.311  , 1.63,    1.61,    1.58};
    std::vector<G4double> pdTeflonReflectivity = {1,    1,    1, 1, 1, 1, 1};
    std::vector<G4double> pdTeflonSpecularLobe =  {0.01,    0.01,    0.01, 0.01, 0.01, 0.01 ,0.01};
    std::vector<G4double> pdTeflonSpecularSpike =  {0.01,    0.01,    0.01, 0.01, 0.01, 0.01, 0.01};
    std::vector<G4double> pdTeflonBackscatter =  {0.01,    0.01,    0.01, 0.01, 0.01, 0.01, 0.01};
    std::vector<G4double> pdTeflonEfficiency =  {0,     0,     0, 0, 0, 0, 0	};
    std::vector<G4double> pdTeflonTransmittance = {1e-12, 1e-12, 1e-12, 1e-12, 1e-12, 1e-12, 1e-12};
	
    G4MaterialPropertiesTable *pTeflonPropertiesTable = new G4MaterialPropertiesTable();
      
    pTeflonPropertiesTable->AddProperty("RINDEX", pdTeflonPhotonMomentum, pdTeflonRefractiveIndex);
    pTeflonPropertiesTable->AddProperty("REFLECTIVITY", pdTeflonPhotonMomentum, pdTeflonReflectivity);
    pTeflonPropertiesTable->AddProperty("SPECULARLOBECONSTANT", pdTeflonPhotonMomentum, pdTeflonSpecularLobe);
    pTeflonPropertiesTable->AddProperty("SPECULARSPIKECONSTANT", pdTeflonPhotonMomentum, pdTeflonSpecularSpike);
    pTeflonPropertiesTable->AddProperty("BACKSCATTERCONSTANT", pdTeflonPhotonMomentum, pdTeflonBackscatter);
    pTeflonPropertiesTable->AddProperty("EFFICIENCY", pdTeflonPhotonMomentum, pdTeflonEfficiency);
    pTeflonPropertiesTable->AddProperty("TRANSMITTANCE", pdTeflonPhotonMomentum, pdTeflonTransmittance	); 
 
  //----InnerCryostat_LXe----//
  InnerCryostat_LXe = new G4OpticalSurface("InnerCryostat_LXe");
  new G4LogicalBorderSurface("InnerCryostat_LXe", InnerCryostatVolume_Physical, m_pLXePhysical, InnerCryostat_LXe);
  InnerCryostat_LXe->SetType(dielectric_dielectric);
  InnerCryostat_LXe->SetFinish(ground);
  InnerCryostat_LXe->SetModel(unified);
  
  InnerCryostat_LXeProperties = G4Material::GetMaterial("SS304LSteel")->GetMaterialPropertiesTable();
  InnerCryostat_LXe->SetMaterialPropertiesTable(InnerCryostat_LXeProperties);
  
  
   //----InnerCryostatBottomFillerVessel_LXe----//
  InnerCryostatBottomFillerVessel_LXe = new G4OpticalSurface("InnerCryostatBottomFillerVessel_LXe");
  new G4LogicalBorderSurface("InnerCryostatBottomFillerVessel_LXe", InnerCryostatBottomFillerVessel_Physical , m_pLXePhysical, InnerCryostatBottomFillerVessel_LXe);
  InnerCryostatBottomFillerVessel_LXe->SetType(dielectric_dielectric);
  InnerCryostatBottomFillerVessel_LXe->SetFinish(ground);
  InnerCryostatBottomFillerVessel_LXe->SetModel(unified);
  
  InnerCryostatBottomFillerVessel_LXeProperties = G4Material::GetMaterial("SS304LSteel")->GetMaterialPropertiesTable();
  InnerCryostatBottomFillerVessel_LXe->SetMaterialPropertiesTable(InnerCryostatBottomFillerVessel_LXeProperties);
  
  
  //---InnerCryostatVessel_LXe---//
  InnerCryostatVessel_LXe = new G4OpticalSurface("InnerCryostatVessel_LXe");
  new G4LogicalBorderSurface("InnerCryostatVessel_LXe", InnerCryostatVessel_Physical , m_pLXePhysical, InnerCryostatVessel_LXe);
  InnerCryostatVessel_LXe->SetType(dielectric_dielectric);
  InnerCryostatVessel_LXe->SetFinish(ground);
  InnerCryostatVessel_LXe->SetModel(unified);
  
  InnerCryostatVessel_LXeProperties = G4Material::GetMaterial("SS304LSteel")->GetMaterialPropertiesTable();
  InnerCryostatVessel_LXe->SetMaterialPropertiesTable(InnerCryostatVessel_LXeProperties);
  
    //----InnerCryostat_GXe----//
  InnerCryostat_GXe = new G4OpticalSurface("InnerCryostat_GXe");
  new G4LogicalBorderSurface("InnerCryostat_GXe", InnerCryostatVolume_Physical, m_pGXePhysical, InnerCryostat_GXe);
  InnerCryostat_GXe->SetType(dielectric_dielectric);
  InnerCryostat_GXe->SetFinish(ground);
  InnerCryostat_GXe->SetModel(unified);
  
  InnerCryostat_GXeProperties = G4Material::GetMaterial("SS304LSteel")->GetMaterialPropertiesTable();
  InnerCryostat_GXe->SetMaterialPropertiesTable(InnerCryostat_GXeProperties);
  
  
   //----InnerCryostatBottomFillerVessel_GXe----//
  InnerCryostatBottomFillerVessel_GXe = new G4OpticalSurface("InnerCryostatBottomFillerVessel_GXe");
  new G4LogicalBorderSurface("InnerCryostatBottomFillerVessel_GXe", InnerCryostatBottomFillerVessel_Physical , m_pGXePhysical, InnerCryostatBottomFillerVessel_GXe);
  InnerCryostatBottomFillerVessel_GXe->SetType(dielectric_dielectric);
  InnerCryostatBottomFillerVessel_GXe->SetFinish(ground);
  InnerCryostatBottomFillerVessel_GXe->SetModel(unified);
  
  InnerCryostatBottomFillerVessel_GXeProperties = G4Material::GetMaterial("SS304LSteel")->GetMaterialPropertiesTable();
  InnerCryostatBottomFillerVessel_GXe->SetMaterialPropertiesTable(InnerCryostatBottomFillerVessel_GXeProperties);
  
  
  //---InnerCryostatVessel_GXe---//
  InnerCryostatVessel_GXe = new G4OpticalSurface("InnerCryostatVessel_GXe");
  new G4LogicalBorderSurface("InnerCryostatVessel_GXe", InnerCryostatVessel_Physical , m_pGXePhysical, InnerCryostatVessel_GXe);
  InnerCryostatVessel_GXe->SetType(dielectric_dielectric);
  InnerCryostatVessel_GXe->SetFinish(ground);
  InnerCryostatVessel_GXe->SetModel(unified);
  
  InnerCryostatVessel_GXeProperties = G4Material::GetMaterial("SS304LSteel")->GetMaterialPropertiesTable();
  InnerCryostatVessel_GXe->SetMaterialPropertiesTable(InnerCryostatVessel_GXeProperties);

  
  //----PTFE_reflectorSurface----//
  PTFE_reflectorSurface = new G4OpticalSurface("PTFE_reflectorSurface");
  new G4LogicalBorderSurface("PTFE_reflectorSurface", m_pLXeSensitivePhysical, m_pPTFEReflectorPhysical, PTFE_reflectorSurface);
  PTFE_reflectorSurface->SetType(dielectric_dielectric);
  PTFE_reflectorSurface->SetFinish(ground);
  PTFE_reflectorSurface->SetModel(unified);
  
  PTFE_reflectorSurfaceProperties = G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable();
  PTFE_reflectorSurface->SetMaterialPropertiesTable(PTFE_reflectorSurfaceProperties);
  
  
  //----PTFE_reflector_TopSurface----//
  PTFE_reflector_TopSurface = new G4OpticalSurface("PTFE_reflector_TopSurface");
  new G4LogicalBorderSurface("PTFE_reflector_TopSurface", m_pGXePhysical, m_pPTFEReflectorTopPhysical, PTFE_reflector_TopSurface);
  PTFE_reflector_TopSurface->SetType(dielectric_dielectric);
  PTFE_reflector_TopSurface->SetFinish(ground);
  PTFE_reflector_TopSurface->SetModel(unified);
  
  PTFE_reflector_TopSurfaceProperties = G4Material::GetMaterial("GXeTeflon")->GetMaterialPropertiesTable();
  PTFE_reflector_TopSurface->SetMaterialPropertiesTable(PTFE_reflector_TopSurfaceProperties);
  
  
  //----PTFE_reflector_TopExtraFillingSurface---//
  PTFE_reflector_TopExtraFillingSurface = new G4OpticalSurface("PTFE_reflector_TopExtraFillingSurface");
  new G4LogicalBorderSurface("PTFE_reflector_TopExtraFillingSurface", m_pLXeExtraFillingInsidePhysical, m_pPTFEReflectorTopPhysical, PTFE_reflector_TopExtraFillingSurface);
  PTFE_reflector_TopExtraFillingSurface->SetType(dielectric_dielectric);
  PTFE_reflector_TopExtraFillingSurface->SetFinish(ground);
  PTFE_reflector_TopExtraFillingSurface->SetModel(unified);
  
  PTFE_reflector_TopExtraFillingSurfaceProperties = G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable();
  PTFE_reflector_TopExtraFillingSurface->SetMaterialPropertiesTable(PTFE_reflector_TopExtraFillingSurfaceProperties);
  
  
  //----PTFE_TopAnodeSurface---//
  PTFE_TopAnodeSurface = new G4OpticalSurface("PTFE_TopAnodeSurface");
  new G4LogicalBorderSurface("PTFE_TopAnodeSurface", m_pAnodePhysical, m_pPTFEReflectorTopPhysical, PTFE_TopAnodeSurface);
  PTFE_TopAnodeSurface->SetType(dielectric_dielectric);
  PTFE_TopAnodeSurface->SetFinish(ground);
  PTFE_TopAnodeSurface->SetModel(unified);
  
  PTFE_TopAnodeSurfaceProperties = G4Material::GetMaterial("GXeTeflon")->GetMaterialPropertiesTable();
  PTFE_TopAnodeSurface->SetMaterialPropertiesTable(PTFE_TopAnodeSurfaceProperties);


  //----PTFE_TopTopScreeningSurface----//
  PTFE_TopTopScreeningSurface = new G4OpticalSurface("PTFE_TopTopScreeningSurface");
  new G4LogicalBorderSurface("PTFE_TopTopScreeningSurface", m_pTopScreeningPhysical, m_pPTFEReflectorTopPhysical, PTFE_TopTopScreeningSurface);
  PTFE_TopTopScreeningSurface->SetType(dielectric_dielectric);
  PTFE_TopTopScreeningSurface->SetFinish(ground);
  PTFE_TopTopScreeningSurface->SetModel(unified);
  
  PTFE_TopTopScreeningSurfaceProperties = G4Material::GetMaterial("GXeTeflon")->GetMaterialPropertiesTable();
  PTFE_TopTopScreeningSurface->SetMaterialPropertiesTable(PTFE_TopTopScreeningSurfaceProperties);
  

  //----PTFE_reflector_BottomTPCSurface----//
  PTFE_reflector_BottomTPCSurface = new G4OpticalSurface("PTFE_reflector_BottomTPCSurface");
  new G4LogicalBorderSurface("PTFE_reflector_BottomTPCSurface", m_pLXePhysical, m_pPTFEReflectorBottomPhysical, PTFE_reflector_BottomTPCSurface);
  PTFE_reflector_BottomTPCSurface->SetType(dielectric_dielectric);
  PTFE_reflector_BottomTPCSurface->SetFinish(ground);
  PTFE_reflector_BottomTPCSurface->SetModel(unified);
  
  PTFE_reflector_BottomTPCSurfaceProperties = G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable();
  PTFE_reflector_BottomTPCSurface->SetMaterialPropertiesTable(PTFE_reflector_BottomTPCSurfaceProperties);
  
  
   //----CuFSRTPCSurface----//
  CuFSRTPCSurface = new G4OpticalSurface("CuFSRTPCSurface");
  new G4LogicalBorderSurface("CuFSRTPCSurface", m_pLXePhysical, m_pCuFSRPhysical, CuFSRTPCSurface);
  CuFSRTPCSurface->SetType(dielectric_dielectric);
  CuFSRTPCSurface->SetFinish(ground);
  CuFSRTPCSurface->SetModel(unified);
  
  CuFSRTPCSurfaceProperties = G4Material::GetMaterial("Copper")->GetMaterialPropertiesTable();
  CuFSRTPCSurface->SetMaterialPropertiesTable(CuFSRTPCSurfaceProperties);
  
  
   //----CuArrayDiskTopTPCSurface----//
  CuArrayDiskTopTPCSurface = new G4OpticalSurface("CuArrayDiskTopTPCSurface");
  new G4LogicalBorderSurface("CuFSRTPCSurface", m_pLXePhysical, m_pCuArrayDiskTopPhysical, CuArrayDiskTopTPCSurface);
  CuArrayDiskTopTPCSurface->SetType(dielectric_dielectric);
  CuArrayDiskTopTPCSurface->SetFinish(ground);
  CuArrayDiskTopTPCSurface->SetModel(unified);
  
  CuArrayDiskTopTPCSurfaceProperties = G4Material::GetMaterial("Copper")->GetMaterialPropertiesTable();
  CuArrayDiskTopTPCSurface->SetMaterialPropertiesTable(CuArrayDiskTopTPCSurfaceProperties);
  
  
  //----PTFEReflectorTPCSurface----//
  PTFEReflectorTPCSurface = new G4OpticalSurface("PTFEReflectorTPCSurface");
  new G4LogicalBorderSurface("PTFEReflectorTPCSurface", m_pLXePhysical, m_pPTFEReflectorPhysical, PTFEReflectorTPCSurface);
  PTFEReflectorTPCSurface->SetType(dielectric_dielectric);
  PTFEReflectorTPCSurface->SetFinish(ground);
  PTFEReflectorTPCSurface->SetModel(unified);
  
  PTFEReflectorTPCSurfaceProperties = G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable();
  PTFEReflectorTPCSurface->SetMaterialPropertiesTable(PTFEReflectorTPCSurfaceProperties);
  
  
  //----PTFEReflectorOutTPCSurface----//
  PTFEReflectorOutTPCSurface = new G4OpticalSurface("PTFEReflectorOutTPCSurface");
  new G4LogicalBorderSurface("PTFEReflectorOutTPCSurface", m_pLXePhysical, m_pPTFEReflectorPhysical2 , PTFEReflectorOutTPCSurface);
  PTFEReflectorOutTPCSurface->SetType(dielectric_dielectric);
  PTFEReflectorOutTPCSurface->SetFinish(ground);
  PTFEReflectorOutTPCSurface->SetModel(unified);
  
  PTFEReflectorOutTPCSurfaceProperties = G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable();
  PTFEReflectorOutTPCSurface->SetMaterialPropertiesTable(pTeflonPropertiesTable);
  
  
  //----PTFEReflectorCryostaturface----//
  PTFEReflectorCryostaturface = new G4OpticalSurface("PTFEReflectorCryostaturface");
  new G4LogicalBorderSurface("PTFEReflectorCryostaturface", m_pLXePhysical, m_pPTFEReflectorCryostatPhysical , PTFEReflectorCryostaturface);
  PTFEReflectorCryostaturface->SetType(dielectric_dielectric);
  PTFEReflectorCryostaturface->SetFinish(ground);
  PTFEReflectorCryostaturface->SetModel(unified);
  
  PTFEReflectorCryostaturfaceProperties = G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable();
  PTFEReflectorCryostaturface->SetMaterialPropertiesTable(PTFEReflectorCryostaturfaceProperties);
  
 
  
  
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----// 


void LXeMainVolume::SkinSurfaceProperties()
{

    std::vector<G4double> pdTeflonPhotonMomentum = { 2.00 * eV, 2.87 * eV, 2.90 * eV, 3.47 * eV, 7.0 * eV, 7.07 * eV, 7.14 * eV  };
    std::vector<G4double> pdTeflonRefractiveIndex = {1.3, 1.306 , 1.306 , 1.311  , 1.63,    1.61,    1.58};
    std::vector<G4double> pdTeflonReflectivity = {1,    1,    1, 1, 1, 1, 1};
    std::vector<G4double> pdTeflonSpecularLobe =  {0.01,    0.01,    0.01, 0.01, 0.01, 0.01 ,0.01};
    std::vector<G4double> pdTeflonSpecularSpike =  {0.01,    0.01,    0.01, 0.01, 0.01, 0.01, 0.01};
    std::vector<G4double> pdTeflonBackscatter =  {0.01,    0.01,    0.01, 0.01, 0.01, 0.01, 0.01};
    std::vector<G4double> pdTeflonEfficiency =  {0,     0,     0, 0, 0, 0, 0	};
    std::vector<G4double> pdTeflonTransmittance = {1e-12, 1e-12, 1e-12, 1e-12, 1e-12, 1e-12, 1e-12};
	
    G4MaterialPropertiesTable *pTeflonPropertiesTable = new G4MaterialPropertiesTable();
      
    pTeflonPropertiesTable->AddProperty("RINDEX", pdTeflonPhotonMomentum, pdTeflonRefractiveIndex);
    pTeflonPropertiesTable->AddProperty("REFLECTIVITY", pdTeflonPhotonMomentum, pdTeflonReflectivity);
    pTeflonPropertiesTable->AddProperty("SPECULARLOBECONSTANT", pdTeflonPhotonMomentum, pdTeflonSpecularLobe);
    pTeflonPropertiesTable->AddProperty("SPECULARSPIKECONSTANT", pdTeflonPhotonMomentum, pdTeflonSpecularSpike);
    pTeflonPropertiesTable->AddProperty("BACKSCATTERCONSTANT", pdTeflonPhotonMomentum, pdTeflonBackscatter);
    pTeflonPropertiesTable->AddProperty("EFFICIENCY", pdTeflonPhotonMomentum, pdTeflonEfficiency);
    pTeflonPropertiesTable->AddProperty("TRANSMITTANCE", pdTeflonPhotonMomentum, pdTeflonTransmittance	); 

  //----InnerCrysotatSkin---//
  InnerCrysotatSkin = new G4OpticalSurface("InnerCrysotatSkin");
  InnerCrysotatSkin->SetType(dielectric_dielectric);
  InnerCrysotatSkin->SetFinish(ground);
  InnerCrysotatSkin->SetModel(unified);
  
  InnerCrysotatSkinProperties = G4Material::GetMaterial("SS304LSteel")->GetMaterialPropertiesTable();
  InnerCrysotatSkin->SetMaterialPropertiesTable(InnerCrysotatSkinProperties);
  new G4LogicalSkinSurface("InnerCrysotatSkin", InnerCryostatVessel_Logical, InnerCrysotatSkin);
  
  
  //----CuArrayDiskSkin---//
  CuArrayDiskSkin = new G4OpticalSurface("CuArrayDiskSkin");
  CuArrayDiskSkin->SetType(dielectric_dielectric);
  CuArrayDiskSkin->SetFinish(ground);
  CuArrayDiskSkin->SetModel(unified);
  
  CuArrayDiskSkinProperties = G4Material::GetMaterial("Copper")->GetMaterialPropertiesTable();
  CuArrayDiskSkin->SetMaterialPropertiesTable(CuArrayDiskSkinProperties);
  new G4LogicalSkinSurface("CuArrayDiskSkin", m_pCuArrayDiskLogical, CuArrayDiskSkin);
  
  //----PTFEArrayDiskSkin---//
  PTFEArrayDiskSkin = new G4OpticalSurface("PTFEArrayDiskSkin");
  PTFEArrayDiskSkin->SetType(dielectric_dielectric);
  PTFEArrayDiskSkin->SetFinish(ground);
  PTFEArrayDiskSkin->SetModel(unified);
  
  PTFEArrayDiskSkinProperties = G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable();
  PTFEArrayDiskSkin->SetMaterialPropertiesTable(PTFEArrayDiskSkinProperties);
  new G4LogicalSkinSurface("PTFEArrayDiskSkin", m_pPTFEArrayDiskLogical, PTFEArrayDiskSkin);

  
  //----CuFSRSkin---//
  CuFSRSkin = new G4OpticalSurface("CuFSRSkin");
  CuFSRSkin->SetType(dielectric_dielectric);
  CuFSRSkin->SetFinish(ground);
  CuFSRSkin->SetModel(unified);
  
  CuFSRSkinProperties = G4Material::GetMaterial("Copper")->GetMaterialPropertiesTable();
  CuFSRSkin->SetMaterialPropertiesTable(CuFSRSkinProperties);
  new G4LogicalSkinSurface("CuFSRSkin	", m_pCuFSRLogical, CuFSRSkin);
  
  
  //----PTFEPillarSkin---//
  PTFEPillarSkin = new G4OpticalSurface("PTFEPillarSkin");
  PTFEPillarSkin->SetType(dielectric_dielectric);
  PTFEPillarSkin->SetFinish(ground);
  PTFEPillarSkin->SetModel(unified);
  
  PTFEPillarSkinProperties = G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable();
  PTFEPillarSkin->SetMaterialPropertiesTable(PTFEPillarSkinProperties);
  new G4LogicalSkinSurface("PTFEPillarSkin", m_pPTFEPillarLogical, PTFEPillarSkin);


  //----PTFEReflectorSkin---//
  PTFEReflectorSkin = new G4OpticalSurface("PTFEReflectorSkin");
  PTFEReflectorSkin->SetType(dielectric_dielectric);
  PTFEReflectorSkin->SetFinish(ground);
  PTFEReflectorSkin->SetModel(unified);
  
  PTFEReflectorSkinProperties = G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable();
  PTFEReflectorSkin->SetMaterialPropertiesTable(pTeflonPropertiesTable);
  new G4LogicalSkinSurface("PTFEReflectorSkin", m_pPTFEReflectorOutTPCLogical, PTFEReflectorSkin);
  
  
  //----  PTFEReflectorBottomOutTPCSkin---//
  PTFEReflectorBottomOutTPCSkin = new G4OpticalSurface("PTFEReflectorBottomOutTPCSkin");
  PTFEReflectorBottomOutTPCSkin->SetType(dielectric_dielectric);
  PTFEReflectorBottomOutTPCSkin->SetFinish(ground);
  PTFEReflectorBottomOutTPCSkin->SetModel(unified);
  
  PTFEReflectorBottomOutTPCSkinProperties = G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable();
  PTFEReflectorBottomOutTPCSkin->SetMaterialPropertiesTable(PTFEReflectorBottomOutTPCSkinProperties);
  new G4LogicalSkinSurface("PTFEReflectorBottomOutTPCSkin", m_pPTFEReflectorOutTPCLogical, PTFEReflectorBottomOutTPCSkin);
  
  
  //----PTFEReflectorGXeLXeSkin---//
  PTFEReflectorGXeLXeSkin = new G4OpticalSurface("PTFEReflectorGXeLXeSkin");
  PTFEReflectorGXeLXeSkin->SetType(dielectric_dielectric);
  PTFEReflectorGXeLXeSkin->SetFinish(ground);
  PTFEReflectorGXeLXeSkin->SetModel(unified);
  
  PTFEReflectorGXeLXeSkinProperties = G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable();
  PTFEReflectorGXeLXeSkin->SetMaterialPropertiesTable(PTFEReflectorGXeLXeSkinProperties);
  new G4LogicalSkinSurface("PTFEReflectorGXeLXeSkin", m_pPTFEReflectorGXeLXeLogical , PTFEReflectorGXeLXeSkin);
  
  
  //----PTFEReflectorBottomLXeSkin---//
  PTFEReflectorBottomLXe = new G4OpticalSurface("PTFEReflectorBottomLXe");
  PTFEReflectorBottomLXe->SetType(dielectric_dielectric);
  PTFEReflectorBottomLXe->SetFinish(ground);
  PTFEReflectorBottomLXe->SetModel(unified);
  
  PTFEReflectorBottomLXeProperties = G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable();
  PTFEReflectorBottomLXe->SetMaterialPropertiesTable(PTFEReflectorBottomLXeProperties);
  new G4LogicalSkinSurface("PTFEReflectorBottomLXe", m_pPTFEReflectorBottomLXeLogical , PTFEReflectorBottomLXe);
  
  

  
  

  




}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----// 


void LXeMainVolume::PrintGeometryInformation()
{

  G4cout << "============================================================================================= " << G4endl;
  const G4double InnerCryostatMass = InnerCryostatVessel_Logical->GetMass(false, false)/kg;
  const G4double InnerCryostatVolume = InnerCryostatMass/(G4Material::GetMaterial("SS304LSteel")->GetDensity()*m3/kg);  
  G4cout << "InnerCryostatVessel:                     " << InnerCryostatMass << " kg " << "     =============    " << InnerCryostatVolume << " m3 " << G4endl;

  const G4double InnerCryostatBottomFillerMass = InnerCryostatBottomFillerVessel_Logical->GetMass(false, false)/kg;
  const G4double InnerCryostatBottomFillerVolume = InnerCryostatBottomFillerMass/(G4Material::GetMaterial("SS304LSteel")->GetDensity()*m3/kg);  
  G4cout << "InnerCryostatBottomFillerVessel:         " << InnerCryostatBottomFillerMass << " kg " << "     =============    " << InnerCryostatBottomFillerVolume << " m3 " << G4endl;
  G4cout << "============================================================================================= " << G4endl;

  //================================== XENON MASSES  =========================================================
  const G4double LXeMass = InnerCryostatBottomVolume_Logical->GetMass(false, false)/kg;
  const G4double LXeVolume = LXeMass/(G4Material::GetMaterial("LXe")->GetDensity()*m3/kg);  
  G4cout << "Liquid Xenon around BottomFiller:        " << LXeMass << " kg " << "     =============    " << LXeVolume << " m3 " << G4endl;

  const G4double GXeMass = InnerCryostatVolume_Logical->GetMass(false, false)/kg;
  const G4double GXeVolume = GXeMass/(G4Material::GetMaterial("LXe")->GetDensity()*m3/kg);  
  G4cout << "Gaseous Xenon in TopDome:                " << GXeMass << " kg " << "     =============    " << GXeVolume << " m3 " << G4endl;
  G4cout << "============================================================================================= " << G4endl;
  
  //=================================XENON MASSES AROUND TPC---------------------------------------------------
  const G4double LXeMassAroundTPC =  m_pLXeLogical->GetMass(false, false)/kg;
  const G4double LXeVolumeAroundTPC = LXeMassAroundTPC/(G4Material::GetMaterial("LXe")->GetDensity()*m3/kg);  
  G4cout << "Liquid Xenon around TPC:        " << LXeMassAroundTPC << " kg " << "     =============    " << LXeVolumeAroundTPC << " m3 " << G4endl;

  const G4double GXeMassAroundTPC =  m_pGXeLogical->GetMass(false, false)/kg;
  const G4double GXeVolumeAroundTPC = GXeMassAroundTPC/(G4Material::GetMaterial("LXe")->GetDensity()*m3/kg);  
  G4cout << "Gaseous Xenon in TopDome:                " << GXeMassAroundTPC << " kg " << "     =============    " << GXeVolumeAroundTPC << " m3 " << G4endl;
  G4cout << "============================================================================================= " << G4endl;


}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----// 


G4UnionSolid * LXeMainVolume::BuildVesselShape( G4double Diameter_cyl, G4double Height_cyl, G4double Thickness_Wall )
{

    // ConstructVessel: construction of a cylindrical shape with two DIN 28011 (no Straight Flange componentent; SF=0) torispheric domes 
    //                  
    // Input: Diameter_cyl = cylinder inner diameter ###   Height_cyl = cylinder length of the vessel ###   Thickness_Wall = Wall thickness of the cylinder & minimal thickness of the dome walls
    
    // Derive parameter
    G4double R_cyl = Diameter_cyl/2;	// inner radius of the cylinder
    G4double R0 = Diameter_cyl; 	// sphere inner radius (DIN 28011)
    G4double R1 = 0.1 * R0;		// toroid radius(DIN 28011)
    G4double rc0 = R_cyl-R1;
    G4double dR0 = R0-R1;
    G4double dTheta = asin(rc0/dR0);
    G4double dZ     = sqrt(dR0*dR0 - rc0*rc0);

    // Build Cylinder Body, Bottom Sphere & Torus, Top Sphere & Torus
    G4Tubs   *pCyl     = new G4Tubs("CylinderBody", 0. *cm, Diameter_cyl/2 + Thickness_Wall, Height_cyl/2 , 0.*deg, 360.*deg);

    G4Sphere *pBottom1 = new G4Sphere("Bottom1", 0.1 *mm, R0 + Thickness_Wall , 0., 2*M_PI, M_PI-dTheta, dTheta );
    G4Torus  *pBottom2 = new G4Torus("Bottom2",0.1 *mm, R1 + Thickness_Wall, rc0, 0., 2*M_PI);
         
    G4Sphere *pTop1    = new G4Sphere("Top1", 0.1 *mm, R0 + Thickness_Wall, 0., 2*M_PI, 0., dTheta );
    G4Torus  *pTop2    = new G4Torus("Top2",0.1 *mm, R1 + Thickness_Wall, rc0, 0., 2*M_PI);
    
    // Make one solid by combining the vessel components
    G4UnionSolid *pVessel0;
    G4UnionSolid *pVessel1;
    G4UnionSolid *pVessel2;
    G4UnionSolid *pVessel3;
    pVessel0 = new G4UnionSolid("UnionSolid", pCyl, pTop1, 0, G4ThreeVector(0.,0.,Height_cyl/2 - dZ));
    pVessel1 = new G4UnionSolid("UnionSolid", pVessel0, pBottom1, 0, G4ThreeVector(0.,0.,-Height_cyl/2  + dZ));
    pVessel2 = new G4UnionSolid("UnionSolid", pVessel1, pTop2, 0, G4ThreeVector(0.,0.,Height_cyl/2 ));
    pVessel3 = new G4UnionSolid("UnionSolid", pVessel2, pBottom2, 0, G4ThreeVector(0.,0.,-Height_cyl/2 ));

    return pVessel3;
}


//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----// 


G4SubtractionSolid * LXeMainVolume::BuildFillerShape( G4double Diameter_cyl, G4double Height_cyl, G4double Thickness_Wall )
{

    // ConstructFillerShape: construction of the filler volume, being placed in the bottom torispheric dome
    // Input: Diameter_cyl = cylinder inner diameter ### Height_cyl = cylinder length of the Cryosstat vessel ###  Thickness_Wall = minimal thickness of the vessel
    
    // Derive parameter
    G4double R_cyl = Diameter_cyl/2;	
    G4double R0 = Diameter_cyl; 	
    G4double R1 = 0.1 * R0;		
    G4double rc0 = R_cyl-R1;
    G4double dR0 = R0-R1;
    G4double dTheta = asin(rc0/dR0);
    G4double dZ     = sqrt(dR0*dR0 - rc0*rc0);

    // Build Cylinder Body, Bottom Sphere & Torus, Top Sphere & Torus
    G4Tubs   *pCyl     = new G4Tubs("CylinderBody", 0. *cm, Diameter_cyl/2 + Thickness_Wall, Height_cyl/2 - Thickness_Wall, 0.*deg, 360.*deg);
    G4Sphere *pBottom1 = new G4Sphere("Bottom1", 0.1 *mm, R0 + Thickness_Wall , 0., 2*M_PI, M_PI-dTheta, dTheta );
    G4Torus  *pBottom2 = new G4Torus("Bottom2",0.1 *mm, R1 + Thickness_Wall, rc0, 0., 2*M_PI);

    // Make one solid by combining the vessel components
    G4UnionSolid *pFiller0;
    G4UnionSolid *pFiller1;
    G4SubtractionSolid *pFiller2;
    pFiller0 = new G4UnionSolid("UnionSolid", pCyl, pBottom1, 0, G4ThreeVector(0.,0.,-Height_cyl/2  + dZ));
    pFiller1 = new G4UnionSolid("UnionSolid", pFiller0, pBottom2, 0, G4ThreeVector(0.,0.,-Height_cyl/2 ));
    pFiller2 = new G4SubtractionSolid("SubtractionSolid", pFiller1, pCyl, 0, G4ThreeVector(0.,0.,0.));

    return pFiller2;
}
