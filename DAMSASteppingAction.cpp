#include "DAMSASteppingAction.hpp" 
#include "DAMSAAnalysis.hpp"       //our analysis class to record particles
#include "G4Step.hh"               //information on current step
#include "G4Track.hh"              //particle trajectory
#include "G4VPhysicalVolume.hh"    //information on volume

DAMSASteppingAction::DAMSASteppingAction()
: G4UserSteppingAction()
{}

DAMSASteppingAction::~DAMSASteppingAction()
{}

void DAMSASteppingAction::UserSteppingAction(const G4Step* step)
{
    // OPTIMIZATION: Only check volume if particle just entered a new volume
    if(step->GetPostStepPoint()->GetStepStatus() != fGeomBoundary) {
        return;  // Not crossing boundary, skip expensive checks
    }
    
    G4VPhysicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    G4String volName = volume->GetName();
    
    // OPTIMIZATION: Single string comparison instead of two separate ifs
    if(volName != "ScoringVolume" && volName != "TargetExitVolume") {
        return;  // Not our volumes, skip
    }
    
    // Now we know we're in one of our scoring volumes
    G4Track* track = step->GetTrack();
    G4String particleName = track->GetDefinition()->GetParticleName();
    G4double energy = track->GetKineticEnergy();
    G4ThreeVector momentum = track->GetMomentumDirection();
    G4double angle = momentum.angle(G4ThreeVector(0,0,1));
    
    G4double globalTime = step->GetPostStepPoint()->GetGlobalTime(); // Geant4 ns
    if(volName == "ScoringVolume") {
        DAMSAAnalysis::Instance()->RecordParticle(particleName, energy, "DetectorEntrance", angle, globalTime);
        track->SetTrackStatus(fStopAndKill);
    }
    else if(volName == "TargetExitVolume") {
        DAMSAAnalysis::Instance()->RecordParticle(particleName, energy, "TargetExit", angle, globalTime);
        // Don't kill track - let it continue to detector
    }
}
