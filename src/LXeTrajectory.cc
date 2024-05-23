
#include "LXeTrajectory.hh"

#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4Polyline.hh"
#include "G4Polymarker.hh"
#include "G4ThreeVector.hh"
#include "G4Trajectory.hh"
#include "G4TrajectoryPoint.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"

G4ThreadLocal G4Allocator<LXeTrajectory>* LXeTrajectoryAllocator = nullptr;

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeTrajectory::LXeTrajectory()
  : G4Trajectory()
  , fWls(false)
  , fDrawit(false)
  , fForceNoDraw(false)
  , fForceDraw(false)
{
  fParticleDefinition = nullptr;
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeTrajectory::LXeTrajectory(const G4Track* aTrack)
  : G4Trajectory(aTrack)
  , fWls(false)
  , fDrawit(false)
{
  fParticleDefinition = aTrack->GetDefinition();
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeTrajectory::LXeTrajectory(LXeTrajectory& right)
  : G4Trajectory(right)
  , fWls(right.fWls)
  , fDrawit(right.fDrawit)
{
  fParticleDefinition = right.fParticleDefinition;
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeTrajectory::~LXeTrajectory() {}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeTrajectory::DrawTrajectory() const
{
  // Taken from G4VTrajectory and modified to select colours based on particle
  // type and to selectively eliminate drawing of certain trajectories.

/*  if(!fForceDraw && (!fDrawit || fForceNoDraw))
    return;

  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(!pVVisManager)
    return;

  const G4double markerSize = 0.05;
  G4bool lineRequired       = true;
  G4bool markersRequired    = true;

  G4Polyline trajectoryLine;
  G4Polymarker stepPoints;
  G4Polymarker auxiliaryPoints;

  for(G4int i = 0; i < GetPointEntries(); ++i)
  {
    G4VTrajectoryPoint* aTrajectoryPoint = GetPoint(i);
    const std::vector<G4ThreeVector>* auxiliaries =
      aTrajectoryPoint->GetAuxiliaryPoints();
    if(auxiliaries)
    {
      for(size_t iAux = 0; iAux < auxiliaries->size(); ++iAux)
      {
        const G4ThreeVector pos((*auxiliaries)[iAux]);
        if(lineRequired)
        {
          trajectoryLine.push_back(pos);
        }
        if(markersRequired)
        {
          auxiliaryPoints.push_back(pos);
        }
      }
    }
    const G4ThreeVector pos(aTrajectoryPoint->GetPosition());
    if(lineRequired)
    {
      trajectoryLine.push_back(pos);
    }
    if(markersRequired)
    {
      stepPoints.push_back(pos);
    }
  }


  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();


  G4Polyline trajectoryLine;
  G4Polymarker stepPoints;
  G4Polymarker auxiliaryPoints;

  const G4double markerSize = 0.05;
  G4bool lineRequired       = true;
  G4bool markersRequired    = true; 

    G4Colour colour;

    if(fParticleDefinition == G4OpticalPhoton::OpticalPhotonDefinition())
    {
      if(fWls)  // WLS photons are red
        colour = G4Colour(1., 0., 0.);
      else
      {  // Scintillation and Cerenkov photons are green
        colour = G4Colour(0., 1., 0.);
      }
    }
    else  // All other particles are blue
      colour = G4Colour(0., 0., 1.);

    G4VisAttributes trajectoryLineAttribs(colour);
    trajectoryLine.SetVisAttributes(&trajectoryLineAttribs);
    pVVisManager->Draw(trajectoryLine);*/
  }

