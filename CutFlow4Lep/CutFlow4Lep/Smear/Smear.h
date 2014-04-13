#ifndef SMEAR_H
#define SMEAR_H

#include "CutFlow4Lep/UsesEvent.h"

using namespace std;
class Smear : public UsesEvent
{
	public:
		Smear(D3PDReader::Event *tEvent) : UsesEvent(tEvent) {};
		~Smear() {};

		virtual void executeSmear() = 0;
		vector<Double_t> getSmear() {return m_smear;};

	protected:
		virtual void initializeSmearObj() = 0;
		vector<Double_t> m_smear;
		

	private:

};
#endif