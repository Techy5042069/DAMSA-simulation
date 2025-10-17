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
        
        DAMSAAnalysis::Instance()->RecordParticle(particleName, energy, "DetectorEntrance");   //record particle in our analysis notebook
        
        track->SetTrackStatus(fStopAndKill);   //kills the track; stops simulating
    }
    else if(volume->GetName() == "TargetExitVolume") {   //same logic for target exit
        G4Track* track = step->GetTrack();
        G4String particleName = track->GetDefinition()->GetParticleName();
        G4double energy = track->GetKineticEnergy();
        
        DAMSAAnalysis::Instance()->RecordParticle(particleName, energy, "TargetExit");
        
        // Don't kill track at the target exit - let it continue to detector
   }
}
