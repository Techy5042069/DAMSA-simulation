#include "DAMSADetectorConstruction.hpp"  // our header
#include "G4RunManager.hh"
#include "G4NistManager.hh"               // material list
#include "G4Box.hh"                       // box shapes
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"            //store pointers to scoring volume logical volume 
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSTrackLength.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4SubtractionSolid.hh"
#include "DAMSASensitiveDetector.hpp"

DAMSADetectorConstruction::DAMSADetectorConstruction()
: G4VUserDetectorConstruction(),                           // : starts an initialization list. setting all pointers to nullptr
  fTungstenTargetLV(nullptr), fDecayChamberLV(nullptr), fCsICalLV(nullptr), fScoringVolumeLV(nullptr), fTungsten(nullptr), fVacuum(nullptr), fSilicon(nullptr), fCsI(nullptr), fAir(nullptr), fMagField(nullptr), fFieldMgr(nullptr), fCsIPositionZ(-6*cm)
{
    for(int i=0; i<6; i++) fSiTrackerLV[i] = nullptr;
    DefineMaterials();                                    // call our material setup function. must be called before geometry
}

DAMSADetectorConstruction::~DAMSADetectorConstruction()     
{ 
    delete fMagField;    
}

void DAMSADetectorConstruction::DefineMaterials()         // Instance() gets the only one datatbase. Only one with one name
{
    G4NistManager* nist = G4NistManager::Instance();
    
    // Define materials
    fTungsten = nist->FindOrBuildMaterial("G4_W");
    fVacuum = nist->FindOrBuildMaterial("G4_Galactic");
    fSilicon = nist->FindOrBuildMaterial("G4_Si");
    fCsI = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
    fAir = nist->FindOrBuildMaterial("G4_AIR");
}
G4VPhysicalVolume* DAMSADetectorConstruction::Construct()   // Construct() most important function
{
    // World volume (defining world dimensions) must be big enough to hold everything
    G4double worldSizeX = 1.0*m;   // G4double is GEANT's version of double
    G4double worldSizeY = 1.0*m;   // must specify units
    G4double worldSizeZ = 2.0*m;
    
    G4Box* solidWorld = new G4Box("World", worldSizeX/2, worldSizeY/2, worldSizeZ/2); // boxes use half-widths from center
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, fAir, "World");     // Solid (shape) + material = LV, LV has geometry and material, but NO POSITION yet
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0); // no rotation(box stays aligned with axes), position (0,0,0) empty so its at origin, what to place, name, no mother volume( this is top-level container), no boolean operation(subtract shapes to create holes), copy number (first and only copy)  LV + position = physical volume (place in space)
    
    // DAMSA Target: 5cm x 5cm x 10cm tungsten
    G4Box* targetSolid = new G4Box("TargetOuter", 2.5*cm, 2.5*cm, 5.0*cm); // position, half-width measurements
    fTungstenTargetLV = new G4LogicalVolume(targetSolid, fTungsten, "TungstenTarget");  // shape + material = LV
    new G4PVPlacement(0, G4ThreeVector(0, 0, -80*cm), fTungstenTargetLV, "TungstenTarget", logicWorld, false, 0); // everything we place must go inside something else. except world which has no mother.
    
    // Vacuum decay chamber: 10cm radius x 30cm length
    G4Tubs* decayChamberOuter = new G4Tubs("DecayChamberOuter", 0, 10*cm, 15*cm, 0, 360*deg); // 0 inner radius, 10 outer radius, 15 half length in z, 0 starting angle, 360 spanning angle (full circle)
    G4Tubs* decayChamberHole = new G4Tubs("DecayChamberHole", 0, 1*cm, 15.1*cm, 0, 360*deg); // creates inner hole (beam pipe), 1 cm radius hole
    G4SubtractionSolid* decayChamberSolid = new G4SubtractionSolid("DecayChamber", decayChamberOuter, decayChamberHole); // cylinder with hole as hollow tube
    fDecayChamberLV = new G4LogicalVolume(decayChamberSolid, fVacuum, "DecayChamber"); // hollow tube filled with vacuum
    new G4PVPlacement(0, G4ThreeVector(0, 0, -60*cm), fDecayChamberLV, "DecayChamber", logicWorld, false, 0); // position
    
    // Silicon tracker layers (6 identical layers, 2cm spacing, 10cm x 10cm each)
    for(int i = 0; i < 6; i++) {
        G4Box* siLayerBox = new G4Box("SiLayer", 5*cm, 5*cm, 0.1*cm);
        G4Box* siHole = new G4Box("SiHole", 1*cm, 1*cm, 0.2*cm); // beam hole in silicon
        G4SubtractionSolid* siLayerSolid = new G4SubtractionSolid("SiLayer", siLayerBox, siHole);  // silicon wafer with hole
        fSiTrackerLV[i] = new G4LogicalVolume(siLayerSolid, fSilicon, "SiTracker"); // storing in array element i
        
        G4double zPos = -40*cm + i*2*cm;  // Start at -40cm, 2cm spacing, calculates z position for each layer
        new G4PVPlacement(0, G4ThreeVector(0, 0, zPos), fSiTrackerLV[i], "SiTracker", logicWorld, false, i); // i is the copy number, 0,1,2... important to identify which layer was hit.
    }
    
    // CsI Electromagnetic Calorimeter (12cm x 12cm x 44cm)
    G4Box* csiBox = new G4Box("CsIBox", 6*cm, 6*cm, 22*cm);
    G4Box* csiHole = new G4Box("CsIHole", 1*cm, 1*cm, 22.1*cm);  // beam hole
    G4SubtractionSolid* csiSolid = new G4SubtractionSolid("CsICalorimeter", csiBox, csiHole);
    fCsICalLV = new G4LogicalVolume(csiSolid, fCsI, "CsICalorimeter");
    new G4PVPlacement(0, G4ThreeVector(0, 0, fCsIPositionZ), fCsICalLV, "CsICalorimeter", logicWorld, false, 0);

    //Scoring volume at detector entrance(invisible, counts particles)
    G4Box* scoringBox = new G4Box("ScoringBox", 15*cm, 15*cm, 0.1*mm);    //area bgger than detector to catch all particles, thickness b=very thin as we just need to detect crossing
    fScoringVolumeLV = new G4LogicalVolume(scoringBox, fVacuum, "ScoringVolume");  //combine shape+vacuum material
    new G4PVPlacement(0, G4ThreeVector(0, 0, -28*cm), fScoringVolumeLV, "ScoringVolume", logicWorld, false, 0);  //place scoring volume at Z=-28 cm(detector entrance), last silicon layer(-30 cm), CsI front(-28 cm), scoring volume right at detector front

   
    G4VisAttributes* invisibleVis = new G4VisAttributes();  //making scoring volume invisible, just a counter
    invisibleVis->SetVisibility(false);
    fScoringVolumeLV->SetVisAttributes(invisibleVis);

    // Target exit scoring volume (Z = -75cm, right after target)    //same but at target exit
    G4Box* targetExitBox = new G4Box("TargetExitBox", 15*cm, 15*cm, 0.1*mm);
    G4LogicalVolume* targetExitLV = new G4LogicalVolume(targetExitBox, fVacuum, "TargetExitVolume");
    new G4PVPlacement(0, G4ThreeVector(0, 0, -75*cm), targetExitLV, "TargetExitVolume", logicWorld, false, 0);
    targetExitLV->SetVisAttributes(invisibleVis);
    
    // Add colors for visualization
    G4VisAttributes* tungstenVis = new G4VisAttributes(G4Colour(0.7, 0.7, 0.7)); // (red, green, blue) 0 is dark, 1 is bright
    fTungstenTargetLV->SetVisAttributes(tungstenVis);
    
    G4VisAttributes* vacuumVis = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.3));  // optional alpha for opacity
    fDecayChamberLV->SetVisAttributes(vacuumVis);
    
    G4VisAttributes* siVis = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
    for(int i = 0; i < 6; i++) fSiTrackerLV[i]->SetVisAttributes(siVis);

    G4VisAttributes* csiVis = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
    fCsICalLV->SetVisAttributes(csiVis);
    
    return physWorld; // returns the world volume to GEANT4
}

void DAMSADetectorConstruction::SetupMagneticField()
{
    // Create uniform magnetic field of 1 Tesla in Z direction
    fMagField = new G4UniformMagField(G4ThreeVector(0., 1.0*tesla, 0.));
    
    // Create field manager
    fFieldMgr = new G4FieldManager();
    fFieldMgr->SetDetectorField(fMagField);
    fFieldMgr->CreateChordFinder(fMagField);
    
    G4cout << "1T magnetic field applied in y-direction" << G4endl;
}

void DAMSADetectorConstruction::ConstructSDandField()  // apply sensitive detectors and field
{
   // SetupMagneticField();
    
    // Apply magnetic field to silicon tracker region
   // for(int i = 0; i < 6; i++) {
    //    fSiTrackerLV[i]->SetFieldManager(fFieldMgr, true);
   // }
    // Create sensitive detector
    G4SDManager* SDman = G4SDManager::GetSDMpointer();  // gets sensitive detector manager 

    //Silicon tracker sensitive detector
    DAMSASensitiveDetector* siSD = new DAMSASensitiveDetector("SiTrackerSD", "SiTrackerHitsCollection"); // creates sensitive detector for silicon, registering it with GEANT4
    SDman->AddNewDetector(siSD);
    for(int i = 0; i<6; i++) {
        fSiTrackerLV[i]->SetSensitiveDetector(siSD);  // makes all silicon layers sensitive for recording hits
    }

    //CsI calorimeter sensitive detector
    DAMSASensitiveDetector* csiSD = new DAMSASensitiveDetector("CsICalSD", "CsICalHitsCollection");
    SDman->AddNewDetector(csiSD);
    fCsICalLV->SetSensitiveDetector(csiSD);

    G4cout << "Sensitive detectors configured for Si trackers and CsI calorimeter" << G4endl; 

}

