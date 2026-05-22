cmake_minimum_required(VERSION 3.16)
project(DAMSA_Simulation)

# Find GEANT4 package
find_package(Geant4 REQUIRED ui_all vis_all)

# Find ROOT package
find_package(ROOT REQUIRED)

# Include GEANT4 headers
include(${Geant4_USE_FILE})

# Include ROOT headers
include_directories(${ROOT_INCLUDE_DIRS})

# Include directories
include_directories(${PROJECT_SOURCE_DIR})

# Common source files
set(common_sources
    DAMSADetectorConstruction.cpp
    DAMSAPrimaryGeneratorAction.cpp
    DAMSASensitiveDetector.cpp
    DAMSASteppingAction.cpp
    DAMSAAnalysis.cpp
)

# Executable 1: Visualization
add_executable(damsa_sim 
    damsa_sim.cpp 
    ${common_sources}
)
target_link_libraries(damsa_sim ${Geant4_LIBRARIES})

# Executable 2: Background Analysis (counts only)
add_executable(damsa_background_analysis
    damsa_background_analysis.cpp
    ${common_sources}
)
target_link_libraries(damsa_background_analysis ${Geant4_LIBRARIES})

# Executable 3: Energy & Angular Distributions (ROOT histograms)
add_executable(damsa_energy_angular_distributions
    damsa_energy_angular_distributions.cpp
    ${common_sources}
)
target_link_libraries(damsa_energy_angular_distributions ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# Legacy executables (if needed)
add_executable(damsa_gap_scan
    damsa_gap_scan.cpp
    ${common_sources}
)
target_link_libraries(damsa_gap_scan ${Geant4_LIBRARIES})


add_executable(damsa_tof_analysis
    damsa_tof_analysis.cpp
    ${common_sources}
)
target_link_libraries(damsa_tof_analysis ${Geant4_LIBRARIES})
