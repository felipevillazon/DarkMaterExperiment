#ifndef LXeActionInitialization_h
#define LXeActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class LXeDetectorConstruction;

class LXeActionInitialization : public G4VUserActionInitialization
{
 public:
  LXeActionInitialization(const LXeDetectorConstruction* det);
  ~LXeActionInitialization();

  void BuildForMaster() const override;
  void Build() const override;

 private:
  const LXeDetectorConstruction* fDetector;
};

#endif
