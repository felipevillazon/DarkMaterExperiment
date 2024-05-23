
#ifndef LXeTrajectory_h
#define LXeTrajectory_h 1

#include "G4Allocator.hh"
#include "G4Track.hh"
#include "G4Trajectory.hh"

class G4Polyline;
class G4ParticleDefinition;

class LXeTrajectory : public G4Trajectory
{
 public:
  LXeTrajectory();
  LXeTrajectory(const G4Track* aTrack);
  LXeTrajectory(LXeTrajectory&);
  ~LXeTrajectory();

  void DrawTrajectory() const override;

  inline void* operator new(size_t);
  inline void operator delete(void*);

  void SetDrawTrajectory(G4bool b) { fDrawit = b; }
  void WLS() { fWls = true; }
  void SetForceDrawTrajectory(G4bool b) { fForceDraw = b; }
  void SetForceNoDrawTrajectory(G4bool b) { fForceNoDraw = b; }

 private:
  G4bool fWls;
  G4bool fDrawit;
  G4bool fForceNoDraw;
  G4bool fForceDraw;
  G4ParticleDefinition* fParticleDefinition;
};

extern G4ThreadLocal G4Allocator<LXeTrajectory>* LXeTrajectoryAllocator;

inline void* LXeTrajectory::operator new(size_t)
{
  if(!LXeTrajectoryAllocator)
    LXeTrajectoryAllocator = new G4Allocator<LXeTrajectory>;
  return (void*) LXeTrajectoryAllocator->MallocSingle();
}

inline void LXeTrajectory::operator delete(void* aTrajectory)
{
  LXeTrajectoryAllocator->FreeSingle((LXeTrajectory*) aTrajectory);
}

#endif
