#ifndef ZVERTEXWEIGHT_H
#define ZVERTEXWEIGHT_H

#include <CutFlow4Lep/Weights/Weights.h>
#include <CutFlow4Lep/Calculations/MCHiggsMass.h>
#include <CutFlow4Lep/StructDef.h>

#include <egammaAnalysisUtils/VertexPositionReweightingTool.h>

using namespace std;

class ZVertexWeight : public Weights
{
	public:
		ZVertexWeight(D3PDReader::Event *tEvent);
		~ZVertexWeight();

	protected:

	private:
		void initializeReweight();
		void setWeight();

		VertexPositionReweightingTool *m_reweight;
};
#endif