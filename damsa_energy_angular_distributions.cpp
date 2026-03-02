#include "DAMSADetectorConstruction.hpp"
#include "DAMSAPrimaryGeneratorAction.hpp"
#include "DAMSASteppingAction.hpp"
#include "DAMSAAnalysis.hpp"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "G4SystemOfUnits.hh"
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>

int main()
{
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== DAMSA Energy & Angular Distributions ===" << G4endl;
    G4cout << "=== Generates ROOT histograms ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    
    const G4int nEvents = 1000;
    
    // Setup run manager
    G4RunManager* runManager = new G4RunManager;
    runManager->SetUserInitialization(new DAMSADetectorConstruction());
    
    G4VModularPhysicsList* physicsList = new QBBC;
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);
    
    runManager->SetUserAction(new DAMSAPrimaryGeneratorAction());
    runManager->SetUserAction(new DAMSASteppingAction());
    runManager->Initialize();
    
    DAMSAAnalysis::Instance()->Reset();
    
    G4cout << "\nRunning " << nEvents << " events..." << G4endl;
    runManager->BeamOn(nEvents);
    
    G4cout << "\n=============================================" << G4endl;
    G4cout << "=== Creating ROOT Histograms ===" << G4endl;
    G4cout << "=============================================" << G4endl;
    
    // Create ROOT file
    TFile* rootFile = new TFile("damsa_distributions.root", "RECREATE");
    
    // Get data from analysis
    DAMSAAnalysis* analysis = DAMSAAnalysis::Instance();
    
    // Create histograms - Target Exit
    TH1D* h_photon_energy_exit = new TH1D("h_photon_energy_exit", 
        "Photon Energy at Target Exit;Energy (GeV);Count", 100, 0, 8);
    TH1D* h_photon_angle_exit = new TH1D("h_photon_angle_exit",
        "Photon Angle at Target Exit;Angle (degrees);Count", 90, 0, 90);
    TH1D* h_neutron_energy_exit = new TH1D("h_neutron_energy_exit",
        "Neutron Energy at Target Exit;Energy (GeV);Count", 100, 0, 8);
    TH1D* h_neutron_angle_exit = new TH1D("h_neutron_angle_exit",
        "Neutron Angle at Target Exit;Angle (degrees);Count", 90, 0, 90);
    
    // Create histograms - Detector Entrance
    TH1D* h_photon_energy_det = new TH1D("h_photon_energy_det",
        "Photon Energy at Detector Entrance;Energy (GeV);Count", 100, 0, 8);
    TH1D* h_photon_angle_det = new TH1D("h_photon_angle_det",
        "Photon Angle at Detector Entrance;Angle (degrees);Count", 90, 0, 90);
    TH1D* h_neutron_energy_det = new TH1D("h_neutron_energy_det",
        "Neutron Energy at Detector Entrance;Energy (GeV);Count", 100, 0, 8);
    TH1D* h_neutron_angle_det = new TH1D("h_neutron_angle_det",
        "Neutron Angle at Detector Entrance;Angle (degrees);Count", 90, 0, 90);
    TH1D* h_electron_energy_det = new TH1D("h_electron_energy_det",
        "Electron Energy at Detector Entrance;Energy (GeV);Count", 100, 0, 8);
    TH1D* h_electron_angle_det = new TH1D("h_electron_angle_det",
        "Electron Angle at Detector Entrance;Angle (degrees);Count", 90, 0, 90);
    TH1D* h_positron_energy_det = new TH1D("h_positron_energy_det",
        "Positron Energy at Detector Entrance;Energy (GeV);Count", 100, 0, 8);
    TH1D* h_positron_angle_det = new TH1D("h_positron_angle_det",
        "Positron Angle at Detector Entrance;Angle (degrees);Count", 90, 0, 90);


   // Fill histograms from analysis data
    G4cout << "Filling histograms..." << G4endl;
    
    // Fill photon histograms (detector entrance)
    G4int nPhotons = analysis->GetDetectorPhotonCount();
    const G4double* photonEnergies = analysis->GetDetectorPhotonEnergies();
    const G4double* photonAngles = analysis->GetDetectorPhotonAngles();
    
    for(G4int i = 0; i < nPhotons; i++) {
        h_photon_energy_det->Fill(photonEnergies[i] / GeV);
        h_photon_angle_det->Fill(photonAngles[i] * 180.0 / 3.14159265);
    }
    
    // Fill neutron histograms (detector entrance)
    G4int nNeutrons = analysis->GetDetectorNeutronCount();
    const G4double* neutronEnergies = analysis->GetDetectorNeutronEnergies();
    const G4double* neutronAngles = analysis->GetDetectorNeutronAngles();
    
    for(G4int i = 0; i < nNeutrons; i++) {
        h_neutron_energy_det->Fill(neutronEnergies[i] / GeV);
        h_neutron_angle_det->Fill(neutronAngles[i] * 180.0 / 3.14159265);
    }
    
    // Fill electron histograms (detector entrance)
    G4int nElectrons = analysis->GetDetectorElectronCount();
    const G4double* electronEnergies = analysis->GetDetectorElectronEnergies();
    const G4double* electronAngles = analysis->GetDetectorElectronAngles();
    
    for(G4int i = 0; i < nElectrons; i++) {
        h_electron_energy_det->Fill(electronEnergies[i] / GeV);
        h_electron_angle_det->Fill(electronAngles[i] * 180.0 / 3.14159265);
    }
    
    // Fill positron histograms (detector entrance)
    G4int nPositrons = analysis->GetDetectorPositronCount();
    const G4double* positronEnergies = analysis->GetDetectorPositronEnergies();
    const G4double* positronAngles = analysis->GetDetectorPositronAngles();
    
    for(G4int i = 0; i < nPositrons; i++) {
        h_positron_energy_det->Fill(positronEnergies[i] / GeV);
        h_positron_angle_det->Fill(positronAngles[i] * 180.0 / 3.14159265);
    }
    
    G4cout << "Filled histograms with:" << G4endl;
    G4cout << "  Photons: " << nPhotons << G4endl;
    G4cout << "  Neutrons: " << nNeutrons << G4endl;
    G4cout << "  Electrons: " << nElectrons << G4endl;
    G4cout << "  Positrons: " << nPositrons << G4endl;    
   
 
    // Write and close
    rootFile->Write();
    rootFile->Close();
    delete rootFile;
    
    // Print summary
    analysis->PrintSummary();
    
    delete runManager;
    return 0;
}
