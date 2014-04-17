#ifndef MUONSMEAR_H
#define MUONSMEAR_H

#include "CutFlow4Lep/Smear/Smear.h"
#include "CutFlow4Lep/StructDef.h"

#include <D3PDReader/MuonD3PDObject.h>

#include <MuonMomentumCorrections/SmearingClass.h>
#include <MuonMomentumCorrections/MuonResolutionAndMomentumScaleFactors.h>

#include <MuonEfficiencyCorrections/AnalysisMuonConfigurableScaleFactors.h>

#include <PileupReweighting/TPileupReweighting.h>


using namespace std;
class MuonSmear4Lep : public Smear
{
	public:
		MuonSmear4Lep(D3PDReader::Event *tEvent, Root::TPileupReweighting *tPileupReweighting);
		~MuonSmear4Lep() {};

		virtual void executeSmear() = 0;
		vector<Double_t> getEff() {return m_muonEff;};

	protected:
		virtual void initializeMuonObj() = 0;
		// Initialize appropriate objects for child use
		void initializeSmearObj();
		void initializeScaleFactors();
		void initializeResolutionAndMomentumScaleFactors();

		// Executes muon uncertainty correction (reduces duplicated code in children)
		void muonUncertaintyCorrection(D3PDReader::MuonD3PDObjectElement *currMuon);

		D3PDReader::MuonD3PDObject *m_muon;
		vector<Double_t> m_muonEff;
		MuonSmear::SmearingClass *m_smearTool;

		Analysis::MuonResolutionAndMomentumScaleFactors *m_muonResoMomScaleFactors;
		Analysis::AnalysisMuonConfigurableScaleFactors *m_stacoCSF;
		Analysis::AnalysisMuonConfigurableScaleFactors *m_stacoSACSF;
		Analysis::AnalysisMuonConfigurableScaleFactors *m_caloMuCSF;

		Root::TPileupReweighting *m_pileupReweighting;

	private:
};
#endif