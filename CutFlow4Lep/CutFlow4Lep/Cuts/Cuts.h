#include <D3PDReader/Event.h>

using namespace std;

class Cuts
{
	public:
		Cuts(D3PDReader::Event *tEvent) : m_event(tEvent) {};
		~Cuts() {};

		virtual Bool_t passedCut() = 0;

	protected:
		D3PDReader::Event *m_event;

	private:

};