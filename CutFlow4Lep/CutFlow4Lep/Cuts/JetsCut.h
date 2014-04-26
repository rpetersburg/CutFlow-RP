#ifndef JETSCUT_H
#define JETSCUT_H

#include "CutFlow4Lep/Cuts/ParticleCuts.h"
#include "CutFlow4Lep/ParticleObjects/Jets.h"

using namespace std;
class JetsCut : public ParticleCuts<Jets>
{
	public:
		JetsCut(D3PDReader::Event *tEvent, vector<Jets*> *tInitJetsVec);
		~JetsCut();

		Bool_t passedCut();
		void executeCut();

	protected:

	private:
		Bool_t passedEMCut(D3PDReader::JetD3PDObjectElement *currJets);
		Bool_t passedEMTruthCut(D3PDReader::JetD3PDObjectElement *currJets);
		Bool_t passedEMFidCut(D3PDReader::JetD3PDObjectElement *currJets);
		Bool_t passedEMTruthFidCut(D3PDReader::JetD3PDObjectElement *currJets);

		Bool_t passedpTCut(Double_t pT);
		Bool_t passedFidpTCut(Double_t pT);
		Bool_t passedEtaCut(Double_t pT, Double_t eta);
		Bool_t passedFidEtaCut(Double_t pT, Double_t eta, Double_t phi, Double_t M);
		Bool_t passedJetsCleaning(D3PDReader::JetD3PDObjectElement *currJets);

};
#endif