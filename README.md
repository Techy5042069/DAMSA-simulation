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

##Details of detector geometry

-Tungsten Target: 5×5×10 cm³ solid block, positioned at Z = -80 cm
- Vacuum Chamber: Cylindrical, radius 10 cm, length 30 cm, centered at Z = -60 cm
- Vacuum Chamber Wall: 5 mm stainless steel (specified, not yet implemented)
- Silicon Tracker: 6 layers, each 12×12×0.2 cm³, positioned from Z = -40 cm to -30 cm with 2 cm spacing
- CsI Calorimeter: 12×12×44 cm³, centered at Z = -6 cm

## Beam Parameters

-Particle Type: Electrons
- Beam Energy: 8 GeV
- Beam Profile: Pinpoint (no transverse spread)
- Starting Position: Z = -90 cm

## Output
Simulation generates VRML files for visualization showing:

## Detector geometry
Particle trajectories color-coded by type (electrons: red, photons: green, positrons: cyan, neutrons: yellow)


