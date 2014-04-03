#include "CutFlow4Lep/Cuts/Cuts.h"

using namespace std;
class VertexCut : public Cuts
{
	public:
		VertexCut(D3PDReader::Event *tEvent);
		~VertexCut() {};

		Bool_t passedCut();

	protected:

	private:
};