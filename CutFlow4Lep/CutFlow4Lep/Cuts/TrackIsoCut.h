#ifndef TRACKISOCUT_H
#define TRACKISOCUT_H

#include "CutFlow4Lep/Cuts/QuadLeptonCuts.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"

using namespace std;
class TrackIsoCut : public QuadLeptonCuts
{
public:
	TrackIsoCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton);
	~TrackIsoCut();

	Bool_t passedCut();

protected:

private:
	vector<Double_t> getTrackPT();
};
#endif