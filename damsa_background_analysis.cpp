#include "DAMSADetectorConstruction.hpp"
#include "DAMSAPrimaryGeneratorAction.hpp"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "DAMSASteppingAction.hpp"
#include "DAMSAAnalysis.hpp"
#include "G4SystemOfUnits.hh"
#include <fstream>

int main()
{
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== DAMSA Background Characterization ===" << G4endl;
    G4cout << "=== Complete Analysis: Counts, Energy, Angles ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    
    // Number of events to run
    const G4int nEvents = 1000;  // Adjust for better statistics
    
    G4RunManager* runManager = new G4RunManager;
    
    runManager->SetUserInitialization(new DAMSADetectorConstruction());
    
    G4VModularPhysicsList* physicsList = new QBBC;
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);
    
    runManager->SetUserAction(new DAMSAPrimaryGeneratorAction());
    runManager->SetUserAction(new DAMSASteppingAction());
    
    runManager->Initialize();
    
    DAMSAAnalysis::Instance()->Reset();
    
    G4cout << "\nRunning " << nEvents << " events for background analysis..." << G4endl;
    G4cout << "This may take several minutes..." << G4endl;
    
    runManager->BeamOn(nEvents);
    
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== BACKGROUND ANALYSIS COMPLETE ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    
    // Print complete summary
    DAMSAAnalysis::Instance()->PrintSummary();
    
    // Calculate and print acceptance ratios
    G4int exitPhotons = DAMSAAnalysis::Instance()->GetTargetExitPhotons();
    G4int detPhotons = DAMSAAnalysis::Instance()->GetDetectorPhotons();
    G4int exitNeutrons = DAMSAAnalysis::Instance()->GetTargetExitNeutrons();
    G4int detNeutrons = DAMSAAnalysis::Instance()->GetDetectorNeutrons();
    G4int forwardPhotons = DAMSAAnalysis::Instance()->GetForwardPhotons();
    
    G4double photonAcceptance = (exitPhotons > 0) ? 100.0 * detPhotons / exitPhotons : 0.0;
    G4double neutronAcceptance = (exitNeutrons > 0) ? 100.0 * detNeutrons / exitNeutrons : 0.0;
    G4double forwardFraction = (detPhotons > 0) ? 100.0 * forwardPhotons / detPhotons : 0.0;
    
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== ACCEPTANCE SUMMARY ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    G4cout << "Photon acceptance: " << photonAcceptance << "%" << G4endl;
    G4cout << "Neutron acceptance: " << neutronAcceptance << "%" << G4endl;
    G4cout << "Forward photons (0-20°): " << forwardFraction << "% of detected" << G4endl;
    G4cout << "=============================================" << G4endl;
    
    // Save summary to file
    std::ofstream outFile("background_analysis_summary.txt");
    outFile << "DAMSA Background Analysis Summary\n";
    outFile << "Events: " << nEvents << "\n";
    outFile << "Beam: 8 GeV electrons\n";
    outFile << "Gap: 47 cm (current baseline)\n\n";
    
    outFile << "TARGET EXIT:\n";
    outFile << "  Photons: " << exitPhotons << "\n";
    outFile << "  Neutrons: " << exitNeutrons << "\n\n";
    
    outFile << "DETECTOR ENTRANCE:\n";
    outFile << "  Photons: " << detPhotons << " (" << photonAcceptance << "% acceptance)\n";
    outFile << "  Neutrons: " << detNeutrons << " (" << neutronAcceptance << "% acceptance)\n";
    outFile << "  Forward photons: " << forwardPhotons << " (" << forwardFraction << "%)\n";
    
    outFile.close();
    
    G4cout << "\nResults saved to: background_analysis_summary.txt\n" << G4endl;
    
    delete runManager;
    return 0;
}
