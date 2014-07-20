#include <iostream>
#include <oledb.h>
#include <msdasc.h>

using namespace std;


int main()
{
	const wchar_t* szInitializationString = L"Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;User ID=SANDY\\Caloni;Initial Catalog=Scua;Data Source=SANDY\\SQLEXPRESS";

	CoInitialize(NULL);

	IDataInitialize* pIDataInitialize = NULL;
	HRESULT hr = CoCreateInstance(
		__uuidof(MSDAINITIALIZE), NULL, CLSCTX_ALL,
		__uuidof(IDataInitialize), (void**)&pIDataInitialize);

	IDBInitialize* pIDBInitialize = NULL;
	hr = pIDataInitialize->GetDataSource(NULL, CLSCTX_ALL,
		szInitializationString,
		__uuidof(IDBInitialize), (IUnknown**)&pIDBInitialize);
	hr = pIDBInitialize->Initialize();

	IDBCreateSession* pIDBCreateSession = NULL;
	hr = pIDBInitialize->QueryInterface(
		IID_IDBCreateSession, (void**)&pIDBCreateSession);

	IDBCreateCommand* pIDBCreateCommand = NULL;
	hr = pIDBCreateSession->CreateSession(NULL,
		IID_IDBCreateCommand,
		(IUnknown**)&pIDBCreateCommand);

	ICommandText* pICommandText = NULL;
	hr = pIDBCreateCommand->CreateCommand(NULL,
		IID_ICommandText, (IUnknown**)&pICommandText);

	hr = pICommandText->SetCommandText(DBGUID_SQL, OLESTR("insert into sec_auditoria_monitor_log_tmp values(1, 1, 1, 1, '2014-07-12', 'user', 'proc', 'status', 'path', 'evt', 'wks', 'net', 'orig', 'acao');"));

	hr = pICommandText->Execute(NULL, IID_NULL, NULL, NULL, NULL);

	pICommandText->Release();

	pIDBCreateSession->Release();
	pIDBCreateCommand->Release();
	pIDBInitialize->Release();
	pIDataInitialize->Release();
}
