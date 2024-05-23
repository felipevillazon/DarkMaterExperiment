#ifndef __DARWINPMTPLACER_H__
#define __DARWINPMTPLACER_H__

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <globals.hh>
#include <G4ThreeVector.hh>
#include <G4SystemOfUnits.hh>

class DarwinPMTPlacer
{
  public:

    DarwinPMTPlacer(G4String fname);
    ~DarwinPMTPlacer();

    int GetNTop();
    int GetNBottom();
    G4ThreeVector GetPMTPosition(int i_pmt);
    void PrintPMTPositions();
  private: 

    std::string filename;

    std::vector<double> x;
    std::vector<double> y;

    int n_top;
    int n_bot;
    
};
#endif
