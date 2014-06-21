#ifndef CHANNELCUTFLOW_H
#define CHANNELCUTFLOW_H

#include <TROOT.h>

#include "CutFlow4Lep/ParticleObjects/QuadLepton.h"

using namespace std;
class ChannelCutFlow
{
	public:
		ChannelCutFlow();
		~ChannelCutFlow();

		virtual Bool_t passedCut() = 0;

	protected:
		template<class Lepton> vector<DiLepton*> getDiLeptons(vector<Lepton*> *leptonVec);
		vector<QuadLepton*> getQuadLeptons(vector<DiLepton*> *diLeptons);
		vector<QuadLepton*> getQuadLeptons(vector<DiLepton*> *iDiLeptons, vector<DiLepton*> *jDiLeptons);

		QuadLepton* getQuadEvent(vector<QuadLepton*> *quadLeptonVec, Bool_t doZ4lAnalysis);

		Int_t getNumCaloAndStandAlone(DiLepton *diLepton);

	private:
};
#endif