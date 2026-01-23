#include "DAMSADetectorConstruction.hpp"
#include "DAMSAPrimaryGeneratorAction.hpp"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "DAMSASteppingAction.hpp"
#include "DAMSAAnalysis.hpp"
#include <fstream>
#include <vector>
#include <iomanip>

int main()
{
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== DAMSA Automated Gap Distance Scan ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    
    // Define gap distances to scan (in cm)
    std::vector<G4double> gapDistances = {20, 30, 40, 47, 50, 60, 70, 80, 90, 100};
    
    // Target parameters (target exit at Z = -75 cm, CsI half-length = 22 cm)
    const G4double targetExitZ = -75.0;
    const G4double csiHalfLength = 22.0;
    
    // Number of events per gap configuration
    const G4int eventsPerGap = 1000;
    
    // Open output file for results
    std::ofstream outFile("gap_scan_results.txt");
    outFile << "# DAMSA Gap Distance Optimization Scan Results\n";
    outFile << "# Gap[cm]  CsI_Z[cm]  TargetExitPhotons  TargetExitNeutrons  ";
    outFile << "DetectorPhotons  DetectorNeutrons  ForwardPhotons  Efficiency[%]\n";
    
    // Create Run Manager (only once)
    G4RunManager* runManager = new G4RunManager;
    
    // Set up physics list (only once)
    G4VModularPhysicsList* physicsList = new QBBC;
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);
    
    // Set up primary generator and stepping action (only once)
    runManager->SetUserAction(new DAMSAPrimaryGeneratorAction());
    runManager->SetUserAction(new DAMSASteppingAction());
    
    // Get UI manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    // Loop over all gap distances
    for(size_t i = 0; i < gapDistances.size(); i++) {
        G4double gap = gapDistances[i] * CLHEP::cm;
        
        // Calculate CsI center position: target_exit + gap + csi_half_length
        G4double csiCenterZ = (targetExitZ + gapDistances[i] + csiHalfLength) * CLHEP::cm;
        
        G4cout << "\n================================================" << G4endl;
        G4cout << "=== Configuration " << (i+1) << "/" << gapDistances.size() << ": Gap = " 
               << gapDistances[i] << " cm ===" << G4endl;
        G4cout << "=== CsI Center Z = " << (csiCenterZ/CLHEP::cm) << " cm ===" << G4endl;
        G4cout << "================================================" << G4endl;
        
        // Create detector construction with specific CsI position
        DAMSADetectorConstruction* detector = new DAMSADetectorConstruction();
        detector->SetCsIPositionZ(csiCenterZ);
        
        // Set detector construction
        runManager->SetUserInitialization(detector);
        
        // Initialize (or reinitialize) the run manager
        if(i == 0) {
            runManager->Initialize();
        } else {
            // For subsequent iterations, reinitialize geometry
            runManager->ReinitializeGeometry(true);  // true = destroy old geometry
        }
        
        // Reset analysis counters
        DAMSAAnalysis::Instance()->Reset();
        
        // Run the simulation
        G4cout << "Running " << eventsPerGap << " events..." << G4endl;
        runManager->BeamOn(eventsPerGap);
        
        // Get results
        G4int targetExitPhotons = DAMSAAnalysis::Instance()->GetTargetExitPhotons();
        G4int targetExitNeutrons = DAMSAAnalysis::Instance()->GetTargetExitNeutrons();
        G4int detectorPhotons = DAMSAAnalysis::Instance()->GetDetectorPhotons();
        G4int detectorNeutrons = DAMSAAnalysis::Instance()->GetDetectorNeutrons();
        G4int forwardPhotons = DAMSAAnalysis::Instance()->GetForwardPhotons();
        
        // Calculate efficiency (photons reaching detector / photons exiting target)
        G4double efficiency = 0.0;
        if(targetExitPhotons > 0) {
            efficiency = 100.0 * detectorPhotons / targetExitPhotons;
        }
        
        // Print summary
        G4cout << "\n--- Results for Gap = " << gapDistances[i] << " cm ---" << G4endl;
        G4cout << "Target Exit Photons:   " << targetExitPhotons << G4endl;
        G4cout << "Target Exit Neutrons:  " << targetExitNeutrons << G4endl;
        G4cout << "Detector Photons:      " << detectorPhotons << G4endl;
        G4cout << "Detector Neutrons:     " << detectorNeutrons << G4endl;
        G4cout << "Forward Photons (0-20°): " << forwardPhotons << G4endl;
        G4cout << "Photon Efficiency:     " << std::fixed << std::setprecision(2) 
               << efficiency << "%" << G4endl;
        
        // Write to file
        outFile << std::setw(8) << gapDistances[i] << "  "
                << std::setw(10) << (csiCenterZ/CLHEP::cm) << "  "
                << std::setw(18) << targetExitPhotons << "  "
                << std::setw(20) << targetExitNeutrons << "  "
                << std::setw(16) << detectorPhotons << "  "
                << std::setw(17) << detectorNeutrons << "  "
                << std::setw(14) << forwardPhotons << "  "
                << std::setw(13) << std::fixed << std::setprecision(2) << efficiency << "\n";
        outFile.flush();
        
        // Print detailed spectrum for this configuration
        DAMSAAnalysis::Instance()->PrintSummary();
    }
    
    outFile.close();
    
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== Gap Scan Complete ===" << G4endl;
    G4cout << "=== Results saved to: gap_scan_results.txt ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    
    delete runManager;
    return 0;
}
