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

void DAMSASteppingAction::UserSteppingAction(const G4Step* step)  //get the volume where partilce currently is
{
    G4VPhysicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    //GetPreStepPoint tells start of the step, GetTouchableHandle tells handling the geometry at this point and GetVolume tells about physical volume at this location

    if(volume->GetName() == "ScoringVolume") {   //check if particle entered the detector entrance scoring volume
        G4Track* track = step->GetTrack();                                    //get particle type
        G4String particleName = track->GetDefinition()->GetParticleName();    //get name as string
        G4double energy = track->GetKineticEnergy();                          //get current kinetic energy

        // Get momentum direction and calculate angle relative to beam axis
        G4ThreeVector momentum = track->GetMomentumDirection();
        G4double angle = momentum.angle(G4ThreeVector(0,0,1));  // angle in radians
        
        DAMSAAnalysis::Instance()->RecordParticle(particleName, energy, "DetectorEntrance", angle);   //record particle in our analysis notebook
        
        track->SetTrackStatus(fStopAndKill);   //kills the track; stops simulating
    }
    else if(volume->GetName() == "TargetExitVolume") {   //same logic for target exit
        G4Track* track = step->GetTrack();
        G4String particleName = track->GetDefinition()->GetParticleName();
        G4double energy = track->GetKineticEnergy();

        G4ThreeVector momentum = track->GetMomentumDirection();
        G4double angle = momentum.angle(G4ThreeVector(0,0,1));
        
        DAMSAAnalysis::Instance()->RecordParticle(particleName, energy, "TargetExit", angle);
        
        // Don't kill track at the target exit - let it continue to detector
   }
}
