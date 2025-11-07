#include "DAMSAAnalysis.hpp"
#include "G4SystemOfUnits.hh"
#include <iostream>

DAMSAAnalysis* DAMSAAnalysis::fInstance = nullptr;  //initialize the singleton pointer to null

DAMSAAnalysis* DAMSAAnalysis::Instance()  //gets the singleton instance
{
    if(!fInstance) {                          // checks if instance exists
        fInstance = new DAMSAAnalysis();      //if it doesn't it creates new DAMSAAnalysis
    }
    return fInstance;                         //returns pointer to instance
}

DAMSAAnalysis::DAMSAAnalysis()               //constructor-initializes all counters to zero
: fTargetExitNeutrons(0), fTargetExitPhotons(0),
  fTargetExitElectrons(0), fTargetExitPositrons(0),
  fTargetExitEnergy(0),
  fDetectorNeutrons(0), fDetectorPhotons(0),
  fDetectorElectrons(0), fDetectorPositrons(0),
  fDetectorEnergy(0), fDetectorPhotonEnergyCount(0) // initializes photon energy counter to zero
{}

DAMSAAnalysis::~DAMSAAnalysis()
{}

void DAMSAAnalysis::RecordParticle(const G4String& particleName, G4double energy, const G4String& location, G4double angle) //records particle at target exit
{
    if(location == "TargetExit") {
        if(particleName == "neutron") fTargetExitNeutrons++;   //check particle type, increment appropriate counter
        else if(particleName == "gamma") fTargetExitPhotons++;
        else if(particleName == "e-") fTargetExitElectrons++;
        else if(particleName == "e+") fTargetExitPositrons++;
        fTargetExitEnergy += energy;  //add energy to total
  }
    else if(location == "DetectorEntrance") {               //same logic for detector entrance counters
        if(particleName == "neutron") fDetectorNeutrons++;
        else if(particleName == "gamma") {
            fDetectorPhotons++;   // counting photon as before

            //Storing photon energy for histograms
            if(fDetectorPhotonEnergyCount < kMaxPhotons) {  //check if array has space before storing
                fDetectorPhotonEnergies[fDetectorPhotonEnergyCount] = energy; // store eenrgy at next available array position
                fDetectorPhotonAngles[fDetectorPhotonEnergyCount]= angle; 
                fDetectorPhotonEnergyCount++;  //increment counter for next photon
            }
        }
        else if(particleName == "e-") fDetectorElectrons++;
        else if(particleName == "e+") fDetectorPositrons++;
        fDetectorEnergy += energy;
  }

}

void DAMSAAnalysis::PrintSummary()
{
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== Particles EXITING Target (Z=-75cm) ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    G4cout << "Neutrons:   " << fTargetExitNeutrons << G4endl;
    G4cout << "Photons:    " << fTargetExitPhotons << G4endl;
    G4cout << "Electrons:  " << fTargetExitElectrons << G4endl;
    G4cout << "Positrons:  " << fTargetExitPositrons << G4endl;
    G4cout << "Total Energy: " << fTargetExitEnergy/GeV << " GeV" << G4endl;
    
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== Particles ENTERING Detector (Z=-28cm) ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    G4cout << "Neutrons:   " << fDetectorNeutrons << G4endl;
    G4cout << "Photons:    " << fDetectorPhotons << G4endl;
    G4cout << "Electrons:  " << fDetectorElectrons << G4endl;
    G4cout << "Positrons:  " << fDetectorPositrons << G4endl;
    G4cout << "Total Energy: " << fDetectorEnergy/GeV << " GeV" << G4endl;
    G4cout << "=============================================\n" << G4endl;

// Print photon energy histogram at detector entrance
    if(fDetectorPhotonEnergyCount > 0) {  // only prints if photon data
        G4cout << "\n=== Photon Energy Spectrum at Detector Entrance ===" << G4endl;
        // Define energy bins (GeV)
        G4double bins[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nBins = 9;
        G4int counts[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  //array to count photons in each bin and initializing to 0
        // Bin the photon energies
        for(G4int i = 0; i < fDetectorPhotonEnergyCount; i++) {  //loop through all stored photon energies and bin them
            G4double E = fDetectorPhotonEnergies[i] / GeV;  //get energy of photon i and convert into GeV for comparison
            for(G4int j = 0; j < nBins; j++) {  //checking which bin
                if(E >= bins[j] && E < bins[j+1]) {  //checking energy of bin
                    counts[j]++;  //increment counter
                    break;     // breaks when finds the correct bin
                }
            }
        }
        // Print histogram
        for(G4int i = 0; i < nBins; i++) {
            G4cout << "  [" << bins[i] << " - " << bins[i+1] << " GeV]: "
                   << counts[i] << " photons" << G4endl;
        }
        G4cout << "  Total photons recorded: " << fDetectorPhotonEnergyCount << G4endl;
    }
    
    // Print photon angular distribution at detector entrance
    if(fDetectorPhotonEnergyCount > 0) {
        G4cout << "\n=== Photon Angular Distribution at Detector Entrance ===" << G4endl;
        // Define angle bins (degrees)
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0, 0, 0, 0, 0, 0, 0};
        
        // Bin the photon angles
        for(G4int i = 0; i < fDetectorPhotonEnergyCount; i++) {
            // Convert from radians to degrees
            G4double angleDeg = fDetectorPhotonAngles[i] * 180.0 / 3.14159265;
            
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        // Print histogram
        G4cout << "  (Angle relative to beam axis, 0° = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " photons" << G4endl;
        }
    }
    
    G4cout << "=============================================\n" << G4endl;
}

void DAMSAAnalysis::Reset()
{
    fTargetExitNeutrons = 0;
    fTargetExitPhotons = 0;
    fTargetExitElectrons = 0;
    fTargetExitPositrons = 0;
    fTargetExitEnergy = 0;
    
    fDetectorNeutrons = 0;
    fDetectorPhotons = 0;
    fDetectorElectrons = 0;
    fDetectorPositrons = 0;
    fDetectorEnergy = 0;
    fDetectorPhotonEnergyCount = 0;
}
