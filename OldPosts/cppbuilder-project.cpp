#include <vcl.h>

WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->Run();
	}
	//...

	return 0;
}// Codigo-fonte disponivel no GitHub (Caloni/Caloni.com.br) 
