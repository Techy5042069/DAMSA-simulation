#include "DAMSADetectorConstruction.hpp"
#include "DAMSAPrimaryGeneratorAction.hpp"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "DAMSASteppingAction.hpp"
#include "DAMSAAnalysis.hpp"
#include <fstream>

int main()
{
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== DAMSA Gap Distance Optimization Scan ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    G4cout << "\nThis program will run simulations at different" << G4endl;
    G4cout << "target-detector gap distances to find optimal spacing.\n" << G4endl;
    G4cout << "Each gap requires rebuilding geometry - running manually." << G4endl;
    G4cout << "\nTo use: Modify CsI position in DAMSADetectorConstruction.cpp" << G4endl;
    G4cout << "and run damsa_sim for each gap distance.\n" << G4endl;
    G4cout << "Gap formula: CsI_center_Z = -75 + gap + 22\n" << G4endl;
    
    G4cout << "Recommended gap values to test:" << G4endl;
    G4cout << "Gap 20cm: CsI at Z = -33cm" << G4endl;
    G4cout << "Gap 30cm: CsI at Z = -23cm" << G4endl;
    G4cout << "Gap 40cm: CsI at Z = -13cm" << G4endl;
    G4cout << "Gap 47cm: CsI at Z = -6cm  (CURRENT)" << G4endl;
    G4cout << "Gap 50cm: CsI at Z = -3cm" << G4endl;
    G4cout << "Gap 60cm: CsI at Z = +7cm" << G4endl;
    G4cout << "Gap 70cm: CsI at Z = +17cm" << G4endl;
    G4cout << "Gap 80cm: CsI at Z = +27cm" << G4endl;
    G4cout << "Gap 90cm: CsI at Z = +37cm" << G4endl;
    G4cout << "Gap 100cm: CsI at Z = +47cm" << G4endl;
    
    G4cout << "\n=============================================" << G4endl;
    
    return 0;
}
