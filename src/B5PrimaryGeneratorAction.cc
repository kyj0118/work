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
/// \file B5PrimaryGeneratorAction.cc
/// \brief Implementation of the B5PrimaryGeneratorAction class

#include "B5PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "TRandom3.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ThreeVector gPrimaryParticlePosition;
G4ThreeVector gPrimaryParticleMomentumDirection;
int gPrimaryParticlePDG;
double gPrimaryParticleEnergy;
double gPrimaryParticleMass;
B5PrimaryGeneratorAction::B5PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),     
  fParticleGun(nullptr),
  fGeneralParticleSource(nullptr), 
  fGamma(nullptr),
  fMomentum(1000.*MeV)
{
  fUseMacFile = 0;
  fMomentum = 1000.*MeV;
  if(fUseMacFile){
    fGeneralParticleSource  = new G4GeneralParticleSource();
  }
  else {
    auto particleTable = G4ParticleTable::GetParticleTable();
    fGamma = particleTable->FindParticle("gamma");
    fParticleGun  = new G4ParticleGun(fGamma);
    fParticleGun->SetParticleMomentum(fMomentum);
  }
  
  // define commands for this class
  //DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B5PrimaryGeneratorAction::~B5PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGeneralParticleSource;
  //delete fGamma;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B5PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  
  if(fUseMacFile){
    gPrimaryParticlePosition = fGeneralParticleSource -> GetParticlePosition();
    gPrimaryParticleEnergy = fGeneralParticleSource -> GetParticleEnergy();
    gPrimaryParticleMomentumDirection = fGeneralParticleSource -> GetParticleMomentumDirection();
    gPrimaryParticlePDG = fGeneralParticleSource ->	GetParticleDefinition() -> GetPDGEncoding();
    gPrimaryParticleMass = fGeneralParticleSource -> GetParticleDefinition() -> GetPDGMass();
    fGeneralParticleSource -> GeneratePrimaryVertex(event);
  }
  else {
    fParticleGun->SetParticleDefinition(fGamma);  
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
    fParticleGun->SetParticleTime(0.0*ns);
    // random generation
    double dx,dy,dz;
    double theta_limit = cos(30.0/180.0*3.14159265358979); // 0 ~ 30 degree
    dz = gRandom -> Uniform(theta_limit,1); // uniform cos(theta)
    double phi = gRandom -> Uniform(0,4*3.14159265358979); // uniform phi
    double sin_theta= sqrt(1.0-dz*dz);
    dx = sin_theta * cos(phi);
    dy = sin_theta * sin(phi); 
    //gRandom -> Sphere(dx,dy,dz,1);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(dx,dy,dz));
    
    
    gPrimaryParticlePosition = fParticleGun -> GetParticlePosition();
    gPrimaryParticleEnergy = fParticleGun -> GetParticleEnergy();
    gPrimaryParticleMomentumDirection = fParticleGun -> GetParticleMomentumDirection();
    gPrimaryParticlePDG = fParticleGun ->	GetParticleDefinition() -> GetPDGEncoding();
    gPrimaryParticleMass = fParticleGun -> GetParticleDefinition() -> GetPDGMass();

    fParticleGun->GeneratePrimaryVertex(event);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
