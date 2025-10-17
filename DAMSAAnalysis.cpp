#include "DAMSAAnalysis.hpp"
#include "G4SystemOfUnits.hh"
#include <iostream>

DAMSAAnalysis* DAMSAAnalysis::fInstance = nullptr;  \\initialize the singleton pointer to null

DAMSAAnalysis* DAMSAAnalysis::Instance()  \\gets the singleton instance
{
    if(!fInstance) {                          \\ checks if instance exists
        fInstance = new DAMSAAnalysis();      \\if it doesn't it creates new DAMSAAnalysis
    }
    return fInstance;                         \\returns pointer to instance
}

DAMSAAnalysis::DAMSAAnalysis()               \\constructor-initializes all counters to zero
: fTargetExitNeutrons(0), fTargetExitPhotons(0),
  fTargetExitElectrons(0), fTargetExitPositrons(0),
  fTargetExitEnergy(0),
  fDetectorNeutrons(0), fDetectorPhotons(0),
  fDetectorElectrons(0), fDetectorPositrons(0),
  fDetectorEnergy(0)
{}

DAMSAAnalysis::~DAMSAAnalysis()
{}

void DAMSAAnalysis::RecordParticle(const G4String& particleName, G4double energy, const G4String& location) #records particle at target exit
{
    if(location == "TargetExit") {
        if(particleName == "neutron") fTargetExitNeutrons++;   \\check particle type, increment appropriate counter
        else if(particleName == "gamma") fTargetExitPhotons++;
        else if(particleName == "e-") fTargetExitElectrons++;
        else if(particleName == "e+") fTargetExitPositrons++;
        fTargetExitEnergy += energy;  #add energy to total
  }
    else if(location == "DetectorEntrance") {               \\same logic for detector entrance counters
        if(particleName == "neutron") fDetectorNeutrons++;
        else if(particleName == "gamma") fDetectorPhotons++;
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
}
