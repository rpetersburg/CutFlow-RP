#include <CutFlow4Lep/Weights/Weights.h>

using namespace std;

class ggFWeight : public Weights
{
	public:
		ggFWeight(D3PDReader::Event *tEvent, Int_t tSampleType);
		~ggFWeight();

	protected:

	private:
		void setWeight();


};
