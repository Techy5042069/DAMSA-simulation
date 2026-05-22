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

    runManager->BeamOn(1);
    G4cout << "This is working\n";
    DAMSAAnalysis::Instance()->PrintTOFSummary();
    G4cout << "This is not working\n";

    delete runManager;
    return 0;
}
