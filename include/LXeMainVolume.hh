#ifndef LXeMainVolume_h
#define LXeMainVolume_h 1
#include "LXeDetectorConstruction.hh"
#include "DarwinConstructSensors.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include <math.h>
#include <vector>
#include "G4Polycone.hh" 
using std::vector;
#include "DarwinPMTPlacer.hh"

class G4Box;
class G4LogicalVolume;
class G4Sphere;
class G4Tubs;
class G4UnionSolid;
class LXeWLSFiber;
class G4SubtractionSolid;


class LXeMainVolume : public G4PVPlacement
{
 public:
  
  LXeMainVolume(G4RotationMatrix* pRot, const G4ThreeVector& tlate, G4LogicalVolume* pMotherLogical, G4bool pMany, G4int pCopyNo, LXeDetectorConstruction* c);

  G4LogicalVolume* GetLogSensitive() { return fSensitive_log; }
  G4LogicalVolume* GetLogMain() { return fMain_Logical; }
  G4LogicalVolume* GetLogSiPM() { return  m_pSensitiveSiPMOutTPCLogical; }
  
  void SetArrayTypes(G4String, G4String);
  void SetPMTPositionsFile(G4String);
  G4UnionSolid* BuildVesselShape( G4double Diameter_cyl, G4double Height_cyl, G4double Thickness_Wall );
  G4SubtractionSolid* BuildFillerShape( G4double Diameter_cyl, G4double Height_cyl, G4double Thickness_Wall );
  
  G4double GetPositiveZ() const {return PositiveTargetVolumeZ;}
  G4double GetNegativeZ() const {return NegativeTargetVolumeZ;}
  G4double GetInnerRadius() const {return InnerRadiusTargetVolume;}
  G4double GetOuterRadius() const {return OuterRadiusTargetVolume;}
  
  

 private:
  void VisAttributes();
  void BorderSurfaceProperties();
  void SkinSurfaceProperties();
  void ConstructTopRealisticSensorArray();
  void ConstructBottomRealisticSensorArray();
  void PlaceSiPMs(G4LogicalVolume* pmt_log, G4RotationMatrix* rot, G4int n, G4double& z, G4int k);
  void PrintGeometryInformation(); 
  void CopyValues();


  LXeDetectorConstruction* fConstructor;
  LXeDetectorConstruction* pDet;
  LXeDetectorConstruction *OpMaterialProperties;

   
  //----PARAMETERS FOR PARTICLE GUN----//
  //-----------------------------------//
   
  G4double NegativeTargetVolumeZ;
  G4double PositiveTargetVolumeZ;
  G4double InnerRadiusTargetVolume;
  G4double OuterRadiusTargetVolume;
  

  G4double fSiPM_thick;
  G4double z;
  G4int k;
  G4double fRefl;

  // Basic Volumes
  //
  G4Tubs* fSiPM_tube;
  G4Tubs* fSensitive_tube;
  G4Tubs* Tube1;
  G4Tubs* Tube2;
  G4SubtractionSolid* fShield_tube;
  // Logical volumes
  //
  G4LogicalVolume* fSiPM_log;
  G4LogicalVolume* fSensitive_log;
  G4LogicalVolume* fShield_log;
  
  // Physical volumes
  G4VPhysicalVolume *up;
  G4VPhysicalVolume *down;
  G4VPhysicalVolume *Sensitive;
 
 
  
  
  
  //----MAIN VOLUME----//
  //-------------------//
  //-------------------//
  
  //Paramenters for the Main
  G4double fMain_x;
  G4double fMain_y;
  G4double fMain_z;
  G4double fMainWall_thick;
  
  //SolidVolumes
  G4Box* fMain_box;
  G4Box* fMainWall_box;
  
  //LogicalVolumes
  G4LogicalVolume* fMain_Logical;
  G4LogicalVolume* fMainWall_Logical;
  
  
  
  
 
  //----WLS FIBER----//
  //-----------------//
  //-----------------//
  
  //Parameters for the WLS fibers
  G4int fNfibers; 
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
  
  //LogicalVolumes
  G4LogicalVolume* Fiber_Logical;
  G4LogicalVolume* Clad1_Logical;
  G4LogicalVolume* Clad2_Logical;
  
  //PhysicalVolumes
  G4VPhysicalVolume *Fiber_Physical;
  G4VPhysicalVolume *Clad1_Physical;
  G4VPhysicalVolume *Clad2_Physical;
  





  //----INNER CRYOSTAT---// 
  //---------------------//
  //---------------------//

  //Parameters for the Cryostat Envelop
  G4double CryostatEnvelop_Radius;  
  G4double CryostatEnvelop_Height;

  //Parameters for the inner Cryostat
  G4double InnerCryo_WallThickness; 
  G4double InnerCryo_InnerCylDiameter; 
  G4double InnerCryo_InnerCylHeight; 
  G4double InnerCryo_FlangeHeight;
  G4double InnerCryo_FlangeThickness;
  G4double InnerCryo_StiffenerHeight;
  G4double InnerCryo_StiffenerThickness;
  G4int InnerCryo_StiffenerNumber;

  //LogicalVolumes
  G4LogicalVolume* CryostatVolume_Logical;

  G4LogicalVolume* InnerCryostatVessel_Logical;
  G4LogicalVolume* InnerCryostatVolume_Logical;
  G4LogicalVolume* InnerCryostatBottomVolume_Logical;
  G4LogicalVolume* InnerCryostatBottomFillerVessel_Logical;
  G4LogicalVolume* InnerCryostatBottomFillerVolume_Logical;
  G4LogicalVolume* InnerCryostatTPCVolume_Logical;

  //PhysicalVolumes
  G4VPhysicalVolume* InnerCryostatVessel_Physical;
  G4VPhysicalVolume* InnerCryostatVolume_Physical;
  G4VPhysicalVolume* InnerCryostatBottomVolume_Physical;
  G4VPhysicalVolume* InnerCryostatBottomFillerVessel_Physical;
  G4VPhysicalVolume* InnerCryostatBottomFillerVolume_Physical;
  G4VPhysicalVolume* InnerCryostatTPCVolume_Physical;
  
  
  
  //--------TPC----------// 
  //---------------------//
  //---------------------//
  
  //Parameters
   G4double bPrintTopPos;
   G4double bPrintBottomPos;
   
   G4String sTopArrayType;
   G4String sBottomArrayType;

   G4double dCryostatInnerRadius;
   G4double dCryostatInnerHeight;
   G4double dActiveLXeRadius;
   G4double dActiveLXeHeight;

   G4double dGXeHeight;
   G4double dLXeExtraFilling;

   G4double dReflectorThickness;
   G4double dReflectorBottomHeight;
   G4double dPillarThickness;
   G4double dPillarWidth;
   G4int iPillarNb;

   G4double dFSRThickness;
   G4double dFSRHeight;
   G4double dFSRInnerRadius;
   G4int iFSRNb;
   G4double dFSRSeparation;
   G4double dFSROffsetZ;

   G4int iNbOfTopPMTs;
   G4int iNbOfBottomPMTs;

   G4double dPMTHeight;
   G4double dPMTRadiusHead;

   G4double dPMTRadiusBody;

   G4double dBasesThickness;
   G4double dBasesRadius;
   G4double dPTFEArrayReflectorThickness;
   G4double dPTFEArrayHolderThickness;
   G4double dCuSensorArrayThickness;
   G4double dSensorArrayRadius;
   
   G4double dBasesArrayDiskThickness;
   G4double dCuSensorArrayDiskThickness;
   G4double dPTFESensorArrayDiskThickness;
   G4double dCuSensorArrayHoleRadius;
   
   G4double dElectrodesFrameThickness;
   G4double dElectrodesTopFrameHeight;
   G4double dCathodeFrameHeight;
   G4double dBottomscreenFrameHeight;
   G4double dTopScreeningFrameHeight;

   G4double dTopScreeningThickness;
   G4double dAnodeThickness; 
   G4double dGateAnodeDistance;
   G4double dGateThickness; 
   G4double dCathodeThickness; 
   G4double dBottomScreeningThickness;

   G4double dSensorThickness;
   G4double dSensorWindowThickness;
   
   G4ThreeVector GetPMTsPosition(G4int);
   
   //Solids
   G4Tubs *pTPCCylinder;
   G4Tubs *pGXeCylinder; 
   G4Tubs *pLXeExtraFillingInside;
   G4Tubs *pLXeExtraFillingOutside;
   G4Tubs *pLXeSensitive; 

   G4Tubs *pPTFEReflector;
   G4Tubs *pPTFEReflectorTop;
   G4Tubs *pPTFEReflectorBottom;
   G4Box *pPTFEPillar;
   G4Box *pPTFEPillarBottom;

   G4Tubs *pTiElectrodesTopFrame;
   G4Tubs *pTiCathodeFrame;
   G4Tubs *pTiBottomscreenFrame; 

   G4Tubs *pCuFSR;

   G4Tubs *pCirlexBases;
   G4Tubs *pCuSensorArray;
   G4Tubs *pPTFESensorArrayThin;
   G4Tubs *pPTFESensorArrayThick;

   // Realistic PTFE reflector
   G4SubtractionSolid *m_pPTFEArrayReflectorTop;
   //G4LogicalVolume *pPTFERealisticReflectorTopLogical;   
    
   G4SubtractionSolid *m_pPTFEArrayReflectorBottom;
   //G4LogicalVolume *pPTFERealisticReflectorBottomLogical;  
    
   G4SubtractionSolid *m_pCuArrayTopCut;
   G4SubtractionSolid *m_pPTFEHolderTop;  

   G4SubtractionSolid *m_pCuArrayBottomCut;
   G4SubtractionSolid *m_pPTFEHolderBottom;  
   // effective disk array
   G4Tubs *pCirlexBasesArrayDisk;
   G4Tubs *pCuSensorArrayDisk;
   G4Tubs *pPTFESensorArrayDisk;
   //LogicalVolumes
   G4LogicalVolume *m_pTPCLogical;
   G4LogicalVolume *m_pLXeLogical;
   G4LogicalVolume *m_pGXeLogical;
   G4LogicalVolume *m_pLXeExtraFillingInsideLogical;
   G4LogicalVolume *m_pLXeExtraFillingOutsideLogical;
   G4LogicalVolume *m_pLXeSensitiveLogical; 

   G4LogicalVolume *m_pPTFEReflectorLogical;
   G4LogicalVolume *m_pPTFEReflectorTopLogical;
   G4LogicalVolume *m_pPTFEReflectorBottomLogical;
   G4LogicalVolume *m_pPTFEPillarLogical;
   G4LogicalVolume *m_pPTFEPillarBottomLogical;

   G4LogicalVolume *m_pTiElectrodesTopFrameLogical;
   G4LogicalVolume *m_pTiCathodeFrameLogical;
   G4LogicalVolume *m_pTiBottomscreenFrameLogical;

   G4LogicalVolume *m_pCuFSRLogical;

   G4LogicalVolume *m_pCirlexBasesLogical;
   G4LogicalVolume *m_pCuArrayTopLogical;
   G4LogicalVolume *m_pCuArrayBottomLogical;
   G4LogicalVolume *m_pPTFEArrayReflectorTopLogical;
   G4LogicalVolume *m_pPTFEArrayReflectorBottomLogical;
   G4LogicalVolume *m_pPTFEArrayHolderTopLogical;
   G4LogicalVolume *m_pPTFEArrayHolderBottomLogical;


   G4LogicalVolume *m_pPMTR11410;
   G4LogicalVolume *m_pPMTArrayTopLogical;
   G4LogicalVolume *m_pPMTArrayBottomLogical;
   G4LogicalVolume *m_pCirlexBasesArrayDiskLogical;
   G4LogicalVolume *m_pCuArrayDiskLogical;
   G4LogicalVolume *m_pPTFEArrayDiskLogical;
   //PhysicalVolumes
   G4VPhysicalVolume *m_pLXePhysical;
   G4VPhysicalVolume *m_pGXePhysical; 
   G4VPhysicalVolume *m_pLXeExtraFillingInsidePhysical;
   G4VPhysicalVolume *m_pLXeExtraFillingOutsidePhysical;
   G4VPhysicalVolume *m_pLXeSensitivePhysical;

   G4VPhysicalVolume *m_pPTFEReflectorPhysical;
   G4VPhysicalVolume *m_pPTFEReflectorTopPhysical;
   G4VPhysicalVolume *m_pPTFEReflectorBottomPhysical;
   G4VPhysicalVolume *m_pPTFEPillarPhysical;
   G4VPhysicalVolume *m_pPTFEPillarBottomPhysical;

   G4VPhysicalVolume *m_pTiElectrodesTopFramePhysical;
   G4VPhysicalVolume *m_pTiCathodeFramePhysical;
   G4VPhysicalVolume *m_pTiBottomscreenFramePhysical;
 
   G4VPhysicalVolume *m_pCuFSRPhysical;  

   G4VPhysicalVolume *m_pTopScreeningPhysical;
   G4VPhysicalVolume *m_pAnodePhysical;
   G4VPhysicalVolume *m_pGatePhysical;
   G4VPhysicalVolume *m_pCathodePhysical;
   G4VPhysicalVolume *m_pBottomScreeningPhysical;

   G4VPhysicalVolume *m_pCuArrayTopPhysical;
   G4VPhysicalVolume *m_pCuArrayBottomPhysical;
   G4VPhysicalVolume *m_pPTFEArrayReflectorTopPhysical;
   G4VPhysicalVolume *m_pPTFEArrayReflectorBottomPhysical;
   G4VPhysicalVolume *m_pPTFEArrayHolderTopPhysical;
   G4VPhysicalVolume *m_pPTFEArrayHolderBottomPhysical;
   vector<G4VPhysicalVolume*> m_pCirlexBasesTopPhysical;
   vector<G4VPhysicalVolume*> m_pPMTTopPhysical;

   vector<G4VPhysicalVolume*> m_pCirlexBasesBottomPhysical;
   vector<G4VPhysicalVolume*> m_pPMTBottomPhysical;
   
   G4VPhysicalVolume *m_pCuArrayDiskTopPhysical;
   G4VPhysicalVolume *m_pPTFEArrayDiskTopPhysical;
   G4VPhysicalVolume *m_pCirlexBasesDiskTopPhysical;
   G4VPhysicalVolume *m_pPMTArrayTopPhysical;
   
   G4VPhysicalVolume *m_pCuArrayDiskBottomPhysical;
   G4VPhysicalVolume *m_pPTFEArrayDiskBottomPhysical;
   G4VPhysicalVolume *m_pCirlexBasesDiskBottomPhysical;
   G4VPhysicalVolume *m_pPMTArrayBottomPhysical;
   
    DarwinPMTPlacer *pPMTplacer;
    
  //--------BORDER SURFACE PROPERTIES---------// 
  //------------------------------------------//
  //------------------------------------------//
  
  G4OpticalSurface* InnerCryostat_LXe;
  G4MaterialPropertiesTable* InnerCryostat_LXeProperties ;
  G4OpticalSurface* InnerCryostatBottomFillerVessel_LXe;
  G4MaterialPropertiesTable* InnerCryostatBottomFillerVessel_LXeProperties;
  G4OpticalSurface* InnerCryostatVessel_LXe;
  G4MaterialPropertiesTable* InnerCryostatVessel_LXeProperties;
  G4OpticalSurface* InnerCryostat_GXe;
  G4MaterialPropertiesTable* InnerCryostat_GXeProperties;
  G4OpticalSurface* InnerCryostatBottomFillerVessel_GXe;
  G4MaterialPropertiesTable* InnerCryostatBottomFillerVessel_GXeProperties;
  G4OpticalSurface* InnerCryostatVessel_GXe;
  G4MaterialPropertiesTable* InnerCryostatVessel_GXeProperties;
  G4OpticalSurface* PTFE_reflectorSurface;
  G4MaterialPropertiesTable* PTFE_reflectorSurfaceProperties;
  G4OpticalSurface* PTFE_reflector_TopSurface;
  G4MaterialPropertiesTable* PTFE_reflector_TopSurfaceProperties;
  G4OpticalSurface* PTFE_reflector_TopExtraFillingSurface;
  G4MaterialPropertiesTable* PTFE_reflector_TopExtraFillingSurfaceProperties;
  G4OpticalSurface* PTFE_TopAnodeSurface;
  G4MaterialPropertiesTable* PTFE_TopAnodeSurfaceProperties;
  G4OpticalSurface* PTFE_TopTopScreeningSurface;
  G4MaterialPropertiesTable* PTFE_TopTopScreeningSurfaceProperties;
  G4OpticalSurface* PTFE_reflector_BottomTPCSurface;
  G4MaterialPropertiesTable* PTFE_reflector_BottomTPCSurfaceProperties;
  G4OpticalSurface* CuFSRTPCSurface;
  G4MaterialPropertiesTable* CuFSRTPCSurfaceProperties;
  G4OpticalSurface* CuArrayDiskTopTPCSurface;
  G4MaterialPropertiesTable* CuArrayDiskTopTPCSurfaceProperties;
  G4OpticalSurface* PTFEReflectorTPCSurface;
  G4MaterialPropertiesTable* PTFEReflectorTPCSurfaceProperties;
  G4OpticalSurface* PTFEReflectorOutTPCSurface;
  G4MaterialPropertiesTable* PTFEReflectorOutTPCSurfaceProperties;
  
  G4OpticalSurface* PTFEReflectorCryostaturface;
  G4MaterialPropertiesTable* PTFEReflectorCryostaturfaceProperties;
  G4OpticalSurface* SensitiveLXesurface;
  G4MaterialPropertiesTable* SensitiveLXesurfaceProperties;
  
  


  //--------SKIN SURFACE PROPERTIES-----------// 
  //------------------------------------------//
  //------------------------------------------//
  
  G4OpticalSurface* InnerCrysotatSkin;
  G4MaterialPropertiesTable* InnerCrysotatSkinProperties;
  G4OpticalSurface* CuArrayDiskSkin;
  G4MaterialPropertiesTable* CuArrayDiskSkinProperties;
  G4OpticalSurface* PTFEArrayDiskSkin;
  G4MaterialPropertiesTable* PTFEArrayDiskSkinProperties;
  G4OpticalSurface* CuFSRSkin;
  G4MaterialPropertiesTable* CuFSRSkinProperties;
  G4OpticalSurface* PTFEPillarSkin;
  G4MaterialPropertiesTable* PTFEPillarSkinProperties;
  G4OpticalSurface* PTFEReflectorSkin;
  G4MaterialPropertiesTable* PTFEReflectorSkinProperties;
  G4OpticalSurface* PTFEReflectorBottomOutTPCSkin;
  G4MaterialPropertiesTable* PTFEReflectorBottomOutTPCSkinProperties;
  G4OpticalSurface* PTFEReflectorGXeLXeSkin;
  G4MaterialPropertiesTable* PTFEReflectorGXeLXeSkinProperties;
  G4OpticalSurface* PTFEReflectorBottomLXe;
  G4MaterialPropertiesTable* PTFEReflectorBottomLXeProperties;
  G4MaterialPropertiesTable *pSiPMPropertiesTable;
  G4OpticalSurface* SensitiveSiPMOutTPCSkin;
  
  //--------TEFLON REFELCTOR OUTSIDE TPC-----------// 
  //-----------------------------------------------//
  //-----------------------------------------------//
  
  G4Tubs* pPTFEReflectorOutTPC;
  G4LogicalVolume* m_pPTFEReflectorOutTPCLogical;
  G4VPhysicalVolume* m_pPTFEReflectorPhysical2;
  
  
  //--------TEFLON REFELCTOR CRYOSTAT--------------// 
  //-----------------------------------------------//
  //-----------------------------------------------//
  
  G4Tubs* pPTFEReflectorCryostat;
  G4LogicalVolume* m_pPTFEReflectorCryostatLogical;
  G4VPhysicalVolume* m_pPTFEReflectorCryostatPhysical;
  
  
  //--------TEFLON REFELCTOR BOTTOM OUTSIDE TPC-----------// 
  //------------------------------------------------------//
  //------------------------------------------------------//
  
  G4Tubs* pPTFEReflectorBottomOutTPC;
  G4LogicalVolume* m_pPTFEReflectorBottomOutTPCLogical;
  G4VPhysicalVolume* m_pPTFEReflectorBottomOutTPCPhysical;
  
  
  //--------TEFLON REFELCTOR Interface GXe LXe------------// 
  //------------------------------------------------------//
  //------------------------------------------------------//
  
  G4Tubs* pPTFEReflectorGXeLXe;
  G4LogicalVolume* m_pPTFEReflectorGXeLXeLogical;
  G4VPhysicalVolume* m_pPTFEReflectorGXeLXePhysical;
  
  
  //--------TEFLON REFELCTOR BOTTOM LXe-------------------// 
  //------------------------------------------------------//
  //------------------------------------------------------//
  
  G4Tubs* pPTFEReflectorBottomLXe;
  G4LogicalVolume* m_pPTFEReflectorBottomLXeLogical;
  G4VPhysicalVolume* m_pPTFEReflectorBottomLXePhysical;
  
  
  //--------SENSITIVE SiPM--------------------------------// 
  //------------------------------------------------------//
  //------------------------------------------------------//
  
  G4Tubs* pSensitiveSiPMOutTPC;
  G4LogicalVolume* m_pSensitiveSiPMOutTPCLogical;
  G4VPhysicalVolume* SensitiveDown;
  G4VPhysicalVolume* SensitiveUp;
  
  
};

#endif
