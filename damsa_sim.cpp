#include "DAMSADetectorConstruction.hpp"
#include "DAMSAPrimaryGeneratorAction.hpp"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "QBBC.hh"
#include "DAMSASteppingAction.hpp"
#include "DAMSAAnalysis.hpp"

int main()
{

    G4RunManager* runManager = new G4RunManager; // RunManager controls everything initialization, running events, cleanup
    runManager->SetUserInitialization(new DAMSADetectorConstruction());  // new creates an instance of detector class

    G4VModularPhysicsList* physicsList = new QBBC;  // QBBC is pre built list of physics processes Quark Based Bertini Cascade 
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserAction(new DAMSAPrimaryGeneratorAction());// asking manager to use our beam design
    runManager->SetUserAction(new DAMSASteppingAction());   //telling GEANT4 to use our stepping action

    G4VisExecutive* visManager = new G4VisExecutive; 
    visManager->Initialize();                   // builds everything, constructs geometry, initializes physics, sets up tracking

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    runManager->Initialize();

    G4cout <<  "DAMSA Track Visualization" << G4endl;
    G4cout << "Primary Beam: 8 GeV protons" << G4endl;
    G4cout << "Target: Tungsten" << G4endl;
    G4cout << " Physics: electromagnetic + hadronic" << G4endl;
    UImanager->ApplyCommand("/vis/open VRML2FILE");
    UImanager->ApplyCommand("/vis/drawVolume");
    UImanager->ApplyCommand("/vis/scene/create");
    UImanager->ApplyCommand("/vis/scene/add/volume");
    UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
    UImanager->ApplyCommand("/vis/scene/add/hits");
    UImanager->ApplyCommand("/vis/sceneHandler/attach");

    UImanager->ApplyCommand("/vis/modeling/trajectories/create/drawByParticleID");
    UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID-0/set e- red");
    UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID-0/set gamma green");
    UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID-0/set e+ cyan");
    UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID-0/set neutron yellow");

    G4cout << "Running 100 events with track visualization..." << G4endl;
    UImanager->ApplyCommand("/tracking/storeTrajectory 2");
    UImanager->ApplyCommand("/run/beamOn 100");

    UImanager->ApplyCommand("/vis/viewer/flush");

    G4cout << "- Red tracks: Electrons (primary + secondary)" << G4endl;
    G4cout << "- Green tracks: Photons (Bremsstrahlung)" << G4endl;
    G4cout << "- Yellow tracks: Neutrons (background)" << G4endl;
    G4cout << "- Cyan tracks: Positrons" << G4endl;

   //Print particle counting results
    DAMSAAnalysis:: Instance()->PrintSummary();     // after all events finish, print accumulated counts
    
    delete visManager;
    delete runManager;
    return 0;
}
