//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file exampleB5.cc
/// \brief Main program of the analysis/B5 example

#include "globals.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"
#include "time.h"

#include "B5DetectorConstruction.hh"
#include "B5PrimaryGeneratorAction.hh"

#include "B5DetectorConstruction.hh"
#include "B5ActionInitialization.hh"
#include "B5PhysicsList.hh"
#include "G4RunManager.hh"
#include "G4VModularPhysicsList.hh"
#include "G4PhysListFactory.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"

#include "G4StepLimiterPhysics.hh"


#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TRandom3.h"
//#include "TInterpreter.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
bool gSaveStepLevel = false;
long gSeed = 0;
int main(int argc,char** argv)
{
  if (argc != 1 && argc != 4){
    std::cout << "./exampleB5 [1. Macro file name] [2. Output file name] [3. Random seed number]" << std::endl;
    std::cout << "ex) ./exampleB5 run.mac example 1" << std::endl;
    return 0;
  }
  if (argc == 4){
    //choose the Random engine
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
    gSeed = (long) atol(argv[3]);
    //set random seed with system time
    //G4long seed = time(NULL);
    CLHEP::HepRandom::setTheSeed(gSeed);
    gRandom -> SetSeed(gSeed);
  }
  TString str_fname = argv[2];
  if (str_fname == ""){
    str_fname = "vistest.root";
  }
  
  if (str_fname(str_fname.Sizeof()-6,5) != ".root") 
    str_fname += ".root";
  
  auto tr = new TTree("tree","test");
  
  gSaveStepLevel = true;  
  G4cout << "Save Step Level : " << gSaveStepLevel << G4endl;
  
  G4RunManager* runManager = new G4RunManager;
  runManager -> SetUserInitialization(new B5PhysicsList());
  runManager -> SetUserInitialization(new B5ActionInitialization(tr));
  runManager -> SetUserInitialization(new B5DetectorConstruction());
  runManager -> SetUserAction(new B5PrimaryGeneratorAction());
  runManager -> Initialize();
  
  TFile *tf = new TFile(str_fname,"RECREATE");
  
  G4VisManager* visManager = new G4VisExecutive;
  visManager -> Initialize();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  if (argc != 1) 
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager -> ApplyCommand(command+fileName);

  }
  else 
  {

    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager -> ApplyCommand("/control/execute vis.mac"); 
    ui -> SessionStart();

    delete ui;
  }
  
  
  tf -> cd();
  tr -> Write();
  tf -> Close();
  
  delete visManager;
  delete runManager;

  return 0;
}
