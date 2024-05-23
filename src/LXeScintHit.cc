#include "LXeScintHit.hh"

#include "G4ios.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VVisManager.hh"

G4ThreadLocal G4Allocator<LXeScintHit>* LXeScintHitAllocator = nullptr;

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeScintHit::LXeScintHit()
  : fEdep(0.)
  , fPos(0.)
  , fPhysVol(nullptr)
{}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeScintHit::LXeScintHit(G4VPhysicalVolume* pVol)
  : fPhysVol(pVol)
{}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeScintHit::~LXeScintHit() {}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeScintHit::LXeScintHit(const LXeScintHit& right)
  : G4VHit()
{
  fEdep    = right.fEdep;
  fPos     = right.fPos;
  fPhysVol = right.fPhysVol;
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

const LXeScintHit& LXeScintHit::operator=(const LXeScintHit& right)
{
  fEdep    = right.fEdep;
  fPos     = right.fPos;
  fPhysVol = right.fPhysVol;
  return *this;
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

G4bool LXeScintHit::operator==(const LXeScintHit&) const
{
  return false;
  // returns false because there currently isn't need to check for equality
}
