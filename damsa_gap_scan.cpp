#include "DAMSADetectorConstruction.hpp"
#include "DAMSAPrimaryGeneratorAction.hpp"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "DAMSASteppingAction.hpp"
#include "DAMSAAnalysis.hpp"
#include "G4SystemOfUnits.hh"
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <iostream>

int main(int argc, char** argv)
{
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== DAMSA Optimization Scan ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    
    // Select scan type
    G4cout << "\nSelect scan type:" << G4endl;
    G4cout << "1. Gap distance only (baseline target)" << G4endl;
    G4cout << "2. Target length only (baseline gap)" << G4endl;
    G4cout << "3. Target transverse size only (baseline gap)" << G4endl;
    G4cout << "4. Full optimization (gap + target geometry)" << G4endl;
    G4cout << "Enter choice (1-4): ";
    
    int choice;
    if (argc > 1) {
        choice = std::atoi(argv[1]);
    } else {
        std::cin >> choice;
    }
    
    // Configuration vectors
    std::vector<G4double> gapDistances = {20, 30, 40, 47, 50, 60, 70, 80, 90, 100};
    std::vector<G4double> targetLengths = {5, 10, 15, 20};
    std::vector<G4double> targetTransverse = {5, 7.5, 10};
    
    // Baseline values
    G4double baseGap = 47.0;
    G4double baseLength = 10.0;
    G4double baseWidth = 5.0;
    
    // Target parameters
    const G4double targetExitZ = -75.0;
    const G4double csiHalfLength = 22.0;
    const G4int eventsPerConfig = 1000;
    
    // Determine what to scan
    std::vector<G4double> gapScan, lengthScan, widthScan;
    
    switch(choice) {
        case 1:
            gapScan = gapDistances;
            lengthScan = {baseLength};
            widthScan = {baseWidth};
            break;
        case 2:
            gapScan = {baseGap};
            lengthScan = targetLengths;
            widthScan = {baseWidth};
            break;
        case 3:
            gapScan = {baseGap};
            lengthScan = {baseLength};
            widthScan = targetTransverse;
            break;
        case 4:
            gapScan = gapDistances;
            lengthScan = targetLengths;
            widthScan = targetTransverse;
            G4cout << "\n Full scan will run " 
                   << (gapScan.size() * lengthScan.size() * widthScan.size())
                   << " configurations!" << G4endl;
            G4cout << "Estimated time: ~" 
                   << (gapScan.size() * lengthScan.size() * widthScan.size() * eventsPerConfig * 0.1 / 3600.0) 
                   << " hours" << G4endl;
            G4cout << "Continue? (y/n): ";
            char confirm;
            std::cin >> confirm;
            if(confirm != 'y' && confirm != 'Y') {
                G4cout << "Scan cancelled." << G4endl;
                return 0;
            }
            break;
        default:
            G4cout << "Invalid choice!" << G4endl;
            return 1;
    }
    
    // Create output file
    std::time_t now = std::time(nullptr);
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", std::localtime(&now));
    
    std::string filename;
    switch(choice) {
        case 1: filename = "gap_scan_"; break;
        case 2: filename = "target_length_scan_"; break;
        case 3: filename = "target_transverse_scan_"; break;
        case 4: filename = "full_optimization_"; break;
    }
    filename += timestamp;
    filename += ".txt";
    
    std::ofstream outFile(filename);
    outFile << "# DAMSA Optimization Scan Results\n";
    outFile << "# Scan type: " << choice << "\n";
    outFile << "# Events per configuration: " << eventsPerConfig << "\n#\n";
    outFile << std::setw(8) << "Config" << "  ";
    outFile << std::setw(8) << "Gap_cm" << "  ";
    outFile << std::setw(10) << "TgtLen_cm" << "  ";
    outFile << std::setw(10) << "TgtWid_cm" << "  ";
    outFile << std::setw(10) << "CsI_Z_cm" << "  ";
    outFile << std::setw(12) << "ExitPhotons" << "  ";
    outFile << std::setw(12) << "ExitNeutrons" << "  ";
    outFile << std::setw(12) << "DetPhotons" << "  ";
    outFile << std::setw(12) << "DetNeutrons" << "  ";
    outFile << std::setw(12) << "FwdPhotons" << "  ";
    outFile << std::setw(10) << "PhotonAcc%" << "  ";
    outFile << std::setw(8) << "S/B" << "\n";
    
    // Create Run Manager ONCE
    G4RunManager* runManager = new G4RunManager;
    
    // Set up physics list ONCE
    G4VModularPhysicsList* physicsList = new QBBC;
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);
    
    // Set up actions ONCE
    runManager->SetUserAction(new DAMSAPrimaryGeneratorAction());
    runManager->SetUserAction(new DAMSASteppingAction());
    
    // Main scan loop
    int configNum = 0;
    int totalConfigs = gapScan.size() * lengthScan.size() * widthScan.size();
    
    G4cout << "\nTotal configurations: " << totalConfigs << "\n" << G4endl;
    
    for (G4double gap : gapScan) {
        for (G4double length : lengthScan) {
            for (G4double width : widthScan) {
                
                configNum++;
                
                G4double csiCenterZ = (targetExitZ + gap + csiHalfLength) * cm;
                
                G4cout << "\n--- Configuration " << configNum << "/" << totalConfigs << " ---" << G4endl;
                G4cout << "Gap: " << gap << " cm, Target: " << width << "×" << width << "×" << length << " cm³" << G4endl;
                
                // Create detector with specific parameters
                DAMSADetectorConstruction* detector = new DAMSADetectorConstruction();
                detector->SetCsIPositionZ(csiCenterZ);
                detector->SetTargetDimensions(width*cm, width*cm, length*cm);
                
                // Set detector construction
                runManager->SetUserInitialization(detector);
                
                // Initialize or reinitialize
                if(configNum == 1) {
                    runManager->Initialize();
                } else {
                    runManager->ReinitializeGeometry(true);
                }
                
                // Reset analysis
                DAMSAAnalysis::Instance()->Reset();
                
                // Run events
                runManager->BeamOn(eventsPerConfig);
                
                // Get results
                G4int exitPhotons = DAMSAAnalysis::Instance()->GetTargetExitPhotons();
                G4int exitNeutrons = DAMSAAnalysis::Instance()->GetTargetExitNeutrons();
                G4int detPhotons = DAMSAAnalysis::Instance()->GetDetectorPhotons();
                G4int detNeutrons = DAMSAAnalysis::Instance()->GetDetectorNeutrons();
                G4int fwdPhotons = DAMSAAnalysis::Instance()->GetForwardPhotons();
                
                // Calculate metrics
                G4double photonAcc = (exitPhotons > 0) ? 100.0 * detPhotons / exitPhotons : 0.0;
                G4cout << "Results: " << detPhotons << " photons, " << detNeutrons << " neutrons" << G4endl;
                
                
                
                // Write to file
                outFile << std::setw(8) << configNum << "  ";
                outFile << std::setw(8) << gap << "  ";
                outFile << std::setw(10) << length << "  ";
                outFile << std::setw(10) << width << "  ";
                outFile << std::setw(10) << (csiCenterZ/cm) << "  ";
                outFile << std::setw(12) << exitPhotons << "  ";
                outFile << std::setw(12) << exitNeutrons << "  ";
                outFile << std::setw(12) << detPhotons << "  ";
                outFile << std::setw(12) << detNeutrons << "  ";
                outFile << std::setw(12) << fwdPhotons << "  ";
                outFile << std::setw(10) << std::fixed << std::setprecision(2) << photonAcc << "  ";
                outFile << "\n";
                outFile.flush();
            }
        }
    }
    
    outFile.close();
    
    G4cout << "\n=============================================" << G4endl;
    G4cout << "Scan complete! Results: " << filename << G4endl;
    G4cout << "=============================================" << G4endl;
    
    delete runManager;
    return 0;
}
