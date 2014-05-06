/** Programa de testes para o projeto IpcObject.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 08/2006
*/
#include "../../ipcobject.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>

#define IPCOBJTST_TIMEOUT INFINITE

using namespace std;


/** Lista de objetos criados e abertos. */
typedef map<string, HANDLE> IpcObjectList;


/** Protótipo para criar uma função de teste. */
typedef HRESULT (*FObjFunc)(ostream& os, istream &is);


/** Objeto-função que encapsula FObjFunc. Como é possível que o usuário
digite o nome de uma função inexistente, temos que garantir que não
seja chamado um ponteiro zerado. */
struct ObjFunc
{
	ObjFunc(FObjFunc objFunc = 0) 
	: m_objFunc(objFunc)
	{
	}

	HRESULT operator () (ostream& os, istream& is)
	{
		HRESULT ret = S_OK;

		if( m_objFunc )
			ret = m_objFunc(os, is);
		else
			os << "Function not found.\n";

		return ret;
	}

	FObjFunc m_objFunc;
};


/** Lista de funções de teste, mapeadas por um nome. */
typedef map<string, ObjFunc> FuncList;


/** Manipulador de ostream que formata a próxima saída para hexadecimal,
tamanho de 8 caracteres e espaços em branco preenchidos com zero.

@sample cout << "Last error: 0x" << setdw << GetLastError() << endl;
*/
ostream& setdw(ostream& os)
{
	return os << hex << setw(8) << setfill('0');
}


IpcObjectList g_objList; /**< Lista de objetos criados durante o teste. */
FuncList g_funcList; /**< Lista de funções de teste usadas. */


/** @brief Interpretador de comandos baseado em entrada-saída de streams. */
HRESULT ParseCommands(ostream& os, istream& is);

/** @brief Imprime descrição do sistema para o código de erro hr. */
void PrintErrorMessage(HRESULT hr, ostream& os);



/** Cria um objeto com o nome que é recebido de is. Imprime resultado em os. */
HRESULT CreateIpcObjectTest(ostream& os, istream& is)
{
	DWORD ret = ERROR_SUCCESS;
	string objName;

	if( is >> objName )
	{
		HANDLE& obj = g_objList[objName];

		if( obj = CreateIpcObject(NULL, objName.c_str(), 1000, 0, 0) )
			os << "IpcObject " << objName << " created: " << setdw << obj << '\n';
		else
			ret = GetLastError();
	}
	else
		cout << "Type the object name correctly.\n";

	return HRESULT_FROM_WIN32(ret);
}


/** Abre um objeto com o nome que é recebido de is. Imprime resultado em os. */
HRESULT OpenIpcObjectTest(ostream& os, istream& is)
{
	DWORD ret = ERROR_SUCCESS;
	string objName;

	if( is >> objName )
	{
		HANDLE& obj = g_objList[objName];

		if( obj = OpenIpcObject(objName.c_str()) )
			os << "IpcObject " << objName << " opened: " << setdw << obj << '\n';
		else
			ret = GetLastError();
	}
	else
		cout << "Type the object name correctly.\n";

	return HRESULT_FROM_WIN32(ret);
}


/** Fecha um objeto previamente criado ou aberto. O nome do resultado é 
impresso em os. */
HRESULT CloseIpcObjectTest(ostream& os, istream& is)
{
	DWORD ret = ERROR_SUCCESS;
	string objName;

	if( is >> objName )
	{
		HANDLE& obj = g_objList[objName];
		ret = CloseHandle(obj) ? ERROR_SUCCESS : GetLastError();
		obj = NULL;
	}
	else
		cout << "Type the object name correctly.\n";

	return HRESULT_FROM_WIN32(ret);
}


/** Imprime em os a lista de objetos criados ou abertos. */
HRESULT ListIpcObjectTest(ostream& os, istream& is)
{
	os << "ListIpcObjectTest\n";

	typedef IpcObjectList::const_iterator Iter;

	for( Iter i = g_objList.begin(), j = g_objList.end(); i != j; ++i )
		os << i->second << ' ' << i->first << '\n';

	return S_OK;
}


/** Envia uma mensagem de texto para um objeto. O nome do objeto é a primeira 
palavra obtida em is, e a mensagem é a primeira linha. */
HRESULT SendIpcObjectMessageTest(ostream& os, istream& is)
{
	DWORD ret = ERROR_SUCCESS;
	string objName;

	if( is >> objName )
	{
		string msg;

		if( getline(is, msg) )
		{
			HANDLE& obj = g_objList[objName];

			PSTR msgBuf = new CHAR[msg.size() + 1];
			lstrcpyA(msgBuf, msg.c_str() + 1);

			ret = SendIpcObjectMessage(obj, 
				msgBuf, (DWORD) msg.size() + 1,
				msgBuf, (DWORD) msg.size() + 1,
				IPCOBJTST_TIMEOUT);

			PrintErrorMessage(HRESULT_CODE(ret), os);
			
			if( ret == ERROR_SUCCESS )
				os << msgBuf << endl;
		}
		else
			cout << "Type the message correctly.\n";
	}
	else
		cout << "Type the object name correctly.\n";

	return HRESULT_FROM_WIN32(ret);
}


/** Estrutura da mensagem que é enviada para um objeto de teste. */
struct MessageContext
{
	HANDLE objH;
	string objName;
	ostream* pos;
	istream* pis;
};


/** Função que recebe mensagens de objetos de teste.

@see MessageContext.
*/
BOOL WINAPI MessageFunction(PVOID context, PVOID msgBuffer, PDWORD msgSize, DWORD bufferSize)
{
	BOOL ret = TRUE;
	MessageContext* msgCtx = static_cast<MessageContext*>( context );
	ostream& os = *msgCtx->pos;
	istream& is = *msgCtx->pis;
	PSTR msg = static_cast<PSTR>( msgBuffer );

	os << "Received message for object " << msgCtx->objName 
		<< "(" << setdw << msgCtx->objH << "):\n"
		<< msg << '\n';

	ostringstream oss;
	istringstream iss(msg);

	HRESULT hr = ParseCommands(oss, iss);

	string ossRet = oss.str();
	if( ossRet.size() + 1 <= bufferSize )
	{
		lstrcpyA(msg, ossRet.c_str());
		*msgSize = DWORD(ossRet.size() + 1);
	}
	else
	{
		msg[0] = 0;
		*msgSize = 1;
	}

	if( hr == S_FALSE )
		ret = FALSE;

	return ret;
}


/** Função que inicia o recebimento de mensagens de um objeto já criado. */
HRESULT ReceiveIpcObjectMessageTest(ostream& os, istream& is)
{
	DWORD ret = ERROR_SUCCESS;
	string objName;

	if( is >> objName )
	{
		HANDLE& obj = g_objList[objName];
		MessageContext msgCtx = { obj, objName, &os, &is };

		os << "Starting to receive messages from " << objName << '\n';
		ret = ReceiveIpcObjectMessage(obj, MessageFunction, &msgCtx, IPCOBJTST_TIMEOUT);
	}
	else
		cout << "Type the object name correctly.\n";

	return HRESULT_FROM_WIN32(ret);
}


/** Sai do programa de testes. */
HRESULT QuitIpcObjectTest(ostream& os, istream& is)
{
	os << "Exiting...\n";
	return S_FALSE;
}


/** Imprime descrição do sistema para o código de erro hr. */
void PrintErrorMessage(HRESULT hr, ostream& os)
{
	os << "ret: 0x" << setdw << hr;

	if( FAILED(hr) )
	{
		PSTR msgBuffer = NULL;

		DWORD err = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			(PSTR) &msgBuffer,
			0,
			NULL);

		if( err && msgBuffer )
		{
			os << ". " << msgBuffer;
			LocalFree(msgBuffer);
		}
		else
			os << endl;
	}
	else
		os << endl;
}


/** Interpreta comandos do stream de entrada e retorna os resultados
na stream de saída. */
HRESULT ParseCommands(ostream& os, istream& is)
{
	HRESULT hr = S_OK;
	string line;

	while( hr != S_FALSE && getline(is, line) )
	{
		string cmd;
		istringstream iss(line);
		iss >> cmd;

		hr = g_funcList[cmd](os, iss);
		PrintErrorMessage(hr, os);
	}

	return hr;
}


/** Atribui funções de teste aos seus respectivos nomes e inicia leitura em is
dos comandos do usuário. */
HRESULT StartIpcObjectTests(ostream& os, istream& is)
{
	g_funcList["create"] = CreateIpcObjectTest;
	g_funcList["open"] = OpenIpcObjectTest;
	g_funcList["receive"] = ReceiveIpcObjectMessageTest;
	g_funcList["send"] = SendIpcObjectMessageTest;
	g_funcList["close"] = CloseIpcObjectTest;
	g_funcList["list"] = ListIpcObjectTest;
	g_funcList["quit"] = QuitIpcObjectTest;

	HRESULT hr;

	while( (hr = ParseCommands(os, is)) == S_OK )
	{
		os << endl;
	}

	return hr;
}


/** No princípio, Deus disse: 'int main!'
*/
int main(int argc, char* argv[])
{
	return (int) StartIpcObjectTests(cout, cin);
}
