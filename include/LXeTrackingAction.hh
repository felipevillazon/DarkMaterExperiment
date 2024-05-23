
#ifndef LXeTrackingAction_h
#define LXeTrackingAction_h 1

#include "globals.hh"
#include "G4UserTrackingAction.hh"

class LXeTrackingAction : public G4UserTrackingAction
{
 public:
  LXeTrackingAction();
  ~LXeTrackingAction(){};

  void PreUserTrackingAction(const G4Track*) override;
  void PostUserTrackingAction(const G4Track*) override;

 private:
};

#endif
