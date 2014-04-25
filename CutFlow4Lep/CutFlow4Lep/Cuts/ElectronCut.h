#ifndef ELECTRONCUT_H
#define ELECTRONCUT_H

#include "CutFlow4Lep/Cuts/Cuts.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"

#include "ElectronPhotonSelectorTools/TElectronLikelihoodTool.h"
#include "ElectronPhotonSelectorTools/TElectronIsEMSelector.h"
#include "ElectronPhotonSelectorTools/TElectronMultiLeptonSelector.h"

class ElectronCut : public Cuts
{
	public:
		ElectronCut(D3PDReader::Event *tEvent, vector<Electron*> *tInitElectronVec);
		~ElectronCut();

		Bool_t passedCut();

		vector<Electron*> getCutElectronVec() {return m_cutElectronVec;};
		void setInitElectronVec(vector<Electron*> *tInitElectronVec) {m_initElectronVec = tInitElectronVec;}

	protected:

	private:
		void init();
		Bool_t passedElectronCut(Electron *currElectronObj);
		Bool_t passedAuthorCut(Electron *currElectronObj);

		vector<Electron*> *m_initElectronVec;
		vector<Electron*> m_cutElectronVec;

  	Root::TElectronIsEMSelector *m_electronTool2011;
 		Root::TElectronLikelihoodTool *m_electronTool2012;		
	  Root::TElectronMultiLeptonSelector *m_electronTool2013;

		Double_t m_z0Cut;
};
#endif