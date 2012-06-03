/** @file HouaissParaBabylon.cpp

@brief Desofuscador e conversor do dicion�rio Houaiss para Babylon.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 2008-04

@desc Permite ver o conte�do dos arquivos do dicion�rio eletr�nico Houaiss 
de forma a ser importado para outros dicion�rios, ou usado independente
da interface oficial.

Linque:
Houaiss: http://www.dicionariohouaiss.com.br

Obs.:
Esse programa n�o se destina � pirataria. Ele n�o fornece nenhum dos
dois dicion�rios, que devem ser adquiridos legalmente. Sua �nica
fun��o � permitir o uso de um �timo dicion�rios em uma excelente
interface de busca de significado de palavras.

@todo Gera��o de log
@todo Testes com v�rias vers�es

Bom proveito!
*/
#include "Decryptor.h"
#include "Conversor.h"
#include "Shell.h"
#include "HouaissParaBabylonDialog.h"
#include <stdio.h>


#pragma comment(lib, "atlthunk.lib")


int WINAPI _tWinMain(HINSTANCE inst, HINSTANCE, PTSTR cmdLine, int show)
{
	DWORD ret;
	HouaissParaBabylonDialog mainDlg;

	Log("Starting program. Compilation: %S", __DATE__);
	InitCommonControls();
	mainDlg.DoModal();
	Log("Finishing program");

	ret = GetLastOperationError();

	if( ret != ERROR_SUCCESS )
	{
		Log("Last error: %08X", ret);

		if( MessageBox(NULL, _T("Ocorreram erros durante a convers�o ")
			_T("do dicion�rio. Gostaria de exportar um arquivo de log ")
			_T("para diagnosticar o problema?"),
			_T("Exporta��o de log"), MB_YESNO | MB_ICONINFORMATION) == IDYES )
		{
			DWORD logErr = ExportLogMessages();

			if( logErr != ERROR_SUCCESS )
			{
				CString errDesc = MessageDescription(logErr);
				
				MessageBox(NULL, errDesc, _T("Erro ao exportar arquivo de log"), 
					MB_ICONERROR | MB_OK);
			}
		}
	}

	return ret;
}
