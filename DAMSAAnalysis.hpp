#ifndef DAMSAAnalysis_h
#define DAMSAAnalysis_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

class DAMSAAnalysis
{
public:
    static DAMSAAnalysis* Instance();
    void RecordParticle(const G4String& particleName, G4double energy, const G4String& location, G4double angle, G4double tof = 0.0);
    void PrintSummary();
    void PrintTOFSummary() const;
    void Reset();

    G4int GetTargetExitPhotons() const { return fTargetExitPhotons; }
    G4int GetTargetExitNeutrons() const { return fTargetExitNeutrons; }
    G4int GetDetectorPhotons() const { return fDetectorPhotons; }
    G4int GetDetectorNeutrons() const { return fDetectorNeutrons; }
    G4int GetForwardPhotons() const;  // Count photons in 0-20 degrees
    
    // Energy and angle array access
    G4int GetDetectorPhotonCount() const { return fDetectorPhotonEnergyCount; }
    G4int GetDetectorNeutronCount() const { return fDetectorNeutronCount; }
    G4int GetDetectorElectronCount() const { return fDetectorElectronCount; }
    G4int GetDetectorPositronCount() const { return fDetectorPositronCount; }
    
    const G4double* GetDetectorPhotonEnergies() const { return fDetectorPhotonEnergies; }
    const G4double* GetDetectorPhotonAngles() const { return fDetectorPhotonAngles; }
    const G4double* GetDetectorNeutronEnergies() const { return fDetectorNeutronEnergies; }
    const G4double* GetDetectorNeutronAngles() const { return fDetectorNeutronAngles; }
    const G4double* GetDetectorElectronEnergies() const { return fDetectorElectronEnergies; }
    const G4double* GetDetectorElectronAngles() const { return fDetectorElectronAngles; }
    const G4double* GetDetectorPositronEnergies() const { return fDetectorPositronEnergies; }
    const G4double* GetDetectorPositronAngles() const { return fDetectorPositronAngles; }
    
    const G4double* GetDetectorPhotonTOF()  const { return fDetectorPhotonTOF; }
    const G4double* GetDetectorNeutronTOF() const { return fDetectorNeutronTOF; }
    
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
    G4double fDetectorPhotonTOF [kMaxPhotons];  
    G4int fDetectorPhotonEnergyCount;
    
    // Neutron histogram storage
    static const int kMaxNeutrons = 1000;
    G4double fDetectorNeutronEnergies[kMaxNeutrons];
    G4double fDetectorNeutronAngles[kMaxNeutrons];
    G4double fDetectorNeutronTOF [kMaxNeutrons];
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
