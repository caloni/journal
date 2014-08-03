#include "Service.h"

class TaServido : public Service
{
public:
	TaServido();

private:
	void Main();
	void Control(DWORD controlCode);

	int events[3]; // SERVICE_CONTROL_STOP, _PAUSE, _CONTINUE
	char* goodbyeMessage; // mensagem de adeus
};
