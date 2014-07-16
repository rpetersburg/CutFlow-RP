#include "CutFlow4Lep/Cuts/TrackIsoCut.h"

TrackIsoCut::TrackIsoCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton)
	: QuadLeptonCuts(tEvent, tQuadLepton)
{

}

TrackIsoCut::~TrackIsoCut()
{

}

Bool_t TrackIsoCut::passedCut()
{
	Bool_t passedCut = true;
	m_quadLepton->clearTrackIsoVecs();
	vector<ChargedLepton*> leptonVec = m_quadLepton->getLeptons();
	vector<Double_t> trackPTVec = getTrackPT();

	Double_t cutTrackIso = 0.15;

	for (UInt_t i = 0; i < leptonVec.size(); i++)
	{
		ChargedLepton *currLepton = leptonVec[i];
		Double_t dataTrackIso = trackPTVec[i] / currLepton->getMomentumVec()->Pt();

		m_quadLepton->trackIsoValPushBack(dataTrackIso);

		if (dataTrackIso > cutTrackIso)
		{
			m_quadLepton->trackIsoPushBack(false);
			passedCut = false;
		}
		else m_quadLepton->trackIsoPushBack(true);
	}
	return passedCut;
}

vector<Double_t> TrackIsoCut::getTrackPT()
{
	vector<ChargedLepton*> leptonVec = m_quadLepton->getLeptons();

	vector<Double_t> trackPT; // Container
	Double_t cutDeltaR = 0.20; // Delta R Cut

	vector<ChargedLepton*>::iterator iLeptonItr;
	vector<ChargedLepton*>::iterator jLeptonItr;
	for (iLeptonItr = leptonVec.begin(); iLeptonItr != leptonVec.end(); ++iLeptonItr)
	{
		ChargedLepton *iLepton = *iLeptonItr;
		Double_t currPTCone20 = iLepton->getPTCone20();

		for (jLeptonItr = leptonVec.begin(); jLeptonItr != leptonVec.end(); ++jLeptonItr)
		{
			ChargedLepton *jLepton = *jLeptonItr;
			if (iLepton == jLepton) continue; // Don't compare the same lepton

			Double_t dataDeltaR = iLepton->getMomentumVec()->DeltaR(*(jLepton->getMomentumVec()));
			Double_t subtrahend = 0; // Value to be subtracted
			if (dataDeltaR < cutDeltaR)
			{
				if (jLepton->getType() == LeptonType::MuonStaco || jLepton->getType() == LeptonType::MuonCalo)
				{
					Muon *jMuon = (Muon*)jLepton;
					subtrahend = sin(jMuon->getMuon()->id_theta()) / fabs(jMuon->getMuon()->id_qoverp());
				}
				else if (jLepton->getType() == LeptonType::MuonStandAlone)
					subtrahend = 0;
				else if (jLepton->getType() == LeptonType::ElectronGSF)
				{
					Electron *jElectron = (Electron*)jLepton;
					subtrahend = jElectron->getElectron()->trackpt();
				}
				else
					cout << "Error: TrackIsoCut::getTrackPT(): Unexpected lepton" << endl;
			}
			currPTCone20 = currPTCone20 - subtrahend;
		}
		trackPT.push_back(currPTCone20);
	}
	return trackPT;
}