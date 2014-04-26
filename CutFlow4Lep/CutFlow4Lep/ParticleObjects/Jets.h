#ifndef JETS_H
#define JETS_H

#include "CutFlow4Lep/ParticleObjects/ChargedLepton.h"

using namespace std;
class Jets : public ChargedLepton
{
	public:
		Jets(D3PDReader::Event *tEvent, D3PDReader::JetD3PDObjectElement *tJets, Int_t tType);
		~Jets();

		D3PDReader::JetD3PDObjectElement* getJets() {return m_jets;};
		Int_t getType() {return m_type;};

	protected:

	private:
		void init();
		void setMomentumLorentzVectors();
		void setTypes();

		D3PDReader::JetD3PDObjectElement *m_jets;
		Int_t m_type;
};
#endif