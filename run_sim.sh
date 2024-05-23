#!/bin/bash

# Loop over values of nfibers from 10 to 1000 in steps of 10
for i in $(seq 100 100 1000)
do
    # Format the values of x for this macro file
    nfibers=$(printf "%.0f" $i)
    
    # Set the output filename for this simulation

    # Create the macro file with the appropriate values of x and z
    echo "/run/initialize" > run_${nfibers}.mac
    echo "/control/verbose 2" >> run_${nfibers}.mac
    echo "/run/verbose 2" >> run_${nfibers}.mac
    echo "/event/verbose 0" >> run_${nfibers}.mac
    echo "/tracking/verbose 0" >> run_${nfibers}.mac
    echo "/LXe/detector/nfibers ${nfibers}" >> run_${nfibers}.mac
    echo "/run/beamOn 1000" >> run_${nfibers}.mac

    # Run the simulation with this macro file and output filename
    ./LXe run_${nfibers}.mac

    # Move and rename the output file
    mv output0.root /home/felipe/Thesis/g4sim/DMsim/data/single_cladding_sim/nfibers_${nfibers}.root
    rm run_${nfibers}.mac
done



