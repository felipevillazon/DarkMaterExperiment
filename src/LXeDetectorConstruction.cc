#include "LXeDetectorConstruction.hh"
#include "LXeDetectorMessenger.hh"
#include "LXeMainVolume.hh"
#include "LXePMTSD.hh"
#include "LXeScintSD.hh"
#include "LXeWLSSD.hh"
#include "LXeWLSFiber.hh"
#include "globals.hh"
#include "G4Box.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4OpticalSurface.hh"
#include "G4PhysicalConstants.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UImanager.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//


LXeDetectorConstruction::LXeDetectorConstruction()

{
  fWorld_box  = nullptr;
  fWorld_log  = nullptr;
  fWorld_phys = nullptr;

  
  // CALL DEFAULT VALUES
  SetDefaults();
 
  // CALL DEFINED MATERIALS & ELEMENTS
  DefineMaterials();
  
  // MESSENGER FOR MODIFYING GEOMETRY
  fDetectorMessenger = new LXeDetectorMessenger(this);
  
  // CALL DEFINED MATERIALS PROPERTIES
  DefineMaterialsProperties();
  
  // CALL DEFINED OPTICAL PROPERTIES OF MATERIALS
  DefinedOpticalPropertiesMaterials();
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeDetectorConstruction::~LXeDetectorConstruction()
{

  delete fMainVolume;  
  delete fDetectorMessenger;

}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//


void LXeDetectorConstruction::DefineMaterials()
{
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4NistManager *pNistManager = G4NistManager::Instance();    
  //***Elements
  fH = new G4Element("Hydrogen", "H", z = 1., a = 1.01 * g / mole);
  fC = new G4Element("Carbon", "C", z = 6., a = 12.01 * g / mole);
  fN = new G4Element("Nitrogen", "N", z = 7., a = 14.01 * g / mole);
  fO = new G4Element("Oxygen", "O", z = 8., a = 16.00 * g / mole);
  fF = new G4Element("Fluorine", "F", z = 9., a = 18.998 *g / mole);
  fCr = new G4Element("Chromium", "Cr", 24., 51.996 * g / mole);
  fMn = new G4Element("Manganese", "Mn", 25., 54.938 * g / mole);
  fFe = new G4Element("Iron", "Fe", 26., 55.85 * g / mole);
  fNi = new G4Element("Nickel", "Ni", 28., 58.693 * g / mole);
  fCu = new G4Element("Copper", "Cu", 29., 63.546 * g / mole);
  fSi = new G4Element("Silicon", "Si", 14., 28.086 * g / mole);	
  fXe = new G4Element("Xenon", "Xe", 54., 131.293 * g / mole);
  fAl = new G4Element("Aluminium", "Al", 13., 26.982 * g / mole);
  
  fTi = pNistManager->FindOrBuildElement("Ti");
  fMo = pNistManager->FindOrBuildElement("Mo");
  fCo = pNistManager->FindOrBuildElement("Co");



  //***Materials

  
  // Vacuum
  fVacuum = new G4Material("Vacuum", 1.e-20 * g / cm3, 2, kStateGas);
  fVacuum->AddElement(fN, 0.755);
  fVacuum->AddElement(fO, 0.245);
  
  // Air
  fAir = new G4Material("Air", density = 1.29 * mg / cm3, 2);
  fAir->AddElement(fN, 70 * perCent);
  fAir->AddElement(fO, 30 * perCent);
  
  // Glass
  fGlass = new G4Material("Glass", density = 1.032 * g / cm3, 2);
  fGlass->AddElement(fC, 91.533 * perCent);
  fGlass->AddElement(fH, 8.467 * perCent);
  
  // Polystyrene
  G4int polyPMMA = 1;
  G4int nC_PMMA  = 3 + 2 * polyPMMA;
  G4int nH_PMMA  = 6 + 2 * polyPMMA;

  G4int polyeth = 1;
  G4int nC_eth  = 2 * polyeth;
  G4int nH_eth  = 4 * polyeth;
  
  // Core (PS)
  Pstyrene = new G4Material("Polystyrene", density = 1.03 * g / cm3, 2);
  Pstyrene->AddElement(fC, 8);
  Pstyrene->AddElement(fH, 8);
  // Cladding(PMMA)
  PMMA = new G4Material("PMMA", density = 1190. * kg / m3, 3);
  PMMA->AddElement(fH, nH_PMMA);
  PMMA->AddElement(fC, nC_PMMA);
  PMMA->AddElement(fO, 2);
  
  PVDF = new G4Material("PVDF", density = 1780. * kg / m3, 3); ;
  PVDF->AddElement(fH, 2);
  PVDF->AddElement(fC, 2);
  PVDF->AddElement(fF, 2);
    
  fSS304LSteel = new G4Material("SS304LSteel", 8.00 * g / cm3, 5, kStateSolid);
  fSS304LSteel->AddElement(fFe, 0.65);
  fSS304LSteel->AddElement(fCr, 0.20);
  fSS304LSteel->AddElement(fNi, 0.12);
  fSS304LSteel->AddElement(fMn, 0.02);
  fSS304LSteel->AddElement(fSi, 0.01);
  
  // LXe
  fLXe = new G4Material("LXe", 2.85 * g / cm3, 1, kStateLiquid, 168.15 * kelvin, 1.5 * atmosphere);
  fLXe->AddElement(fXe, 1);
  
  
  // TEFLON
  fTeflon = new G4Material("Teflon", 2.2*g/cm3, 2, kStateSolid);
  fTeflon->AddElement(fC, 0.240183);
  fTeflon->AddElement(fF, 0.759817);

  // GXe
  fGXe = new G4Material("GXe", 0.005887 * g / cm3, 1, kStateGas, 173.15 * kelvin, 1.5 * atmosphere);
  fGXe->AddElement(fXe, 1);
  
  // Acrylic
  fAcrylic = new G4Material("Acrylic", 1.18 * g / cm3, 3, kStateSolid, 168.15 * kelvin, 1.5 * atmosphere);
  fAcrylic->AddElement(fC, 5);
  fAcrylic->AddElement(fH, 8);
  fAcrylic->AddElement(fO, 2);
  
  // Quartz
  fQuartz = new G4Material("Quartz", 2.201 * g / cm3, 2, kStateSolid, 168.15 * kelvin, 1.5 * atmosphere);
  fQuartz->AddElement(fSi, 1);
  fQuartz->AddElement(fO, 2);
  
  // Cirlex
  fCirlex = new G4Material("Cirlex", 1.43 * g / cm3, 4, kStateSolid);
  fCirlex->AddElement(fC, 22);
  fCirlex->AddElement(fH, 10);
  fCirlex->AddElement(fN, 2);
  fCirlex->AddElement(fO, 5);
  
  // SS316Ti
  G4int ncomponent;
  G4double fractionmass;
  fSS316Ti = new G4Material("SS316Ti", 8.00 * g / cm3, ncomponent = 8, kStateSolid);
  fSS316Ti->AddElement(fCr, fractionmass = 18.00 * perCent);
  fSS316Ti->AddElement(fNi, fractionmass = 14.00 * perCent);
  fSS316Ti->AddElement(fMo, fractionmass = 3.00 * perCent);
  fSS316Ti->AddElement(fMn, fractionmass = 2.00 * perCent);
  fSS316Ti->AddElement(fC, fractionmass = 0.80 * perCent);
  fSS316Ti->AddElement(fSi, fractionmass = 0.75 * perCent);
  fSS316Ti->AddElement(fTi, fractionmass = 0.70 * perCent);
  fSS316Ti->AddElement(fFe, fractionmass = 60.75 * perCent);
  
  // TiGrade1
  fTiGrade1 = new G4Material("TiGrade1", 4.51 * g / cm3, ncomponent = 6, kStateSolid);
  fTiGrade1->AddElement(fC, fractionmass = 0.10 * perCent);
  fTiGrade1->AddElement(fFe, fractionmass = 0.20 * perCent);
  fTiGrade1->AddElement(fH, fractionmass = 0.015 * perCent);
  fTiGrade1->AddElement(fN, fractionmass = 0.03 * perCent);
  fTiGrade1->AddElement(fO, fractionmass = 0.18 * perCent);
  fTiGrade1->AddElement(fTi, fractionmass = 99.475 * perCent);
  
  // TopScreeningMesh
  fTopScreeningMesh = new G4Material("TopScreeningMesh", 0.48 * g / cm3, 1, kStateSolid);
  fTopScreeningMesh->AddMaterial(fSS316Ti,1);  // density is (100.-94.)/100. * 8.g/cm3
  
  // BottomScreeningMesh
  fBottomScreeningMesh = new G4Material("BottomScreeningMesh", 0.48 * g / cm3, 1, kStateSolid);
  fBottomScreeningMesh->AddMaterial(fSS316Ti, 1);
  
  // CathodeMesh
  fCathodeMesh = new G4Material("CathodeMesh", 0.48 * g / cm3, 1, kStateSolid);
  fCathodeMesh->AddMaterial(fSS316Ti, 1);
  
  // AnodeMesh
  fAnodeMesh = new G4Material("AnodeMesh", 0.48 * g / cm3, 1, kStateSolid);
  fAnodeMesh->AddMaterial(fSS316Ti, 1);
  
  // GateMesh
  fGateMesh = new G4Material("GateMesh", 0.48 * g / cm3, 1, kStateSolid);
  fGateMesh->AddMaterial(fSS316Ti, 1);
  
  // Copper
  fCopper = new G4Material("Copper", 8.92 * g / cm3, 1);
  fCopper->AddElement(fCu, 1);
  
  // Kovar
  fKovar = new G4Material("Kovar", 8.33 * g / cm3, 6, kStateSolid);
  fKovar->AddElement(fFe, 0.5358);
  fKovar->AddElement(fNi, 0.29);
  fKovar->AddElement(fCo, 0.17);
  fKovar->AddElement(fC, 0.0002);
  fKovar->AddElement(fSi, 0.001);
  fKovar->AddElement(fMn, 0.003);
  
  // Ceramic
  fCeramic = new G4Material("Ceramic", 4. * g / cm3, 2, kStateSolid, 168.15 * kelvin, 1.5 * atmosphere);
  fCeramic->AddElement(fO, 3);
  fCeramic->AddElement(fAl, 2);
  
  // CoatingAluminium
  fCoatingAluminium = new G4Material("CoatingAluminium", 2.7 * g / cm3, 1, kStateSolid);
  fCoatingAluminium->AddElement(fAl, 1);
  
  // SensorDisk_material
  fSensorDisk_material = new G4Material("SensorDisk_material", 1.7124 * g / cm3, 5, kStateSolid); //Assumes 950 PMTs per disk (radius=1300 mm, thickness=20 mm)
  fSensorDisk_material->AddMaterial(fQuartz, 23.32 * perCent);
  fSensorDisk_material->AddMaterial(fKovar, 57.77 * perCent);
  fSensorDisk_material->AddMaterial(fCoatingAluminium, 0.32 * perCent); //CoatingAluminium=Pure Aluminium
  fSensorDisk_material->AddMaterial(fSS304LSteel, 10.19 * perCent);
  fSensorDisk_material->AddMaterial(fCeramic, 8.40 * perCent);
  
  // GXeTeflon
  fGXeTeflon = new G4Material("GXeTeflon", 2.2 * g / cm3, 2, kStateSolid);
  fGXeTeflon->AddElement(fC, 0.240183);
  fGXeTeflon->AddElement(fF, 0.759817);
  
  // SiPMMaterial
  fSiPMMaterial = new G4Material("SiPMMaterial", 1 * g / cm3, 1, kStateSolid);
  fSiPMMaterial->AddElement(fSi, 1);
  
  // PhotoCathodeAluminium
  fPhotoCathodeAluminium = new G4Material("PhotoCathodeAluminium", 8.00 * g / cm3, 1, kStateSolid);
  fPhotoCathodeAluminium->AddElement(fAl, 1);

   
            
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeDetectorConstruction::DefineMaterialsProperties()
{

  std::vector<G4double> lxe_Energy = { 2.00 * eV, 2.87 * eV, 2.90 * eV, 3.47 * eV, 7.0 * eV, 7.07 * eV, 7.14 * eV };
  // Set optical properties for Glass
  std::vector<G4double> glass_AbsLength = { 420. * cm, 420. * cm, 420. * cm,420. * cm,420. * cm,420. * cm,420. * cm };
  G4MaterialPropertiesTable* glass_mt   = new G4MaterialPropertiesTable();
  glass_mt->AddProperty("ABSLENGTH", lxe_Energy, glass_AbsLength);
  glass_mt->AddProperty("RINDEX", "Fused Silica");
  fGlass->SetMaterialPropertiesTable(glass_mt);

  // Set optical properties for Vacuum
  G4MaterialPropertiesTable* vacuum_mt = new G4MaterialPropertiesTable();
  vacuum_mt->AddProperty("RINDEX", "Air");
  fVacuum->SetMaterialPropertiesTable(vacuum_mt);
  fAir->SetMaterialPropertiesTable(vacuum_mt);  // Give air the same rindex

   //-----LXe------//
    
    G4int iNbEntries = 3;
    
    G4double pdLXePhotonMomentum[iNbEntries] = {6.91 * eV, 6.98 * eV, 7.05 * eV};
    G4double pdLXeScintillation[iNbEntries] = {0.1, 1.0, 0.1};
    G4double pdLXeRefractiveIndex[iNbEntries] = {1.71, 1.69, 1.66};
    G4double pdLXeAbsorbtionLength[iNbEntries] = {50. * m, 50. * m, 50. * m};
    G4double pdLXeScatteringLength[iNbEntries] = {fSLX, fSLX, fSLX};

    pLXePropertiesTable = new G4MaterialPropertiesTable();

    pLXePropertiesTable->AddProperty("FASTCOMPONENT", pdLXePhotonMomentum, pdLXeScintillation, iNbEntries, true);
    pLXePropertiesTable->AddProperty("SLOWCOMPONENT", pdLXePhotonMomentum, pdLXeScintillation, iNbEntries, true);
    pLXePropertiesTable->AddProperty("RINDEX", pdLXePhotonMomentum, pdLXeRefractiveIndex, iNbEntries, true);
    pLXePropertiesTable->AddProperty("ABSLENGTH", pdLXePhotonMomentum, pdLXeAbsorbtionLength, iNbEntries, true);
    pLXePropertiesTable->AddProperty("RAYLEIGH", pdLXePhotonMomentum, pdLXeScatteringLength, iNbEntries, true);

    pLXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 0. / keV, true);
    pLXePropertiesTable->AddConstProperty("RESOLUTIONSCALE", 0, true);
    pLXePropertiesTable->AddConstProperty("FASTTIMECONSTANT", 3. * ns, true);
    pLXePropertiesTable->AddConstProperty("SLOWTIMECONSTANT", 27. * ns, true);
    pLXePropertiesTable->AddConstProperty("YIELDRATIO", 1.0, true);
    
    fLXe->SetMaterialPropertiesTable(pLXePropertiesTable);


    //-----GXe------//

    G4double pdGXePhotonMomentum[iNbEntries] = {6.91 * eV, 6.98 * eV, 7.05 * eV};
    G4double pdGXeScintillation[iNbEntries] = {0.1, 1.0, 0.1};
    G4double pdGXeRefractiveIndex[iNbEntries] = {1.00, 1.00, 1.00};
    G4double pdGXeAbsorbtionLength[iNbEntries] = {100 * m, 100 * m, 100 * m};
    G4double pdGXeScatteringLength[iNbEntries] = {100 * m, 100 * m, 100 * m};

    pGXePropertiesTable = new G4MaterialPropertiesTable();

    pGXePropertiesTable->AddProperty("FASTCOMPONENT", pdGXePhotonMomentum, pdGXeScintillation, iNbEntries, true);
    pGXePropertiesTable->AddProperty("SLOWCOMPONENT", pdGXePhotonMomentum, pdGXeScintillation, iNbEntries, true);
    pGXePropertiesTable->AddProperty("RINDEX", pdGXePhotonMomentum, pdGXeRefractiveIndex, iNbEntries, true);
    pGXePropertiesTable->AddProperty("ABSLENGTH", pdGXePhotonMomentum, pdGXeAbsorbtionLength, iNbEntries, true);
    pGXePropertiesTable->AddProperty("RAYLEIGH", pdGXePhotonMomentum, pdGXeScatteringLength, iNbEntries, true);

    pGXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 0. / (keV), true);
    pGXePropertiesTable->AddConstProperty("RESOLUTIONSCALE", 0, true);
    pGXePropertiesTable->AddConstProperty("FASTTIMECONSTANT", 3. * ns, true);
    pGXePropertiesTable->AddConstProperty("SLOWTIMECONSTANT", 27. * ns, true);
    pGXePropertiesTable->AddConstProperty("YIELDRATIO", 1.0, true);
    
    fGXe->SetMaterialPropertiesTable(pGXePropertiesTable);
    
    
    //----PTFE-----//
    
    std::vector<G4double> pdTeflonPhotonMomentum = { 2.00 * eV, 2.87 * eV, 2.90 * eV, 3.47 * eV, 7.0 * eV, 7.07 * eV, 7.14 * eV  };
    std::vector<G4double> pdTeflonRefractiveIndex = {1.3, 1.306 , 1.306 , 1.311  , 1.63,    1.61,    1.58};
    std::vector<G4double> pdTeflonReflectivity = {1,    1,    1, 1, 1, 1, 1};
    std::vector<G4double> pdTeflonSpecularLobe =  {0.01,    0.01,    0.01, 0.01, 0.01, 0.01 ,0.01};
    std::vector<G4double> pdTeflonSpecularSpike =  {0.01,    0.01,    0.01, 0.01, 0.01, 0.01, 0.01};
    std::vector<G4double> pdTeflonBackscatter =  {0.01,    0.01,    0.01, 0.01, 0.01, 0.01, 0.01};
    std::vector<G4double> pdTeflonEfficiency =  {1,     1,     1, 1, 1, 1, 1};
    std::vector<G4double> pdTeflonTransmittance = {1e-12, 1e-12, 1e-12, 1e-12, 1e-12, 1e-12, 1e-12};
	
    pTeflonPropertiesTable = new G4MaterialPropertiesTable();
      
    pTeflonPropertiesTable->AddProperty("RINDEX", pdTeflonPhotonMomentum, pdTeflonRefractiveIndex);
    pTeflonPropertiesTable->AddProperty("REFLECTIVITY", pdTeflonPhotonMomentum, pdTeflonReflectivity);
    pTeflonPropertiesTable->AddProperty("SPECULARLOBECONSTANT", pdTeflonPhotonMomentum, pdTeflonSpecularLobe);
    pTeflonPropertiesTable->AddProperty("SPECULARSPIKECONSTANT", pdTeflonPhotonMomentum, pdTeflonSpecularSpike);
    pTeflonPropertiesTable->AddProperty("BACKSCATTERCONSTANT", pdTeflonPhotonMomentum, pdTeflonBackscatter);
    pTeflonPropertiesTable->AddProperty("EFFICIENCY", pdTeflonPhotonMomentum, pdTeflonEfficiency);
    pTeflonPropertiesTable->AddProperty("TRANSMITTANCE", pdTeflonPhotonMomentum, pdTeflonTransmittance	); 
    
    fTeflon->SetMaterialPropertiesTable(pTeflonPropertiesTable);
    
    
    //----SS304LSteel----//
    
    G4double pdSteelPhotonMomentum[] = {6.91*eV, 6.98*eV, 7.05*eV};
    G4double pdSteelReflectivity[]   = {0.15,    0.2,     0.15};
    G4double pdSteelEfficiency[] =  {0.85,     0.8,     0.85};
    G4double pdSteelAbsLength[] =  {1* mm,     1* mm,     1* mm};
    pSteelPropertiesTable = new G4MaterialPropertiesTable();

    pSteelPropertiesTable->AddProperty("REFLECTIVITY", pdSteelPhotonMomentum, pdSteelReflectivity, iNbEntries);
    pSteelPropertiesTable->AddProperty("EFFICIENCY", pdSteelPhotonMomentum, pdSteelEfficiency, iNbEntries);
    pSteelPropertiesTable->AddProperty("ABSLENGTH", pdSteelPhotonMomentum, pdSteelAbsLength, iNbEntries);
    fSS304LSteel->SetMaterialPropertiesTable(pSteelPropertiesTable);
    
    
    //---POLYSTYRENE---//
  
    std::vector<G4double> ps_n_energy = {1.*eV, 1.24137931*eV, 1.48275862*eV, 1.72413793*eV, 1.96551724*eV,2.20689655*eV, 2.44827586*eV, 2.68965517*eV, 2.93103448*eV, 3.17241379*eV,
                                       3.4137931*eV , 3.65517241*eV, 3.89655172*eV, 4.13793103*eV, 4.37931034*eV,4.62068966*eV, 4.86206897*eV, 5.10344828*eV, 5.34482759*eV, 5.5862069*eV ,
                                       5.82758621*eV, 6.06896552*eV, 6.31034483*eV, 6.55172414*eV, 6.79310345*eV,7.03448276*eV, 7.27586207*eV, 7.51724138*eV, 7.75862069*eV, 8. *eV};
  
    std::vector<G4double> ps_n = { 1.56886052, 1.57262149, 1.57695992, 1.5819645 , 1.58773749,1.59439688, 1.60207878, 1.61094019, 1.6211622 , 1.63295373,
                                 1.64655578, 1.66224633, 1.68034605, 1.70122487, 1.72530948,1.75309213, 1.78514062, 1.82210994, 1.86475565, 1.91394931,
                                 1.97069634, 2.03615649, 2.1116676 , 2.19877293, 2.29925271,2.41516047, 2.5488651 , 2.70309917, 2.88101484, 3.08624826};
  
    std::vector<G4double> ps_abs_energy = { 1 * eV, 2*eV, 3* eV, 3.5*eV };
  
    std::vector<G4double> ps_abs = { 4 * m, 4* m , 4* m, 4*m };
        
    std::vector<G4double> wls_abs_energy = {2.5*eV,3.0*eV,3.5*eV,4.14297299*eV,  4.34910719*eV,  4.62223305*eV,  4.87180152*eV,  5.19768254*eV,5.52626141*eV,  5.9116195*eV ,  6.34034404*eV,  6.73716986*eV,  
                                          7.31862896*eV, 7.80866709*eV,  8.47012392*eV,  8.90094832*eV,  9.28392842*eV,  9.80375918*eV,10.30881052*eV, 10.95336061*eV, 11.58665859*eV, 12.19063474*eV};
                                                
    std::vector<G4double> wls_abs = {4*m,4*m,4*m,0.0241464*mm, 0.0240253*mm,0.024035*mm , 0.0239147*mm, 0.0236714*mm, 0.0234321*mm, 0.0230785*mm,0.0227351*mm, 0.0220701*mm, 0.0214443*mm, 
                                   0.0208513*mm, 0.0199305*mm,0.0194159*mm, 0.0183841*mm, 0.0177327*mm, 0.0171913*mm, 0.0164377*mm,0.0155833*mm, 0.0148621*mm};
  
    std::vector<G4double> wls_emission_energy = {2.08230412*eV, 2.11850012*eV, 2.16363175*eV, 2.21877763*eV, 2.28964037*eV,2.36977778*eV, 2.44587156*eV, 2.5217854*eV , 2.59149453*eV, 2.63065063*eV,
                                               2.67100814*eV, 2.68868579*eV, 2.71262321*eV,  2.74315113*eV, 2.79986872*eV,2.83900166*eV, 2.87924401*eV, 2.9136612*eV , 2.97047354*eV, 2.97773124*eV,
                                               2.99971871*eV, 3.0220333*eV , 3.04468237*eV, 3.0676735*eV , 3.0831948*eV ,3.09887396*eV,3.09887396*eV, 3.15502959*eV, 3.19640315*eV, 3.30923196*eV,
                                               3.37334915*eV};
  
    std::vector<G4double> wls_emission = {0.00520833, 0.01041667, 0.015625  , 0.01041667, 0.046875  ,0.09895833, 0.16145833, 0.265625  , 0.35416667, 0.43229167,0.48958333, 0.546875  , 0.58333333, 
                                        0.63541667, 0.6875 ,0.74479167, 0.765625  , 0.75, 0.671875  , 0.61458333,0.57291667, 0.49479167, 0.390625  , 0.29166667, 0.203125  ,
                                        0.16666667, 0.11458333, 0.06770833, 0.00520833, 0.00520833,0.00520833};
  
    G4MaterialPropertiesTable* PSTableProperty   = new G4MaterialPropertiesTable();
                  
    PSTableProperty->AddProperty("RINDEX", ps_n_energy, ps_n);
    //PSTableProperty->AddProperty("ABSLENGTH", ps_abs_energy, ps_abs);
    PSTableProperty->AddProperty("WLSABSLENGTH", wls_abs_energy, wls_abs);
    PSTableProperty->AddProperty("WLSCOMPONENT", wls_emission_energy, wls_emission);
    PSTableProperty->AddConstProperty("WLSTIMECONSTANT", 0.5 * ns);
        
    Pstyrene->SetMaterialPropertiesTable(PSTableProperty);


       

    //---PMMA---//
  
    std::vector<G4double> pmma_abs = { 3.5 * m, 3.5 * m, 3.5 * m, 3.5 * m };
  
    std::vector<G4double> pmma_abs_energy = { 1 * eV, 2 * eV, 6 * eV, 10 * eV };

    std::vector<G4double> pmma_n_energy  = {1. *eV, 1.24137931*eV, 1.48275862*eV, 1.72413793*eV, 1.96551724*eV,2.20689655*eV, 2.44827586*eV, 2.68965517*eV, 2.93103448*eV, 3.17241379*eV,
                                          3.4137931 *eV, 3.65517241*eV, 3.89655172*eV, 4.13793103*eV, 4.37931034*eV,4.62068966*eV, 4.86206897*eV, 5.10344828*eV, 5.34482759*eV, 5.5862069 *eV,
                                          5.82758621*eV, 6.06896552*eV, 6.31034483*eV, 6.55172414*eV, 6.79310345*eV,7.03448276*eV, 7.27586207*eV, 7.51724138*eV, 7.75862069*eV, 8.  *eV };
        
    std::vector<G4double> pmma_n = {1.47962474, 1.4811668 , 1.4829135 , 1.48489202, 1.48713311,1.48967162, 1.49254703, 1.49580403, 1.49949329, 1.50367216,1.50840561, 1.51376726, 1.51984047, 1.52671967, 
                                  1.53451183,1.54333811, 1.55333575, 1.56466021, 1.57748757, 1.59201729,1.60847529, 1.62711748, 1.64823373, 1.67215238, 1.69924534,
                                  1.72993389, 1.76469521, 1.8040698 , 1.84866991, 1.89918904};
        
    G4MaterialPropertiesTable* PMMATableProperty = new G4MaterialPropertiesTable();
    PMMATableProperty->AddProperty("RINDEX", pmma_n_energy,pmma_n);
    PMMATableProperty->AddProperty("ABSLENGTH", pmma_abs_energy, pmma_abs);
        
    PMMA->SetMaterialPropertiesTable(PMMATableProperty);
  
  
  
    //---PVDF---//
  
    std::vector<G4double> pvdf_n_energy = {1.*eV, 1.24137931*eV, 1.48275862*eV, 1.72413793*eV, 1.96551724*eV,2.20689655*eV, 2.44827586*eV, 2.68965517*eV, 2.93103448*eV, 3.17241379*eV,
                                         3.4137931 *eV, 3.65517241*eV, 3.89655172*eV, 4.13793103*eV, 4.37931034*eV,4.62068966*eV, 4.86206897*eV, 5.10344828*eV, 5.34482759*eV, 5.5862069*eV ,
                                         5.82758621*eV, 6.06896552*eV, 6.31034483*eV, 6.55172414*eV, 6.79310345*eV,7.03448276*eV, 7.27586207*eV, 7.51724138*eV, 7.75862069*eV, 8.*eV    };
  
    std::vector<G4double> pvdf_n = {1.40602789, 1.40677367, 1.40768198, 1.40878824, 1.4101356 ,1.41177659, 1.41377522, 1.41620943, 1.41917413, 1.42278495,
                                   1.4271827 , 1.43253889, 1.43906237, 1.44700756, 1.4566843 ,1.46846995, 1.48282413, 1.5003066 , 1.52159913, 1.54753209,
                                  1.57911677, 1.61758491, 1.66443665, 1.72149909, 1.79099752,1.87564217, 1.97873397, 2.10429323, 2.25721641, 2.44346711};
       
    std::vector<G4double> pvdf_abs_energy = { 1 * eV, 4*eV, 7* eV, 10*eV };
    std::vector<G4double> pvdf_abs = { 2 * m, 2* m , 2* m, 2*m };
        
    G4MaterialPropertiesTable* PVDFTableProperty   = new G4MaterialPropertiesTable();
    PVDFTableProperty->AddProperty("RINDEX", pvdf_n_energy, pvdf_n);
    PVDFTableProperty->AddProperty("ABSLENGTH", pvdf_abs_energy, pvdf_abs);
        	
    PVDF->SetMaterialPropertiesTable(PVDFTableProperty);

    
    //---Acrylic----//
    
    G4double pdAcrylicPhotonMomentum[iNbEntries] = {2.59 * eV, 2.88 * eV, 3.17 * eV};
    G4double pdAcrylicRefractiveIndex[iNbEntries] = {1.50, 1.50, 1.50};
    G4double pdAcrylicAbsorbtionLength[iNbEntries] = {3.5 * m, 3.5 * m, 3.5 * m};
    G4double pdAcrylicScatteringLength[iNbEntries] = {100*m,   100*m,   100*m};

    pAcrylicPropertiesTable = new G4MaterialPropertiesTable();
    pAcrylicPropertiesTable->AddProperty("RINDEX", pdAcrylicPhotonMomentum, pdAcrylicRefractiveIndex, iNbEntries);
    pAcrylicPropertiesTable->AddProperty("ABSLENGTH", pdAcrylicPhotonMomentum, pdAcrylicAbsorbtionLength, iNbEntries);
    pAcrylicPropertiesTable->AddProperty("RAYLEIGH", pdAcrylicPhotonMomentum, pdAcrylicScatteringLength, iNbEntries);

    fAcrylic->SetMaterialPropertiesTable(pAcrylicPropertiesTable);
    
    
    //---Quartz----//
    
    const G4int iNbEntriesMatch = 5;
 
    G4double pdQuartzPhotonMomentum[iNbEntriesMatch] = {1. * eV, 6.9 * eV, 6.91 * eV, 6.98 * eV, 7.05 * eV};  // SERENA: changed  2.*eV to 1.*eV otherwise it gets stuck
    G4double pdQuartzRefractiveIndex[iNbEntriesMatch] = {1.50, 1.50, 1.55, 1.60, 1.60};

    G4double pdQuartzAbsorbtionLength[iNbEntriesMatch] = {30 * m, 30 * m, 30 * m, 30 * m, 30 * m};
    pQuartzPropertiesTable = new G4MaterialPropertiesTable();

    pQuartzPropertiesTable->AddProperty("RINDEX", pdQuartzPhotonMomentum, pdQuartzRefractiveIndex, iNbEntriesMatch);
    pQuartzPropertiesTable->AddProperty("ABSLENGTH", pdQuartzPhotonMomentum,  pdQuartzAbsorbtionLength, iNbEntriesMatch);

    fQuartz->SetMaterialPropertiesTable(pQuartzPropertiesTable);
    
    
    //---Cirlex----//
    
    G4double pdCirlexPhotonMomentum[]  = {6.91*eV, 6.98*eV, 7.05*eV};
    G4double pdCirlexReflectivity[]   = {0.5,    0.5,     0.5};

    pCirlexPropertiesTable = new G4MaterialPropertiesTable();

    pCirlexPropertiesTable->AddProperty("REFLECTIVITY",
    pdCirlexPhotonMomentum, pdCirlexReflectivity, iNbEntries);
    fCirlex->SetMaterialPropertiesTable(pCirlexPropertiesTable);


    //-----TopScreeningMesh------//

    G4double pdTopScreeningMeshPhotonMomentum[] = {6.91 * eV, 6.98 * eV, 7.05 * eV};
    G4double *pdTopScreeningMeshRefractiveIndex = pdGXeRefractiveIndex;
    G4double dTopScreeningMeshAbsortionLength = 3.2323021424 * mm;  // exp(-TopScreeningMeshThickness/dAbsortionLength) = // 0.94, with thickness of 0.2mm
    G4double pdTopScreeningMeshAbsortionLength[] = { dTopScreeningMeshAbsortionLength, dTopScreeningMeshAbsortionLength, dTopScreeningMeshAbsortionLength};
    
    pTopScreeningMeshPropertiesTable = new G4MaterialPropertiesTable();
    pTopScreeningMeshPropertiesTable->AddProperty( "RINDEX", pdTopScreeningMeshPhotonMomentum, pdTopScreeningMeshRefractiveIndex, iNbEntries);
    pTopScreeningMeshPropertiesTable->AddProperty( "ABSLENGTH", pdTopScreeningMeshPhotonMomentum, pdTopScreeningMeshAbsortionLength, iNbEntries);
    fTopScreeningMesh->SetMaterialPropertiesTable( pTopScreeningMeshPropertiesTable);
    
    
    //----BottomScreeningMesh----//
    
    G4double pdBottomScreeningMeshPhotonMomentum[] = {6.91 * eV, 6.98 * eV, 7.05 * eV};
    G4double *pdBottomScreeningMeshRefractiveIndex = pdLXeRefractiveIndex;
    G4double dBottomScreeningMeshAbsortionLength = 3.2323021424 * mm;  // exp(-BottomScreeningMeshThickness/dAbsortionLength) // = 0.94, with thickness of 0.2mm
    G4double pdBottomScreeningMeshAbsortionLength[] = {dBottomScreeningMeshAbsortionLength, dBottomScreeningMeshAbsortionLength, dBottomScreeningMeshAbsortionLength};
    
    pBottomScreeningMeshPropertiesTable = new G4MaterialPropertiesTable();
    pBottomScreeningMeshPropertiesTable->AddProperty("RINDEX", pdBottomScreeningMeshPhotonMomentum, pdBottomScreeningMeshRefractiveIndex, iNbEntries);
    pBottomScreeningMeshPropertiesTable->AddProperty("ABSLENGTH", pdBottomScreeningMeshPhotonMomentum, pdBottomScreeningMeshAbsortionLength, iNbEntries);
    fBottomScreeningMesh->SetMaterialPropertiesTable( pBottomScreeningMeshPropertiesTable);
    
    
    //-----CathodeMesh-----//
    
    G4double pdCathodeMeshPhotonMomentum[] = {6.91 * eV, 6.98 * eV, 7.05 * eV};
    G4double *pdCathodeMeshRefractiveIndex = pdLXeRefractiveIndex;
    G4double dCathodeMeshAbsortionLength = 3.2323021424 * mm;  // exp(-CathodeMeshThickness/dAbsortionLength) = 0.94, // with thickness of 0.2mm
    G4double pdCathodeMeshAbsortionLength[] = {dCathodeMeshAbsortionLength, dCathodeMeshAbsortionLength, dCathodeMeshAbsortionLength};
    
    pCathodeMeshPropertiesTable = new G4MaterialPropertiesTable();
    pCathodeMeshPropertiesTable->AddProperty("RINDEX", pdCathodeMeshPhotonMomentum, pdCathodeMeshRefractiveIndex,iNbEntries);
    pCathodeMeshPropertiesTable->AddProperty("ABSLENGTH", pdCathodeMeshPhotonMomentum, pdCathodeMeshAbsortionLength, iNbEntries);
    fCathodeMesh->SetMaterialPropertiesTable(pCathodeMeshPropertiesTable);
    
    
    //----AnodeMesh----//
    
    G4double pdAnodeMeshPhotonMomentum[] = {6.91 * eV, 6.98 * eV, 7.05 * eV};
    G4double *pdAnodeMeshRefractiveIndex = pdGXeRefractiveIndex;
    G4double dAnodeMeshAbsortionLength = 3.2323021424 * mm;  // exp(-AnodeMeshThickness/dAbsortionLength) = 0.94, // with thickness of 0.2mm
    G4double pdAnodeMeshAbsortionLength[] = {dAnodeMeshAbsortionLength, dAnodeMeshAbsortionLength, dAnodeMeshAbsortionLength};
    
    pAnodeMeshPropertiesTable = new G4MaterialPropertiesTable();
    pAnodeMeshPropertiesTable->AddProperty("RINDEX", pdAnodeMeshPhotonMomentum, pdAnodeMeshRefractiveIndex, iNbEntries);
    pAnodeMeshPropertiesTable->AddProperty("ABSLENGTH", pdAnodeMeshPhotonMomentum,  pdAnodeMeshAbsortionLength, iNbEntries);
    fAnodeMesh->SetMaterialPropertiesTable(pAnodeMeshPropertiesTable);
    
    
    //----GateMesh----//
    
    G4double pdGateMeshPhotonMomentum[] = {6.91 * eV, 6.98 * eV, 7.05 * eV};
    G4double *pdGateMeshRefractiveIndex = pdLXeRefractiveIndex;
    G4double dGateMeshAbsortionLength = 3.2323021424 * mm;  // exp(-GateMeshThickness/dAbsortionLength) = 0.94, // with thickness of 0.2mm
    G4double pdGateMeshAbsortionLength[] = {dGateMeshAbsortionLength, dGateMeshAbsortionLength, dGateMeshAbsortionLength};
    
    pGateMeshPropertiesTable = new G4MaterialPropertiesTable();
    pGateMeshPropertiesTable->AddProperty("RINDEX", pdGateMeshPhotonMomentum, pdGateMeshRefractiveIndex, iNbEntries);
    pGateMeshPropertiesTable->AddProperty("ABSLENGTH", pdGateMeshPhotonMomentum, pdGateMeshAbsortionLength, iNbEntries);
    fGateMesh->SetMaterialPropertiesTable(pGateMeshPropertiesTable);
    
    
    //---Copper---//
    
    G4double pdCopperPhotonMomentum[iNbEntries] = {1.91 * eV, 6.98 * eV, 7.05 * eV};  //{6.91*eV, 6.98*eV, 7.05*eV};
    G4double pdCopperReflectivity[iNbEntries] = {0.15, 0.2, 0.15};
    G4double pdCopperAbsLength[iNbEntries] = {1* mm, 1* mm, 1* mm};
    G4double pdCopperEfficiency[iNbEntries] = {0, 0, 0};
    
    pCopperPropertiesTable = new G4MaterialPropertiesTable();
    pCopperPropertiesTable->AddProperty("REFLECTIVITY", pdCopperPhotonMomentum, pdCopperReflectivity, iNbEntries);
    pCopperPropertiesTable->AddProperty("ABSLENGTH", pdCopperPhotonMomentum, pdCopperAbsLength, iNbEntries);
    pCopperPropertiesTable->AddProperty("EFFICIENCY", pdCopperPhotonMomentum, pdCopperEfficiency, iNbEntries);
    fCopper->SetMaterialPropertiesTable(pCopperPropertiesTable);

 
    //---SensorDisk_material----//
    
    G4double SensorDiskPhotonMomentum[iNbEntriesMatch] = {1. * eV, 6.9 * eV, 6.91 * eV, 6.98 * eV, 7.05 * eV};
    G4double SensorDiskRefractiveIndex[iNbEntriesMatch] = {1.50, 1.50, 1.55, 1.60, 1.62};

    pSensorDiskPropertiesTable = new G4MaterialPropertiesTable();
    pSensorDiskPropertiesTable->AddProperty("RINDEX", SensorDiskPhotonMomentum, SensorDiskRefractiveIndex, iNbEntriesMatch);
    fSensorDisk_material->SetMaterialPropertiesTable(pSensorDiskPropertiesTable);

    
    //----GXeTeflon----//
    
    G4double pdGXeTeflonPhotonMomentum[iNbEntries] = {6.91 * eV, 6.98 * eV, 7.05 * eV};
    G4double pdGXeTeflonRefractiveIndex[iNbEntries] = {1.63, 1.61, 1.58};
    G4double pdGXeTeflonReflectivity[iNbEntries] = {1.00, 1.00, 1.00}; //{0.99, 0.99, 0.99};
    G4double pdGXeTeflonSpecularLobe[iNbEntries] = {0.01, 0.01, 0.01};
    G4double pdGXeTeflonSpecularSpike[iNbEntries] = {0.01, 0.01, 0.01};
    G4double pdGXeTeflonBackscatter[iNbEntries] = {0.01, 0.01, 0.01};
    G4double pdGXeTeflonEfficiency[iNbEntries] = {1.0, 1.0, 1.0};
    G4double pdGXeTeflonTransmittance[iNbEntries] = {1e-12,1e-12,1e-12};

    pGXeTeflonPropertiesTable = new G4MaterialPropertiesTable();

    pGXeTeflonPropertiesTable->AddProperty("RINDEX", pdGXeTeflonPhotonMomentum, pdGXeTeflonRefractiveIndex,iNbEntries);
    pGXeTeflonPropertiesTable->AddProperty("REFLECTIVITY",pdGXeTeflonPhotonMomentum,pdGXeTeflonReflectivity, iNbEntries);
    pGXeTeflonPropertiesTable->AddProperty("SPECULARLOBECONSTANT",pdGXeTeflonPhotonMomentum,pdGXeTeflonSpecularLobe, iNbEntries);
    pGXeTeflonPropertiesTable->AddProperty("SPECULARSPIKECONSTANT",pdGXeTeflonPhotonMomentum,pdGXeTeflonSpecularSpike, iNbEntries);
    pGXeTeflonPropertiesTable->AddProperty("BACKSCATTERCONSTANT",pdGXeTeflonPhotonMomentum,pdGXeTeflonBackscatter, iNbEntries);
    pGXeTeflonPropertiesTable->AddProperty("EFFICIENCY",pdGXeTeflonPhotonMomentum,pdGXeTeflonEfficiency, iNbEntries);
    pGXeTeflonPropertiesTable->AddProperty("TRANSMITTANCE", pdGXeTeflonPhotonMomentum, pdGXeTeflonTransmittance, iNbEntries); 
       
    fGXeTeflon->SetMaterialPropertiesTable(pGXeTeflonPropertiesTable);
    
    
     //----PhotoCathode----//

    G4double pdPhotoCathodePhotonMomentum[] = {1. * eV, 6.9 * eV, 6.91 * eV, 6.98 * eV, 7.05 * eV};  // SERENA: changed  2.*eV to 1.*eV otherwise it gets stuck
    G4double pdPhotoCathodeRefractiveIndex[] = {1.50, 1.50, 1.55, 1.60, 1.62};
    G4double pdPhotoCathodeAbsorbtionLength[] = {1. * nm, 1 * nm, 1. * nm, 1. * nm, 1. * nm};
    
    G4MaterialPropertiesTable *pPhotoCathodePropertiesTable = new G4MaterialPropertiesTable();

    pPhotoCathodePropertiesTable->AddProperty( "RINDEX", pdPhotoCathodePhotonMomentum, pdPhotoCathodeRefractiveIndex, iNbEntriesMatch);
    pPhotoCathodePropertiesTable->AddProperty("ABSLENGTH", pdPhotoCathodePhotonMomentum, pdPhotoCathodeAbsorbtionLength,iNbEntriesMatch);
    fPhotoCathodeAluminium->SetMaterialPropertiesTable(pPhotoCathodePropertiesTable);
    


}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//


void LXeDetectorConstruction::DefinedOpticalPropertiesMaterials()
{

   // optical properties of Teflon in LXe
   pLXeTeflonOpticalSurface = new G4OpticalSurface("LXeTeflonOpticalSurface", unified, ground, dielectric_dielectric, 0.1);
   pLXeTeflonOpticalSurface->SetMaterialPropertiesTable(fTeflon->GetMaterialPropertiesTable());

   // optical properties of Teflon in GXe
   pGXeTeflonOpticalSurface = new G4OpticalSurface("GXeTeflonOpticalSurface", unified, ground, dielectric_dielectric, 0.1);
   pGXeTeflonOpticalSurface->SetMaterialPropertiesTable(fGXeTeflon->GetMaterialPropertiesTable());
}


//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//


G4VPhysicalVolume* LXeDetectorConstruction::Construct()
{
  // DIMENSIONS WORLD VOLUME
  G4double fWorld_x = fMain_x + fSiPM_thick + 1. * m;
  G4double fWorld_y = fMain_y + fSiPM_thick + 1. * m;
  G4double fWorld_z = fMain_z + fSiPM_thick + 1. * m;

  // CREATE WORLD VOLUME
  fWorld_box = new G4Box("World", fWorld_x, fWorld_y, fWorld_z);
  fWorld_log = new G4LogicalVolume(fWorld_box, fVacuum, "World", 0, 0, 0);
  fWorld_phys = new G4PVPlacement(0, G4ThreeVector(), fWorld_log, "World", 0, false, 0);

  fWorld_log->SetVisAttributes(G4VisAttributes::GetInvisible());

  // PLACE MAIN VOLUME INSIDE WORLD VOLUME
  fMainVolume = new LXeMainVolume(0, G4ThreeVector(), fWorld_log, false, 0, this);
  
  return fWorld_phys;
  
    
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeDetectorConstruction::ConstructSDandField()
{

  /*//here we define our sensitive volume. the last line of code call the function define in the header file of the class we want to use the function from.
  
  LXePMTSD* pmt = fPmt_SD.Get();
  // Created here so it exists as pmts are being placed
  G4cout << "Construction /LXeDet/pmtSD" << G4endl;
  LXePMTSD* pmt_SD = new LXePMTSD("/LXeDet/pmtSD");
  fPmt_SD.Put(pmt_SD);
    
  G4SDManager::GetSDMpointer()->AddNewDetector(fPmt_SD.Get());
  //SetSensitiveDetector(fMainVolume->GetLogSensitive(), fPmt_SD.Get());
  SetSensitiveDetector(fMainVolume->GetLogSiPM(), fPmt_SD.Get());*/
  
  
   //here we define our sensitive volume. the last line of code call the function define in the header file of the class we want to use the function from.
  
  LXePMTSD* pmt = fPmt_SD.Get();
  // Created here so it exists as pmts are being placed
  G4cout << "Construction /LXeDet/pmtSD" << G4endl;
  LXePMTSD* pmt_SD = new LXePMTSD("/LXeDet/pmtSD");
  fPmt_SD.Put(pmt_SD);
    
  G4SDManager::GetSDMpointer()->AddNewDetector(fPmt_SD.Get());
  SetSensitiveDetector(fMainVolume->GetLogSensitive(), fPmt_SD.Get());
   
}


//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeDetectorConstruction::SetDefaults()
{
  // Resets to default values
  
  fSiPM_thick = 0.0635 * cm;
  fMainWall_thick = 10 * cm;


  fMain_x = 5* m;
  fMain_y = 5 * m;
  fMain_z = 5 * m;

  fOuterRadius_pmt = 0.1 * cm + 2*0.015*0.1*cm;

  fRefl     = 1.0;
  
  fSLX = 5* mm;

  fNfibers      = 500;
  fMainVolume   = nullptr;
  
  fn_clad2 = 1.49;
  fn_clad1 = 1.42;
  fn_core  = 1.49;
  
  fAbsLength_core = 1.0 * mm;
  
  fWLSTime = 0.5 * ns;
 

  G4UImanager::GetUIpointer()->ApplyCommand("/LXe/detector/scintYieldFactor 1.");

}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeDetectorConstruction::SetHousingReflectivity(G4double r)
{
  fRefl = r;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeDetectorConstruction::SetNFibers(G4int n)
{
  fNfibers = n;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeDetectorConstruction::SetNClad2(G4double k)
{
  fn_clad2 = k;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeDetectorConstruction::SetNClad1(G4double k)
{
  fn_clad1 = k;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeDetectorConstruction::SetNCore(G4double k)
{
  fn_core = k;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeDetectorConstruction::SetWLSTime(G4double k)
{
  fWLSTime = k;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeDetectorConstruction::SetAbsLengthCore(G4double k)
{
  fAbsLength_core = k;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeDetectorConstruction::SetScattLengthLXe(G4double k)
{
  fSLX = k;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//
