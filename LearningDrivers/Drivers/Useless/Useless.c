/** @file Useless.c
 *
 * @brief Projeto inicial de exemplo do blogue DriverEntry
 *
 * @author Fernando Roberto <fernando@driverentry.com.br>
 * @author Wanderley Caloni <wanderley@caloni.com.br>
 *
 * @date 2008-08
 */
#include <ntddk.h>


VOID OnDriverUnload(IN PDRIVER_OBJECT pDriverObject);


/** @brief Ponto de entrada do nosso driver.
 *
 * Tudo começa aqui, depois vai enrolando, enrolando, ...
 */
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, 
		IN PUNICODE_STRING pusRegistryPath)
{
	// Se houve um depurador atachado no sistema, pode-se ver a msg abaixo.
	DbgPrint("Cagamba, não é que funciona mesmo?\n");

	// Aqui informamos ao sistema que nosso driver é capaz de ser descarregado
	// dinamicamente e que a rotina de Callback que vai tratar a finalização de
	// tudo é a OnDriverUnload.
	pDriverObject->DriverUnload = OnDriverUnload;

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

