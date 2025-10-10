cmake_minimum_required(VERSION 3.16)
project(DAMSA_Simulation)

# Find GEANT4 package
find_package(Geant4 REQUIRED ui_all vis_all)

# Include GEANT4 headers
include(${Geant4_USE_FILE})

# Include directories
include_directories(${PROJECT_SOURCE_DIR})

# Set sources
set(sources
    damsa_sim.cpp
    damsa_energy_scan.cpp
    DAMSADetectorConstruction.cpp
    DAMSAPrimaryGeneratorAction.cpp
    DAMSASensitiveDetector.cpp
)

set(headers
    DAMSADetectorConstruction.hpp
    DAMSAPrimaryGeneratorAction.hpp
    DAMSASensitiveDetector.hpp
)

# Add executable
add_executable(damsa_sim damsa_sim.cpp DAMSADetectorConstruction.cpp DAMSAPrimaryGeneratorAction.cpp DAMSASensitiveDetector.cpp)
add_executable(damsa_energy_scan damsa_energy_scan.cpp DAMSADetectorConstruction.cpp DAMSAPrimaryGeneratorAction.cpp DAMSASensitiveDetector.cpp)

# Link GEANT4 libraries
target_link_libraries(damsa_sim ${Geant4_LIBRARIES})
target_link_libraries(damsa_energy_scan ${Geant4_LIBRARIES})

# Copy scripts to build directory
set(DAMSA_SCRIPTS
)

foreach(_script ${DAMSA_SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYONLY
    )
endforeach()
