#include "DAMSAPrimaryGeneratorAction.hpp"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

DAMSAPrimaryGeneratorAction::DAMSAPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), fCurrentEnergy(8.*GeV)
{
    G4int n_particle = 1;   // 1 means shooting one particle per event
    fParticleGun = new G4ParticleGun(n_particle);  // simple beam, all particles identical

    // Default particle kinematic - 8 GeV electrons for DAMSA, beams from SLAC
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e-");
    
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.)); // shoots in +Z direction
    fParticleGun->SetParticleEnergy(8.*GeV);  // DAMSA beam energy
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,-90.*cm)); // 10cm before hitting the target
}

DAMSAPrimaryGeneratorAction::~DAMSAPrimaryGeneratorAction()
{
    delete fParticleGun;
}

void DAMSAPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // This function is called at the beginning of each event
    
    // Randomize the beam position slightly (beam spot size)
    G4double z0 = -90.*cm; // 10 cm before target
    
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -90.*cm));
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

void DAMSAPrimaryGeneratorAction::SetBeamEnergy(G4double energy)
{
    fCurrentEnergy = energy;
    fParticleGun->SetParticleEnergy(energy);
    G4cout << "Beam energy set to: " << energy/MeV << " MeV" << G4endl;
}

G4double DAMSAPrimaryGeneratorAction::GetBeamEnergy() const
{
    return fCurrentEnergy;
}

