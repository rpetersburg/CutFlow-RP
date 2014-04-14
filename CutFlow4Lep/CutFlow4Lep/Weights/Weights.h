#ifndef WEIGHTS_H
#define WEIGHT_H

#include <D3PDReader/Event.h>

#include "CutFlow4Lep/UsesEvent.h"

using namespace std;

class Weights : public UsesEvent
{
	public:
		Weights(D3PDReader::Event *tEvent) : UsesEvent(tEvent), m_weight(1.0) {};
		~Weights() {}

		Double_t getWeight() {return m_weight;};
		void setWeight(Double_t tWeight) {m_weight = tWeight;};

	protected:
		virtual void setWeight() = 0;
		Double_t m_weight;

	private:

};
#endif