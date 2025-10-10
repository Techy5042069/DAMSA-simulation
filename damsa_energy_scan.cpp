#include "DAMSADetectorConstruction.hpp"
#include "DAMSAPrimaryGeneratorAction.hpp"
#include "DAMSASensitiveDetector.hpp"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "QBBC.hh"
#include "G4SystemOfUnits.hh"

int main()
{
    // Construct the run manager
    G4RunManager* runManager = new G4RunManager;
    
    // Set mandatory initialization classes
    runManager->SetUserInitialization(new DAMSADetectorConstruction());
    
    // Physics list
    G4VModularPhysicsList* physicsList = new QBBC;
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);
    
    // Set user action classes
    DAMSAPrimaryGeneratorAction* primaryGenerator = new DAMSAPrimaryGeneratorAction();
    runManager->SetUserAction(primaryGenerator);
    
    // Initialize G4 kernel
    runManager->Initialize();
    
    G4cout << "\n=== DAMSA Energy-Dependent Background Study ===" << G4endl;
    G4cout << "Scanning beam energies from 1 MeV to 1 GeV" << G4endl;
    G4cout << "Measuring backgrounds vs beam energy\n" << G4endl;
    
    // Energy scan: 1 MeV to 1 GeV
    G4double energies[] = {1*MeV, 10*MeV, 100*MeV, 500*MeV, 800*MeV, 1*GeV};
    G4int nEnergies = 6;
    G4int eventsPerEnergy = 20;
    
    for(G4int i = 0; i < nEnergies; i++) {
        G4cout << "\n--- Energy Point " << i+1 << "/" << nEnergies << " ---" << G4endl;
        G4cout << "Beam Energy: " << energies[i]/MeV << " MeV" << G4endl;
        
        // Set beam energy
        primaryGenerator->SetBeamEnergy(energies[i]);
        
        // Run events
        G4String command = "/run/beamOn " + std::to_string(eventsPerEnergy);
        G4UImanager::GetUIpointer()->ApplyCommand(command);
        
        G4cout << "Completed " << eventsPerEnergy << " events at " 
               << energies[i]/MeV << " MeV" << G4endl;
    }
    
    G4cout << "\n=== Energy Scan Complete ===" << G4endl;
    G4cout << "Results show energy-dependent backgrounds for DAMSA optimization" << G4endl;
    
    delete runManager;
    return 0;
}
