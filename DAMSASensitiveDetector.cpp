#include "DAMSASensitiveDetector.hpp"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

DAMSASensitiveDetector::DAMSASensitiveDetector(const G4String& name,
                                             const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHCID(-1),
   fTotalEnergyDeposit(0.),
   fNeutronHits(0),
   fPhotonHits(0),
   fElectronHits(0)
{
    collectionName.insert(hitsCollectionName);
}

DAMSASensitiveDetector::~DAMSASensitiveDetector()
{}

void DAMSASensitiveDetector::Initialize(G4HCofThisEvent*)
{
    // Reset counters for each event
    fTotalEnergyDeposit = 0.;
    fNeutronHits = 0;
    fPhotonHits = 0;
    fElectronHits = 0;
}

G4bool DAMSASensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // Get energy deposit
    G4double edep = aStep->GetTotalEnergyDeposit();
    
    if(edep == 0.) return false;
    
    fTotalEnergyDeposit += edep;
    
    // Count particle types
    G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
    
    if(particleName == "neutron") fNeutronHits++;
    else if(particleName == "gamma") fPhotonHits++;
    else if(particleName == "e-" || particleName == "e+") fElectronHits++;
    
    return true;
}

void DAMSASensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    if(fTotalEnergyDeposit > 0) {
        G4cout << "=== Event Summary ===" << G4endl;
        G4cout << "Total Energy Deposit: " << fTotalEnergyDeposit/MeV << " MeV" << G4endl;
        G4cout << "Neutron hits: " << fNeutronHits << G4endl;
        G4cout << "Photon hits: " << fPhotonHits << G4endl;
        G4cout << "Electron/Positron hits: " << fElectronHits << G4endl;
    }
}
