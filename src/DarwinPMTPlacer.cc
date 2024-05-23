#include "DarwinPMTPlacer.hh"
#include <cassert>

DarwinPMTPlacer::DarwinPMTPlacer(G4String fname){
  filename=fname;
  G4cout<<"DarwinPmtPlacer: PMT placer file: "<<filename<<G4endl;

  n_top=0; 
  n_bot=0;
  std::ifstream inFile(filename);
  std::string line;
  int i_line=0;
  bool bottom_started=false;
  if (inFile){
    while(getline(inFile, line, '\n')){
      // ignoring the commented lines
      if (line[0]=='#') continue;
      // reading PMT id, x, y and array
      int i_pmt;
      double x_, y_;
      std::string arr_;
      std::istringstream ss(line);
      ss>>i_pmt>>arr_>>x_>>y_;
      assert(i_line==i_pmt);
      // Counting top and bottom PMTs
      if (arr_=="top"){
        n_top++;
        // checking whether bottom PMTs always follow the top PMTs
        if (bottom_started==true){
          G4Exception("DarwinPmtPlacer::DarwinPmtPlacer()",
                      "DarwinPmtPlacer", FatalException,
                      "Bottom PMTs must always be after top PMTs"
                 );
        }
      } else if (arr_=="bottom"){
        n_bot++; 
        bottom_started=true;
      } else {
        G4Exception("DarwinPmtPlacer::DarwinPmtPlacer()",
                    "DarwinPmtPlacer", FatalException,
                    ("Array can be only top or bottom, received: "+arr_).c_str()
                 ); 
      }
      x.push_back(x_);
      y.push_back(y_);
      i_line++;
    }
  } else {
    G4Exception("DarwinPmtPlacer::DarwinPmtPlacer()",
                "DarwinPmtPlacer", FatalException,
                ("Cannot open with PMT positions:  "+filename).c_str()
             );
  }
  G4cout<<"Top PMTs: "<<n_top<<G4endl;
  G4cout<<"Bottom PMTs: "<<n_bot<<G4endl;
}

DarwinPMTPlacer::~DarwinPMTPlacer(){
}
int DarwinPMTPlacer::GetNTop(){
  return n_top;
}

int DarwinPMTPlacer::GetNBottom(){
  return n_bot;
}

G4ThreeVector DarwinPMTPlacer::GetPMTPosition(int i_pmt){
  return G4ThreeVector(x.at(i_pmt)*mm, y.at(i_pmt)*mm, 0.*mm);
}

void DarwinPMTPlacer::PrintPMTPositions(){
  G4cout<<"#PMTID\tarray\tx\ty"<<G4endl;
  for (int i=0;i<n_top;i++) G4cout<<i<<"\ttop\t"<<x.at(i)<<"\t"<<y.at(i)<<G4endl;
  for (int i=n_top; i < n_top+n_bot; i++) G4cout<<i<<"\tbottom\t"<<x.at(i)<<"\t"<<y.at(i)<<G4endl;
}
