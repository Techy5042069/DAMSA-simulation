#include "DAMSADetectorConstruction.hpp"
#include "DAMSAPrimaryGeneratorAction.hpp"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "QBBC.hh"
#include "DAMSASteppingAction.hpp"
#include "DAMSAAnalysis.hpp"

int main(int argc, char** argv)
{
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== DAMSA Visualization Mode ===" << G4endl;
    G4cout << "=== Generates VRML files for 3D viewing ===" << G4endl;
    G4cout << "=============================================" << G4endl;

    G4RunManager* runManager = new G4RunManager;  //RunManager = the boss that coordinates everything. Controls initialization, running events, cleanup
    
    runManager->SetUserInitialization(new DAMSADetectorConstruction());
    
    G4VModularPhysicsList* physicsList = new QBBC; //Quark-Based Bertini Cascade, pre-built list of physics processes
//Includes: electromagnetic interactions, nuclear reactions, particle decays 
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);
    
    runManager->SetUserAction(new DAMSAPrimaryGeneratorAction()); //Tells RunManager to use my particle beam design
    runManager->SetUserAction(new DAMSASteppingAction());
    
    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    
    runManager->Initialize();  // construct geometry, initialize physics, set up tracking, must be initialized before running events
    
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    // Run visualization macro
    G4cout << "\nGenerating VRML visualization files..." << G4endl;
    UImanager->ApplyCommand("/control/execute ../vis.mac");
    
    G4cout << "\n=============================================" << G4endl;
    G4cout << "Visualization files generated:" << G4endl;
    G4cout << "  g4_00.wrl - Detector geometry only" << G4endl;
    G4cout << "  g4_01.wrl - Event 1 with particle tracks" << G4endl;
    G4cout << "  g4_02.wrl - Event 2 with particle tracks" << G4endl;
    G4cout << "  g4_03.wrl - Event 3 with particle tracks" << G4endl;
    G4cout << "  g4_04.wrl - Event 4 with particle tracks" << G4endl;
    G4cout << "  g4_05.wrl - Event 5 with particle tracks" << G4endl;
    G4cout << "\nView files at: https://www.viewstl.com/" << G4endl;
    G4cout << "Or use: view3dscene <filename>.wrl" << G4endl;
    G4cout << "=============================================" << G4endl;
    
    delete visManager;
    delete runManager;
    
    return 0;
}
