#include "CutFlow4Lep/Cuts/QuadLeptonCut.h"

QuadLeptonCut::QuadLeptonCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton, Bool_t tDoZ4lAnalysis)
	: QuadLeptonCuts(tEvent, tQuadLepton), m_quadLepton(tQuadLepton), m_doZ4lAnalysis(tDoZ4lAnalysis)
{
	init();
}

QuadLeptonCut::~QuadLeptonCut()
{
	delete m_zMassCut;
	delete m_deltaRCut;
	delete m_jPsiVetoCut;
	delete m_trackIsoCut;
	delete m_caloIsoCut;
	delete m_d0SigCut;
}

void QuadLeptonCut::init()
{
	m_zMassCut = new ZMassCut(m_event, m_quadLepton, m_doZ4lAnalysis);
	m_deltaRCut = new DeltaRCut(m_event, m_quadLepton);
	m_jPsiVetoCut = new JPsiVetoCut(m_event, m_quadLepton);
	m_trackIsoCut = new TrackIsoCut(m_event, m_quadLepton);
	m_caloIsoCut = new CaloIsoCut(m_event, m_quadLepton);
	m_d0SigCut = new D0SigCut(m_event, m_quadLepton);

	m_fsrCorrection = new FSRCorrection(m_event, m_quadLepton, m_electronSmearObj);
}

void QuadLeptonCut::setQuadLepton(QuadLepton *tQuadLepton)
{
	m_quadLepton = tQuadLepton;
	init();
}

Bool_t QuadLeptonCut::passedCut()
{
	if (!m_zMassCut->passedCut()) return false;
	if (!m_deltaRCut->passedCut()) return false;
	if (!m_jPsiVetoCut->passedCut()) return false;

	Bool_t passedTrackIso = m_trackIsoCut->passedCut();
	Bool_t passedCaloIso = m_caloIsoCut->passedCut();
	Bool_t passedD0Sig = m_d0SigCut->passedCut();

	// fillEventVarInfo(quadEvent);
	massCalc();
	m_fsrCorrection->executeCorrection();
	correctZMassConstraint();



	if (!(passedTrackIso && passedCaloIso && passedD0Sig))
		return false;



}

void QuadLeptonCut::massCalc()
{
	m_quadLepton->setMass(InvariantMass::invMassCalc(m_quadLepton->getLeptonLorentzVec()));

}

void QuadLeptonCut::fillEventVarInfo(QuadLepton *quadLepton)
{

}