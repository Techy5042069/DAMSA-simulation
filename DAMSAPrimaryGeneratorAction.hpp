#ifndef DAMSAPrimaryGeneratorAction_h
#define DAMSAPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

/// Primary generator action class for DAMSA experiment
/// Shoots 8 GeV protons at the tungsten target
class DAMSAPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    DAMSAPrimaryGeneratorAction();
    virtual ~DAMSAPrimaryGeneratorAction();

    // Method from the base class
    virtual void GeneratePrimaries(G4Event*);

    // Method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

    // method to set beam energy for parametr studies
    void SetBeamEnergy(G4double energy);

    // Method to get current beam energy
    G4double GetBeamEnergy() const;


private:
    G4ParticleGun* fParticleGun; // Pointer to G4 particle gun
    G4double fCurrentEnergy; // Current beam energy
};

#endif
