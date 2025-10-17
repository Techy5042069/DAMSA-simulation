#ifndef DAMSADetectorConstruction_h       //include guards, prevents repeatation
#define DAMSADetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"  // importing GEANT's template for detectors
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SubtractionSolid.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class G4FieldManager;

class DAMSADetectorConstruction : public G4VUserDetectorConstruction    // custom detector class
{
public:                           // building on top of GEANT4's template
    DAMSADetectorConstruction();    // constructor, runs when automatically when we create the detector
    virtual ~DAMSADetectorConstruction();  // destructor, runs when simulation ends. cleans up memeory

    virtual G4VPhysicalVolume* Construct();  // main funcion where we build our detector geometry
    virtual void ConstructSDandField();      // second function where you add sensitive detectors (that records hits)

private:                                  // helper function that sets up materials like tungsten, silicon, etc.
    //Target
    G4LogicalVolume* fTungstenTargetLV;   // a shape + material combination, pointer to tungsten target

    //Decay Chamber
    G4LogicalVolume* fDecayChamberLV;   // f convention meaning 'field' , LV suffix means logical volume

    //Silicon tracker layers
    G4LogicalVolume* fSiTrackerLV[6];  // [6] depicts array of 6 silicon layers

    //CsI calorimeter
    G4LogicalVolume* fCsICalLV;         // storing different pointers to materials that we will be using
    G4LogicalVolume* fScoringVolumeLV;

    //Magnetic Field
    G4UniformMagField* fMagField;
    G4FieldManager* fFieldMgr;

    //Materials
    G4Material* fTungsten;
    G4Material* fVacuum;
    G4Material* fSilicon;
    G4Material* fCsI;
    G4Material* fAir;

    void DefineMaterials();
    void SetupMagneticField();
};

#endif
