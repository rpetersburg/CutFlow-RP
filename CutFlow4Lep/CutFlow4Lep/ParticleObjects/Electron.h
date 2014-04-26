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
		Electron(D3PDReader::Event *tEvent, D3PDReader::ElectronD3PDObjectElement *tElectron, Double_t tElectronEff, Double_t tElectronSmear, Double_t tElectronResolution, Double_t tElectronClusterpT);
		~Electron();

		D3PDReader::ElectronD3PDObjectElement* getElectron() {return m_electron;};
		void fill(Double_t tElectronEff, Double_t tElectronSmear, Double_t tElectronResolution, Double_t tElCLPt);
		void setResolution(Double_t tResolution) {m_resolution = tResolution;};
		void setClusterpT(Double_t tClusterpT) {m_clusterpT = tClusterpT;};

	protected:

	private:
		void init();
		void setMomentumLorentzVectors();
		void setSysEnergy();
		void setptCone20Correction();
		void setTypes();

		D3PDReader::ElectronD3PDObjectElement *m_electron;

		Double_t m_resolution;
		Double_t m_clusterpT
};
#endif