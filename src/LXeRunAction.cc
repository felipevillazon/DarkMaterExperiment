
#include "LXeRunAction.hh"

#include "LXeHistoManager.hh"
#include "LXeRun.hh"

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeRunAction::LXeRunAction()
  : fRun(nullptr)
  , fHistoManager(nullptr)
{
  // Book predefined histograms
  fHistoManager = new LXeHistoManager();
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

LXeRunAction::~LXeRunAction() { delete fHistoManager; }

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//
G4Run* LXeRunAction::GenerateRun()
{
  fRun = new LXeRun();
  return fRun;
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeRunAction::BeginOfRunAction(const G4Run* run)
{
  G4cout << "==============================================" << G4endl;
  G4cout << "Simulation is running" << G4endl;
  G4cout << "==============================================" << G4endl;
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if(analysisManager->IsActive())
  {
    analysisManager->OpenFile();
  }
  
  
         G4AnalysisManager *man = G4AnalysisManager::Instance();
	 
	 G4int runID = run->GetRunID();
	 
	 std::stringstream strRunID;
	 strRunID << runID;
	 

	  
	 man->OpenFile("output" +strRunID.str()+ ".root");
	 
	 man->CreateNtuple("Hits", "Photons");
	 man->CreateNtupleIColumn("fEvent");
	 man->CreateNtupleDColumn("fX");
	 man->CreateNtupleDColumn("fY");
	 man->CreateNtupleDColumn("fZ");
	 man->CreateNtupleDColumn("detector");
	 man->CreateNtupleDColumn("fEnergy");
	 man->CreateNtupleDColumn("fT");
	 man->CreateNtupleDColumn("fzVertex");
	 man->CreateNtupleDColumn("fyVertex");
	 man->CreateNtupleDColumn("fxVertex");
	 man->FinishNtuple(0);
	 
	 /*man->CreateNtuple("7eV_Hits", "7eV Photons");
	 man->CreateNtupleIColumn("fEvent");
	 man->CreateNtupleDColumn("fX");
	 man->CreateNtupleDColumn("fY");
	 man->CreateNtupleDColumn("fZ");
	 man->CreateNtupleDColumn("detector");
	 man->CreateNtupleDColumn("fEnergy");
	 man->CreateNtupleDColumn("fT");
	 man->FinishNtuple(1);*/
	 
}

//------//------//------//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//-----//

void LXeRunAction::EndOfRunAction(const G4Run*)
{
  if(isMaster)
    fRun->EndOfRun();

  // save histograms
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if(analysisManager->IsActive())
  {
    analysisManager->Write();
    analysisManager->CloseFile();
  }
  
  	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->Write();
	man->CloseFile();
}
