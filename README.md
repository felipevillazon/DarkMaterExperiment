# DarkMaterExperiment
Repository containing the files of the Geant4 simulations where I have designed a dark matter experiment with the focus on detecting neutron background and finding an efficient way of reading out scintillation or Cherenkov light. I have used geant4 version 11.4.0.

Scintillation light is created in a given volume of the simulation design which can be set by the user. Light can be read out directly by photodetectors or by using wavelength-shifting fibers. The output data is placed in a root file that can be later used for further analysis typically using root or uproot in python.
