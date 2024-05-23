
#ifndef LXeStackingAction_h
#define LXeStackingAction_h 1

#include "G4UserStackingAction.hh"

class LXeEventAction;

class LXeStackingAction : public G4UserStackingAction
{
 public:
  LXeStackingAction(LXeEventAction*);
  ~LXeStackingAction();

  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) override;

 private:
  LXeEventAction* fEventAction;
};

#endif
