# DAMSA Detector Simulation

GEANT4 simulation for Dark Matter Search Apparatus (DAMSA) beam-related background studies.

## Overview

This simulation models an 8 GeV electron beam incident on a tungsten target to study background processes relevant for dark sector particle searches, specifically axion-like particles (ALPs) decaying to two photons.

## Requirements

- GEANT4 version 11.3 or later
- CMake 3.10 or later
- C++17 compatible compiler
- Docker (currently used environment)

## Build
mkdir build && cd build
cmake ...
make

## Execute the main simulation 
./damsa_sim

## Details of detector geometry

-Tungsten Target: 5×5×10 cm³ solid block, positioned at Z = -80 cm
- Vacuum Chamber: Cylindrical, radius 10 cm, length 30 cm, centered at Z = -60 cm
- Vacuum Chamber Wall: 5 mm stainless steel (specified, not yet implemented)
- Silicon Tracker: 6 layers, each 12×12×0.2 cm³, positioned from Z = -40 cm to -30 cm with 2 cm spacing
- CsI Calorimeter: 12×12×44 cm³, centered at Z = -6 cm
- Scoring Volume (invisible):
	*Target Exit: 30×30×0.2 mm³ at Z = -75 cm (counts particles exiting target)
	*Detector Entrance: 30×30×0.2 mm³ at Z = -28 cm (counts particles reaching detector)

## Beam Parameters

-Particle Type: Electrons
- Beam Energy: 8 GeV
- Position: Starts at Z
- Beam Profile: Pinpoint (no transverse spread)
- Starting Position: Z = -90 cm

## Output
Simulation generates VRML files for visualization showing:

## Detector geometry
Particle trajectories color-coded by type (electrons: red, photons: green, positrons: cyan, neutrons: yellow)

## DAMSA Time-of-flight analysis (TOF)
	1. This lets us see the arrival time of particles in the scoring volume which is infront of the detector entrance.
	2. With this information, we can do a photon/neutron discrimination. 
	3. The damsa_tof_analysis executable generates a csv file. The first column being `particle_type` and the second being `time of arrival`
	4. The python file generates a histogram with "time" bins.
### Usage
	1. Generate CSV file: ./damsa_tof_analysis
	2. Use tof_analysis.py: py plot_tof.py ./build/tof_data.csv --bin-width 0.5
