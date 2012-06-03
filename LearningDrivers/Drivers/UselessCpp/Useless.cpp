/** @file Useless.c
 *
 * @brief Projeto inicial de exemplo do blogue DriverEntry
 *
 * @author Fernando Roberto <fernando@driverentry.com.br>
 * @author Wanderley Caloni <wanderley@caloni.com.br>
 *
 * @date 2008-08
 */
extern "C"
{
	#include <ntddk.h>
}

#include <HtsCpp.h>


VOID OnDriverUnload(IN PDRIVER_OBJECT pDriverObject);


//-f--> Uma classe com um nome bem original
class MyClass
{
public:
    MyClass()
    {
        DbgPrint("Construtor %p\n", this);
    }
 
    ~MyClass()
    {
        DbgPrint("Destrutor %p\n", this);
    }
 
    VOID SayHello(VOID)
    {
        DbgPrint("Hello from %p\n", this);
    }
};



//
// Testando objetos globais.
//
MyClass g_myGlobalClass;



/** @brief Ponto de entrada do nosso driver.
 *
 * @remark Essa função é equivalente à DriverEntry.
 *
 * Tudo começa aqui, depois vai enrolando, enrolando, ...
 */
extern "C"
CPP_DRIVER_ENTRY(IN PDRIVER_OBJECT pDriverObject, 
		IN PUNICODE_STRING pusRegistryPath)
{
	MyClass Instance;

	// Se houve um depurador atachado no sistema, pode-se ver a msg abaixo.
	DbgPrint("Cagamba, não é que funciona mesmo?\n");

	Instance.SayHello();


	MyClass* pInstance = new(NonPagedPool) MyClass();

	// Aqui informamos ao sistema que nosso driver é capaz de ser descarregado
	// dinamicamente e que a rotina de Callback que vai tratar a finalização de
	// tudo é a OnDriverUnload.
	pDriverObject->DriverUnload = OnDriverUnload;

	pInstance->SayHello();

	delete pInstance;

	// Ufa, conseguimos chegar até aqui, isso merece um retorno de sucesso.
	return STATUS_SUCCESS;
}


/** @brief Função de callback que trata das finalizações necessárias.
 */
VOID OnDriverUnload(IN PDRIVER_OBJECT pDriverObject)
{
	// Se houver um depurador atachado no sistema, pode-se ver a msg abaixo.
	DbgPrint("Mas já? Eu nem fiz nada...\n");
}

