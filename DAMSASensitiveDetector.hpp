#ifndef DAMSASensitiveDetector_h
#define DAMSASensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "globals.hh"

class G4Step;
class G4HCofThisEvent;

class DAMSASensitiveDetector : public G4VSensitiveDetector
{
public:
    DAMSASensitiveDetector(const G4String& name, 
                          const G4String& hitsCollectionName);
    virtual ~DAMSASensitiveDetector();

    // Methods from base class
    virtual void Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

private:
    G4int fHCID;
    G4double fTotalEnergyDeposit;
    G4int fNeutronHits;
    G4int fPhotonHits;
    G4int fElectronHits;
};

#endif
