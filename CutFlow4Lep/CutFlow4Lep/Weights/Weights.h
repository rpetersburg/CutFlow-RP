#include <iostream>

#include <TString.h>

#include <D3PDReader/Event.h>

using namespace std;

class Weights
{
	public:
		Weights(D3PDReader::Event *tEvent, TString tDataYear) : m_event(tEvent), m_dataYear(tDataYear), m_weight(1.0) {};
		~Weights() {}

		Double_t getWeight() {return m_weight;};

	protected:
		virtual void setWeight() = 0;

		D3PDReader::Event *m_event;
		TString m_dataYear;
		Double_t m_weight;

	private:

};