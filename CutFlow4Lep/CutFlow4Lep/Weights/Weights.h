#include <TString.h>

#include <D3PDReader/Event.h>

using namespace std;

class Weights
{
	public:
		Weights(D3PDReader::Event *tEvent, Int_t tSampleType);
		~Weights();

		Double_t getEventWeight();
		Double_t getggFWeight();
		Double_t getHiggsWeight();
		Double_t getJHUWeight();
		Double_t getLepEffWeight();
		Double_t getPileupWeight();
		Double_t getTriggerWeight();
		Double_t getZVertexWeight();

	protected:

	private:
		void setggFWeight();
		void setHiggsWeight();
		void setJHUWeight();
		void setLepEffWeight();
		void setPileupWeight();
		void setTriggerWeight();
		void setZVertexWeight();

		D3PDReader::Event *m_event;
		Int_t m_sampleType;
};