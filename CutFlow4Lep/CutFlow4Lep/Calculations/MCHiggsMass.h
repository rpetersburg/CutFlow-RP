#include <TString.h>
#include "D3PDReader/Event.h"

using namespace std;
class MCHiggsMass
{
	public:
		MCHiggsMass(D3PDReader::Event *tEvent, vector<TString> tFileNameVec);
		~MCHiggsMass();

		Double_t getMass();

	protected:

	private:
		void setMass();
		TString findNumInString(TString sampleName, Int_t startIndex);

		Double_t m_mass;
		D3PDReader::Event *m_event;
		vector<TString> m_fileNameVec;
};