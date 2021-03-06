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
/// \file B2EventAction.cc
/// \brief Implementation of the B2EventAction class

#include "B2EventAction.hh"
#include "B2TrackerSD.hh"

#include "B4Analysis.hh"

#include "G4SteppingControl.hh"
#include "G4StepPoint.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"

#include <iomanip>

#include "G4EventManager.hh"
#include "G4VHitsCollection.hh"
#include "G4ios.hh"

#include <iostream>
#include <fstream>
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2EventAction::B2EventAction()
  : G4UserEventAction(),
    fPixHCID(-1)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2EventAction::~B2EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
B2TrackerHitsCollection*
B2EventAction::GetHitsCollection(G4int hcID,
		const G4Event* event) const
{
        B2TrackerHitsCollection* hitsCollection
	= static_cast<B2TrackerHitsCollection*>(
			event->GetHCofThisEvent()->GetHC(hcID));


	if ( ! hitsCollection ) {
		G4ExceptionDescription msg;
		msg << "Cannot access hitsCollection ID " << hcID;
		G4Exception("B4cEventAction::GetHitsCollection()",
				"MyCode0003", FatalException, msg);
	}

	return hitsCollection;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2EventAction::BeginOfEventAction(const G4Event*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2EventAction::EndOfEventAction(const G4Event* event)
{
  // get number of stored trajectories

  fPixHCID
    = G4SDManager::GetSDMpointer()->GetCollectionID("TrackerHitsCollection");
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  B2TrackerHitsCollection* pixHC = GetHitsCollection(fPixHCID, event);
 
  G4int entries = pixHC->entries();
 
 
  G4double* trID_1 =new G4double[1024*216*512*3];

  for(G4int i = 0; i<1024; i++){
    for(G4int j = 0; j<216; j++){
      for(G4int k = 0; k<512; k++){
	for(G4int l = 0; l<3; l++){
	  
	  *(trID_1+i*216*512*3+j*512*3+k*3+l)=-9999.;
	  
	}
      }
    }
  }

  // G4int i=1,j=1,k=1,m=1;
  //  G4cout  << *(Digit+i*6*10*512+j*6*10+k*6+m) << G4endl;
    
 

  for(G4int ii=0; ii<entries;ii++){
    B2TrackerHit* pixHit = (*pixHC)[ii];
    
    analysisManager->FillNtupleIColumn(1, 0, pixHit->GetPixelNb());
    analysisManager->FillNtupleIColumn(1, 1, pixHit->GetTrackID());
    analysisManager->FillNtupleIColumn(1, 2, pixHit->GetPDGCode());
    analysisManager->FillNtupleIColumn(1, 3, pixHit->GetSensorNb());
    analysisManager->FillNtupleDColumn(1, 4, pixHit->GetETot());
    analysisManager->FillNtupleIColumn(1, 5, pixHit->GetEventID());
    analysisManager->FillNtupleDColumn(1, 6, pixHit->GetPixelPosX());
    analysisManager->FillNtupleDColumn(1, 7, pixHit->GetPixelPosY());
    analysisManager->FillNtupleDColumn(1, 8, pixHit->GetPixelPosZ());
    analysisManager->FillNtupleIColumn(1, 9, pixHit->GetBigSensorNb());
    analysisManager->FillNtupleDColumn(1, 10, pixHit->GetMomentum_x());
    analysisManager->FillNtupleDColumn(1, 11, pixHit->GetMomentum_y());
    analysisManager->FillNtupleDColumn(1, 12, pixHit->GetMomentum_z());
    analysisManager->FillNtupleDColumn(1, 13, pixHit->GetVertexPosX());
    analysisManager->FillNtupleDColumn(1, 14, pixHit->GetVertexPosY());
    analysisManager->FillNtupleDColumn(1, 15, pixHit->GetVertexPosZ());
    analysisManager->AddNtupleRow(1);


    G4int i = pixHit->GetSensorNb();    
    G4int j = pixHit->GetBigSensorNb();
    G4int k = pixHit->GetPixelNb();
    G4int tr = pixHit->GetTrackID();
   
    G4double tr1 = (G4double) tr;

      
      if( *(trID_1+i*216*512*3+j*512*3+k*3+0) == -9999.){
	*(trID_1+i*216*512*3+j*512*3+k*3+0) = tr1;
	analysisManager->FillNtupleIColumn(2, 0, pixHit->GetEventID());
	analysisManager->FillNtupleIColumn(2, 1, pixHit->GetSensorNb());
	analysisManager->FillNtupleIColumn(2, 2, pixHit->GetBigSensorNb());
	analysisManager->FillNtupleIColumn(2, 3, pixHit->GetPixelNb());
	analysisManager->FillNtupleIColumn(2, 4, pixHit->GetPDGCode());
	analysisManager->FillNtupleDColumn(2, 5, pixHit->GetPixelPosX());
	analysisManager->FillNtupleDColumn(2, 6, pixHit->GetPixelPosY());
	analysisManager->FillNtupleDColumn(2, 7, pixHit->GetPixelPosZ());
	analysisManager->FillNtupleDColumn(2, 8, tr1);
	analysisManager->FillNtupleDColumn(2, 9, *(trID_1+i*216*512*3+j*512*3+k*3+1));
	analysisManager->FillNtupleDColumn(2, 10, *(trID_1+i*216*512*3+j*512*3+k*3+2));
	analysisManager->FillNtupleDColumn(2, 11, pixHit->GetMomentum_x());
	analysisManager->FillNtupleDColumn(2, 12, pixHit->GetMomentum_y());
	analysisManager->FillNtupleDColumn(2, 13, pixHit->GetMomentum_z());
	analysisManager->AddNtupleRow(2);
      }

      if(  *(trID_1+i*216*512*3+j*512*3+k*3+0) > -9999. && tr1 != *(trID_1+i*216*512*3+j*512*3+k*3+0))
	*(trID_1+i*216*512*3+j*512*3+k*3+1) = tr1;
      
      else if(  *(trID_1+i*216*512*3+j*512*3+k*3+1) > -9999. && tr1 != *(trID_1+i*216*512*3+j*512*3+k*3+1))
	*(trID_1+i*216*512*3+j*512*3+k*3+2) = tr1;
    }
   
  
   delete[] trID_1;
  }  


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
