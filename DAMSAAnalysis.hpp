#ifndef DAMSAAnalysis_h
#define DAMSAAnalysis_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

class DAMSAAnalysis
{
public:
    static DAMSAAnalysis* Instance();
    
    void RecordParticle(const G4String& particleName, G4double energy, const G4String& location, G4double angle);
    void PrintSummary();
    void Reset();
    
private:
    DAMSAAnalysis();
    ~DAMSAAnalysis();
    static DAMSAAnalysis* fInstance;
    
    // Counters at target exit
    G4int fTargetExitNeutrons;
    G4int fTargetExitPhotons;
    G4int fTargetExitElectrons;
    G4int fTargetExitPositrons;
    G4double fTargetExitEnergy;
    
    // Counters at detector entrance
    G4int fDetectorNeutrons;
    G4int fDetectorPhotons;
    G4int fDetectorElectrons;
    G4int fDetectorPositrons;
    G4double fDetectorEnergy;
    
    // Photon histogram storage
    static const int kMaxPhotons = 10000;
    G4double fDetectorPhotonEnergies[kMaxPhotons];
    G4double fDetectorPhotonAngles[kMaxPhotons];
    G4int fDetectorPhotonEnergyCount;
    
    // Neutron histogram storage
    static const int kMaxNeutrons = 1000;
    G4double fDetectorNeutronEnergies[kMaxNeutrons];
    G4double fDetectorNeutronAngles[kMaxNeutrons];
    G4int fDetectorNeutronCount;
    
    // Electron histogram storage
    static const int kMaxElectrons = 1000;
    G4double fDetectorElectronEnergies[kMaxElectrons];
    G4double fDetectorElectronAngles[kMaxElectrons];
    G4int fDetectorElectronCount;
    
    // Positron histogram storage
    static const int kMaxPositrons = 1000;
    G4double fDetectorPositronEnergies[kMaxPositrons];
    G4double fDetectorPositronAngles[kMaxPositrons];
    G4int fDetectorPositronCount;
};

#endif
