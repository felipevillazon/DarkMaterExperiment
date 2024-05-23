#ifndef LXeDetectorConstruction_h
#define LXeDetectorConstruction_h 1

#include "LXeDetectorMessenger.hh"

#include "G4Cache.hh"
#include "G4VUserDetectorConstruction.hh"
#include "LXeDetector.hh"

class LXeMainVolume;
class LXePMTSD;
class LXeScintSD;
class LXeWLSSD;
class LXeWLSFiber;

class G4Box;
class G4Element;
class G4LogicalVolume;
class G4Material;
class G4MaterialPropertiesTable;
class G4Sphere;
class G4Tubs;
class G4VPhysicalVolume;
class G4UnionSolid;
class G4SubtractionSolid;
class G4OpticalSurface;


class LXeDetectorConstruction : public G4VUserDetectorConstruction
{
 public:
  LXeDetectorConstruction();
  ~LXeDetectorConstruction();

  G4VPhysicalVolume* Construct() override;
  void ConstructSDandField() override;

  // Functions to modify the geometry
  void SetDimensions(G4ThreeVector);
  void GetSiPMThickness(G4double);
  void GetMainWallThickness(G4double);
  void SetDefaults();



	
  // Get values
  G4double GetScintX() const { return fMain_x; }
  G4double GetScintY() const { return fMain_y; }
  G4double GetScintZ() const { return fMain_z; }
  G4double GetSiPMThickness() const { return fSiPM_thick;}
  G4double GetMainWallThickness() const { return fMainWall_thick;}

  void SetHousingReflectivity(G4double);
  G4double GetHousingReflectivity() const { return fRefl; }

  void SetNFibers(G4int n);
  G4int GetNFibers() const { return fNfibers; }

  void SetNClad2(G4double k);
  G4double GetNClad2() const { return fn_clad2; }
  
  void SetNClad1(G4double k);
  G4double GetNClad1() const { return fn_clad1; }
  
  void SetNCore(G4double k);
  G4double GetNCore() const { return fn_core; }
  
  void SetAbsLengthCore(G4double k);
  G4double GetAbsLengthCore() const { return fAbsLength_core; }
  
  void SetWLSTime(G4double k);
  G4double GetWLSTime() const { return fWLSTime; }
  
  void SetScattLengthLXe(G4double k);
  G4double GetScattLengthLXe() const { return fSLX; }


 private:


  void DefineMaterials();
  void DefineMaterialsProperties();
  void DefinedOpticalPropertiesMaterials();
  
  LXeDetectorMessenger* fDetectorMessenger;

  G4Box* fWorld_box;
  G4LogicalVolume* fWorld_log;
  G4VPhysicalVolume* fWorld_phys;

  // MATERIALS & ELEMENTS
  G4Material* fLXe;
  G4Material* fGXe;
  G4Element* fAl;
  G4Element* fN;
  G4Element* fO;
  G4Element* fF;
  G4Material* fAir;
  G4Material* fVacuum;
  G4Element* fC;
  G4Element* fH;
  G4Material* fGlass;
  G4Material* Pstyrene;
  G4Material* PMMA;
  G4Material* fPethylene1;
  G4Material* fPethylene2;
  G4Material* fTeflon;
  G4Element *fCr;
  G4Element *fMn;
  G4Element *fFe;
  G4Element *fNi;
  G4Element *fCu;
  G4Element *fSi;
  G4Element *fXe;	
  G4Material *fSS304LSteel;
  G4Material *fAcrylic;
  G4Element *fB;
  G4Material *fQuartz;
  G4Material *fCirlex;
  G4Material *fSS316Ti;
  G4Element *fTi;
  G4Element *fMo;
  G4Material *fTiGrade1;
  G4Material *fTopScreeningMesh;
  G4Material *fBottomScreeningMesh; 
  G4Material *fCathodeMesh;
  G4Material *fAnodeMesh;
  G4Material *fGateMesh;
  G4Material *fCopper;
  G4Material *fKovar;
  G4Material *fCeramic;
  G4Element *fCo;
  G4Material *fCoatingAluminium;
  G4Material *fSensorDisk_material;
  G4Material *fGXeTeflon;
  G4Material *fSiPMMaterial;
  G4Material *fPhotoCathodeAluminium;
  G4Material* PVDF;
  
  // OPTICAL SURFACES OF MATERIALS
  
  G4OpticalSurface *pLXeTeflonOpticalSurface;
  G4OpticalSurface *pGXeTeflonOpticalSurface;
  
  
  // GEOMETRY
  G4double fMain_x;
  G4double fMain_y;
  G4double fMain_z;
  G4double fSiPM_thick;
  G4double fMainWall_thick;
  G4int fNfibers;
  G4double fRefl;
  G4double fOuterRadius_pmt;
  G4double fn_clad2;
  G4double fn_clad1;
  G4double fn_core;
  G4double fAbsLength_core;
  G4double fWLSTime; 
  G4double fSLX;


  LXeMainVolume* fMainVolume;
  LXeWLSSD* fSensitive;
  LXeWLSFiber* fSiPM;

  // SENSITIVE DETECTORS
  G4Cache<LXeScintSD*> fScint_SD;
  G4Cache<LXePMTSD*> fPmt_SD;
  
  // PROPERTIES TABLES
  G4MaterialPropertiesTable *pLXePropertiesTable;
  G4MaterialPropertiesTable *pGXePropertiesTable;
  G4MaterialPropertiesTable *pTeflonPropertiesTable;
  G4MaterialPropertiesTable *pSteelPropertiesTable;
  G4MaterialPropertiesTable *pAcrylicPropertiesTable;
  G4MaterialPropertiesTable *clad2Property;
  G4MaterialPropertiesTable *clad1Property; 
  G4MaterialPropertiesTable *fiberProperty;
  G4MaterialPropertiesTable *pQuartzPropertiesTable;
  G4MaterialPropertiesTable *pCirlexPropertiesTable;
  G4MaterialPropertiesTable *pTopScreeningMeshPropertiesTable;
  G4MaterialPropertiesTable *pBottomScreeningMeshPropertiesTable;
  G4MaterialPropertiesTable *pCathodeMeshPropertiesTable;
  G4MaterialPropertiesTable *pAnodeMeshPropertiesTable;
  G4MaterialPropertiesTable *pGateMeshPropertiesTable;
  G4MaterialPropertiesTable *pCopperPropertiesTable;
  G4MaterialPropertiesTable *pSensorDiskPropertiesTable;
  G4MaterialPropertiesTable *pGXeTeflonPropertiesTable;
};

#endif
