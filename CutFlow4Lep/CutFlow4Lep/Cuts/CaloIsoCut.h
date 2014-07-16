#ifndef CALOISOCUT_H
#define CALOISOCUT_H

#include "CutFlow4Lep/Cuts/QuadLeptonCuts.h"

#include "egammaAnalysisUtils/CaloIsoCorrection.h"

using namespace std;
class CaloIsoCut : public QuadLeptonCuts
{
public:
	CaloIsoCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton);
	~CaloIsoCut();

	Bool_t passedCut();

protected:

private:
	vector<Double_t> getCaloET();
	Double_t getCaloIsoCorrection(D3PDReader::ElectronD3PDObjectElement *currElectron);
	Double_t deltaRCaloIso(ChargedLepton *iLepton, ChargedLepton *jLepton);
	Double_t deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2);
};
#endif