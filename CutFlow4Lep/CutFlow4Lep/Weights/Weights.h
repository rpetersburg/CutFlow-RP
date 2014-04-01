#include <iostream>

#include <TString.h>

#include <D3PDReader/Event.h>

using namespace std;

class Weights
{
	public:
		Weights(D3PDReader::Event *tEvent, Int_t tDataYear) : m_event(tEvent), m_dataYear(tDataYear), m_weight(1.0) {};
		~Weights() {}

		Double_t getWeight() {return m_weight;};
		void setWeight(Double_t tWeight) {m_weight = tWeight;};

	protected:
		virtual void setWeight() = 0;

		D3PDReader::Event *m_event;
		Int_t m_dataYear;
		Double_t m_weight;

	private:

};