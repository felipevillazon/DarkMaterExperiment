

#ifndef LXeHistoManager_h
#define LXeHistoManager_h 1

#include "globals.hh"
#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class LXeHistoManager
{
 public:
  LXeHistoManager();
  ~LXeHistoManager();

 private:
  void Book();
  G4String fFileName;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
