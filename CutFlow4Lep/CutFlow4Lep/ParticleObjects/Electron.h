#ifndef ELECTRON_H
#define ELECTRON_H

#include "CutFlow4Lep/ParticleObjects/ChargedLepton.h"
#include "CutFlow4Lep/StructDef.h"

#include "ElectronPhotonSelectorTools/TElectronLikelihoodTool.h"
#include "ElectronPhotonSelectorTools/TElectronIsEMSelector.h"
#include "ElectronPhotonSelectorTools/TElectronMultiLeptonSelector.h"

#include <TLorentzVector.h>
#include <TPython.h>

using namespace std;
class Electron : public ChargedLepton
{
	public:
		Electron(D3PDReader::Event *tEvent, D3PDReader::ElectronD3PDObjectElement *tElectron);
		Electron(D3PDReader::Event *tEvent, D3PDReader::ElectronD3PDObjectElement *tElectron, Double_t tElectronEff, Double_t tElectronSmear);
		~Electron();

		D3PDReader::ElectronD3PDObjectElement* getElectron() {return m_electron;};

	protected:

	private:
		void init();
		void setMomentumLorentzVectors();
		void setSysEnergy();
		void setptCone20Correction();
		void setTypes();

		D3PDReader::ElectronD3PDObjectElement *m_electron;
};

#endif