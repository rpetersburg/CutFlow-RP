#include <TString.h>

#include <D3PDReader/Event.h>

#include "CutFlow4Lep\StructDef.h"

using namespace std;

class Weights
{
	public:
		Weights(D3PDReader::Event *tEvent, Int_t tSampleType, TString tDataYear);
		~Weights();

		Double_t getWeight() {return m_weight;};

		Double_t getEventWeight();
		Double_t getggFWeight();
		Double_t getHiggsWeight();
		Double_t getJHUWeight();
		Double_t getLepEffWeight();
		Double_t getPileupWeight();
		Double_t getTriggerWeight();
		Double_t getZVertexWeight();

	protected:
		void setWeight();
		Double_t m_weight;

		D3PDReader::Event *m_event;
		Int_t m_sampleType;
		TString m_dataYear;

	private:

		void setggFWeight();
		void setHiggsWeight();
		void setJHUWeight();
		void setLepEffWeight();
		void setPileupWeight();
		void setTriggerWeight();
		void setZVertexWeight();


};