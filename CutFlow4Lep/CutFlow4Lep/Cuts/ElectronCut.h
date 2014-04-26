#ifndef ELECTRONCUT_H
#define ELECTRONCUT_H

#include "CutFlow4Lep/Cuts/ParticleCuts.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"

#include "ElectronPhotonSelectorTools/TElectronLikelihoodTool.h"
#include "ElectronPhotonSelectorTools/TElectronIsEMSelector.h"
#include "ElectronPhotonSelectorTools/TElectronMultiLeptonSelector.h"

class ElectronCut : public ParticleCuts<Electron>
{
	public:
		ElectronCut(D3PDReader::Event *tEvent, vector<Electron*> *tInitElectronVec);
		~ElectronCut();

		Bool_t passedCut();
		void executeCut();
		void setIsLoose(Bool_t tIsLoose) {m_isLoose = tIsLoose;};

	protected:

	private:
		void initializeTools();
		Bool_t passedElectronCut(Electron *currElectronObj);
		Bool_t passedAuthorCut(Electron *currElectronObj);

  	Root::TElectronIsEMSelector *m_electronTool2011;
 		Root::TElectronLikelihoodTool *m_electronTool2012;		
	  Root::TElectronMultiLeptonSelector *m_electronTool2013;

		Double_t m_z0Cut;
		Bool_t m_isLoose;
};
#endif