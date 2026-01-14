#include "DAMSAAnalysis.hpp"
#include "G4SystemOfUnits.hh"
#include <iostream>

DAMSAAnalysis* DAMSAAnalysis::fInstance = nullptr;

DAMSAAnalysis* DAMSAAnalysis::Instance()
{
    if(!fInstance) {
        fInstance = new DAMSAAnalysis();
    }
    return fInstance;
}

DAMSAAnalysis::DAMSAAnalysis()
: fTargetExitNeutrons(0), fTargetExitPhotons(0),
  fTargetExitElectrons(0), fTargetExitPositrons(0),
  fTargetExitEnergy(0),
  fDetectorNeutrons(0), fDetectorPhotons(0),
  fDetectorElectrons(0), fDetectorPositrons(0),
  fDetectorEnergy(0),
  fDetectorPhotonEnergyCount(0),
  fDetectorNeutronCount(0),
  fDetectorElectronCount(0),
  fDetectorPositronCount(0)
{}

DAMSAAnalysis::~DAMSAAnalysis()
{}

void DAMSAAnalysis::RecordParticle(const G4String& particleName, G4double energy, const G4String& location, G4double angle)
{
    if(location == "TargetExit") {
        if(particleName == "neutron") fTargetExitNeutrons++;
        else if(particleName == "gamma") fTargetExitPhotons++;
        else if(particleName == "e-") fTargetExitElectrons++;
        else if(particleName == "e+") fTargetExitPositrons++;
        fTargetExitEnergy += energy;
    }
    else if(location == "DetectorEntrance") {
        if(particleName == "neutron") {
            fDetectorNeutrons++;
            if(fDetectorNeutronCount < kMaxNeutrons) {
                fDetectorNeutronEnergies[fDetectorNeutronCount] = energy;
                fDetectorNeutronAngles[fDetectorNeutronCount] = angle;
                fDetectorNeutronCount++;
            }
        }
        else if(particleName == "gamma") {
            fDetectorPhotons++;
            if(fDetectorPhotonEnergyCount < kMaxPhotons) {
                fDetectorPhotonEnergies[fDetectorPhotonEnergyCount] = energy;
                fDetectorPhotonAngles[fDetectorPhotonEnergyCount] = angle;
                fDetectorPhotonEnergyCount++;
            }
        }
        else if(particleName == "e-") {
            fDetectorElectrons++;
            if(fDetectorElectronCount < kMaxElectrons) {
                fDetectorElectronEnergies[fDetectorElectronCount] = energy;
                fDetectorElectronAngles[fDetectorElectronCount] = angle;
                fDetectorElectronCount++;
            }
        }
        else if(particleName == "e+") {
            fDetectorPositrons++;
            if(fDetectorPositronCount < kMaxPositrons) {
                fDetectorPositronEnergies[fDetectorPositronCount] = energy;
                fDetectorPositronAngles[fDetectorPositronCount] = angle;
                fDetectorPositronCount++;
            }
        }
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

    // Photon energy spectrum
    if(fDetectorPhotonEnergyCount > 0) {
        G4cout << "\n=== Photon Energy Spectrum at Detector Entrance ===" << G4endl;
        G4double bins[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nBins = 9;
        G4int counts[9] = {0};
        
        for(G4int i = 0; i < fDetectorPhotonEnergyCount; i++) {
            G4double E = fDetectorPhotonEnergies[i] / GeV;
            for(G4int j = 0; j < nBins; j++) {
                if(E >= bins[j] && E < bins[j+1]) {
                    counts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nBins; i++) {
            G4cout << "  [" << bins[i] << " - " << bins[i+1] << " GeV]: "
                   << counts[i] << " photons" << G4endl;
        }
        G4cout << "  Total photons recorded: " << fDetectorPhotonEnergyCount << G4endl;
    }
    
    // Photon angular distribution
    if(fDetectorPhotonEnergyCount > 0) {
        G4cout << "\n=== Photon Angular Distribution at Detector Entrance ===" << G4endl;
        G4double angleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nAngleBins = 7;
        G4int angleCounts[7] = {0};
        
        for(G4int i = 0; i < fDetectorPhotonEnergyCount; i++) {
            G4double angleDeg = fDetectorPhotonAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nAngleBins; j++) {
                if(angleDeg >= angleBins[j] && angleDeg < angleBins[j+1]) {
                    angleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis, 0° = straight forward)" << G4endl;
        for(G4int i = 0; i < nAngleBins; i++) {
            G4cout << "  [" << angleBins[i] << " - " << angleBins[i+1] << " degrees]: " 
                   << angleCounts[i] << " photons" << G4endl;
        }
    }
    
    // Neutron energy and angular distribution
    if(fDetectorNeutronCount > 0) {
        G4cout << "\n=== Neutron Energy Spectrum at Detector Entrance ===" << G4endl;
        G4double neutronEnergyBins[] = {0.0, 0.1, 0.5, 1.0, 2.0, 5.0, 10.0, 50.0, 100.0};
        G4int nNeutronEnergyBins = 8;
        G4int neutronEnergyCounts[8] = {0};
        
        for(G4int i = 0; i < fDetectorNeutronCount; i++) {
            G4double E = fDetectorNeutronEnergies[i] / GeV;
            for(G4int j = 0; j < nNeutronEnergyBins; j++) {
                if(E >= neutronEnergyBins[j] && E < neutronEnergyBins[j+1]) {
                    neutronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nNeutronEnergyBins; i++) {
            G4cout << "  [" << neutronEnergyBins[i] << " - " << neutronEnergyBins[i+1] 
                   << " GeV]: " << neutronEnergyCounts[i] << " neutrons" << G4endl;
        }
        
        G4cout << "\n=== Neutron Angular Distribution at Detector Entrance ===" << G4endl;
        G4double neutronAngleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nNeutronAngleBins = 7;
        G4int neutronAngleCounts[7] = {0};
        
        for(G4int i = 0; i < fDetectorNeutronCount; i++) {
            G4double angleDeg = fDetectorNeutronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nNeutronAngleBins; j++) {
                if(angleDeg >= neutronAngleBins[j] && angleDeg < neutronAngleBins[j+1]) {
                    neutronAngleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis)" << G4endl;
        for(G4int i = 0; i < nNeutronAngleBins; i++) {
            G4cout << "  [" << neutronAngleBins[i] << " - " << neutronAngleBins[i+1] 
                   << " degrees]: " << neutronAngleCounts[i] << " neutrons" << G4endl;
        }
    }
    
    // Electron energy and angular distribution
    if(fDetectorElectronCount > 0) {
        G4cout << "\n=== Electron Energy Spectrum at Detector Entrance ===" << G4endl;
        G4double electronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nElectronEnergyBins = 7;
        G4int electronEnergyCounts[7] = {0};
        
        for(G4int i = 0; i < fDetectorElectronCount; i++) {
            G4double E = fDetectorElectronEnergies[i] / GeV;
            for(G4int j = 0; j < nElectronEnergyBins; j++) {
                if(E >= electronEnergyBins[j] && E < electronEnergyBins[j+1]) {
                    electronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nElectronEnergyBins; i++) {
            G4cout << "  [" << electronEnergyBins[i] << " - " << electronEnergyBins[i+1] 
                   << " GeV]: " << electronEnergyCounts[i] << " electrons" << G4endl;
        }
        
        G4cout << "\n=== Electron Angular Distribution at Detector Entrance ===" << G4endl;
        G4double electronAngleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nElectronAngleBins = 7;
        G4int electronAngleCounts[7] = {0};
        
        for(G4int i = 0; i < fDetectorElectronCount; i++) {
            G4double angleDeg = fDetectorElectronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nElectronAngleBins; j++) {
                if(angleDeg >= electronAngleBins[j] && angleDeg < electronAngleBins[j+1]) {
                    electronAngleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis)" << G4endl;
        for(G4int i = 0; i < nElectronAngleBins; i++) {
            G4cout << "  [" << electronAngleBins[i] << " - " << electronAngleBins[i+1] 
                   << " degrees]: " << electronAngleCounts[i] << " electrons" << G4endl;
        }
    }
    
    // Positron energy and angular distribution
    if(fDetectorPositronCount > 0) {
        G4cout << "\n=== Positron Energy Spectrum at Detector Entrance ===" << G4endl;
        G4double positronEnergyBins[] = {0.0, 0.1, 0.2, 0.5, 1.0, 2.0, 4.0, 8.0};
        G4int nPositronEnergyBins = 7;
        G4int positronEnergyCounts[7] = {0};
        
        for(G4int i = 0; i < fDetectorPositronCount; i++) {
            G4double E = fDetectorPositronEnergies[i] / GeV;
            for(G4int j = 0; j < nPositronEnergyBins; j++) {
                if(E >= positronEnergyBins[j] && E < positronEnergyBins[j+1]) {
                    positronEnergyCounts[j]++;
                    break;
                }
            }
        }
        
        for(G4int i = 0; i < nPositronEnergyBins; i++) {
            G4cout << "  [" << positronEnergyBins[i] << " - " << positronEnergyBins[i+1] 
                   << " GeV]: " << positronEnergyCounts[i] << " positrons" << G4endl;
        }
        
        G4cout << "\n=== Positron Angular Distribution at Detector Entrance ===" << G4endl;
        G4double positronAngleBins[] = {0.0, 5.0, 10.0, 20.0, 30.0, 45.0, 60.0, 90.0};
        G4int nPositronAngleBins = 7;
        G4int positronAngleCounts[7] = {0};
        
        for(G4int i = 0; i < fDetectorPositronCount; i++) {
            G4double angleDeg = fDetectorPositronAngles[i] * 180.0 / 3.14159265;
            for(G4int j = 0; j < nPositronAngleBins; j++) {
                if(angleDeg >= positronAngleBins[j] && angleDeg < positronAngleBins[j+1]) {
                    positronAngleCounts[j]++;
                    break;
                }
            }
        }
        
        G4cout << "  (Angle relative to beam axis)" << G4endl;
        for(G4int i = 0; i < nPositronAngleBins; i++) {
            G4cout << "  [" << positronAngleBins[i] << " - " << positronAngleBins[i+1] 
                   << " degrees]: " << positronAngleCounts[i] << " positrons" << G4endl;
        }
    }
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
    fDetectorNeutronCount = 0;
    fDetectorElectronCount = 0;
    fDetectorPositronCount = 0;
}
