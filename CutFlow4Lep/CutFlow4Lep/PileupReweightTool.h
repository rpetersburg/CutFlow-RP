#include <PileupReweighting/TPileupReweighting.h>
#include "CutFlow4Lep/StructDef.h"

using namespace std;
class PileupReweightTool
{
	public:
		PileupReweightTool(Int_t tDataYear, Int_t tMCCollection, Int_t tDataCalibration);
		~PileupReweightTool();

		Root::TPileupReweighting* getTool() {return m_tool;};

	protected:

	private:
		void initializeTool();

		Root::TPileupReweighting *m_tool;
		Int_t m_dataYear;
		Int_t m_mcCollection;
		Int_t m_dataCalibration;

};