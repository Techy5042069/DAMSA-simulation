#ifndef DAMSAAnalysis_h      //guards preventing files from being included twice during compilation
#define DAMSAAnalysis_h 1

#include "globals.hh"         // imports GEANT4 basic types and unit definitions
#include "G4SystemOfUnits.hh"

class DAMSAAnalysis
{
public:
    static DAMSAAnalysis* Instance();  // singleton pattern-only one analysis object for entire simulation so that all events write to same notebook
    
    void RecordParticle(const G4String& particleName, G4double energy, const G4String& location, G4double angle);  //function to record particle, particlename tells type of particle, energy gives kinetic energy, location gives target exit or detector enetrance
    void PrintSummary();   //prints all accumulated counts at th eend
    void Reset();          // clears all counters back to zero, when running multiple energy scans, resets btwn each energy point
    
private:
    DAMSAAnalysis();           // constructor
    ~DAMSAAnalysis();          // destructor
    static DAMSAAnalysis* fInstance;   // pointer to one instance, shared by all code, not per-object
    
    // Counters at target exit, integer counters for each particle type at target exit
    G4int fTargetExitNeutrons;
    G4int fTargetExitPhotons;
    G4int fTargetExitElectrons;
    G4int fTargetExitPositrons;
    G4double fTargetExitEnergy;  // accumulates total energy of all particles
    
    // Counters at detector entrance, same as before for detector entrance
    G4int fDetectorNeutrons;
    G4int fDetectorPhotons;
    G4int fDetectorElectrons;
    G4int fDetectorPositrons;
    G4double fDetectorEnergy;
    static const int kMaxPhotons = 10000; // maximum no of photons we can store
    G4double fDetectorPhotonEnergies[kMaxPhotons];  // array to store energy of each photon
    G4int fDetectorPhotonEnergyCount; //counter; used as array index
    G4double fDetectorPhotonAngles[kMaxPhotons];  // Store angle for each photon
};

#endif
