#include "DAMSADetectorConstruction.hpp"
#include "DAMSAPrimaryGeneratorAction.hpp"
#include "DAMSASteppingAction.hpp"
#include "DAMSAAnalysis.hpp"

#include "G4RunManager.hh"
#include "QBBC.hh"
#include "G4SystemOfUnits.hh"

int main()
{
    G4RunManager* runManager = new G4RunManager;

    runManager->SetUserInitialization(new DAMSADetectorConstruction());

    G4VModularPhysicsList* physicsList = new QBBC;
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserAction(new DAMSAPrimaryGeneratorAction());
    runManager->SetUserAction(new DAMSASteppingAction());

    runManager->Initialize();

    DAMSAAnalysis::Instance()->Reset();

    runManager->BeamOn(1000);
    std::cerr << "\n=== SIMULATION COMPLETE ===\n" << std::endl;
    DAMSAAnalysis::Instance()->PrintTOFSummary();
    std::cerr << "\n=== DONE ===\n" << std::endl;

    delete runManager;
    return 0;
}
