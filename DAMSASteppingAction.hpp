#ifndef DAMSASteppingAction_h
#define DAMSASteppingAction_h 1

#include "G4UserSteppingAction.hh"  \\imports GEANT4's base class for stepping actions

class DAMSASteppingAction : public G4UserSteppingAction      \\creates custom stepping action by inheriting from GEANT4's base
{
public:
    DAMSASteppingAction();
    virtual ~DAMSASteppingAction();
    
    virtual void UserSteppingAction(const G4Step* step);   \\pointer called by GEANT4 at every step to record position, particle energy, volume, etc..
};

#endif
