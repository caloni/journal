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
 * Tudo come�a aqui, depois vai enrolando, enrolando, ...
 */
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, 
		IN PUNICODE_STRING pusRegistryPath)
{
	// Se houve um depurador atachado no sistema, pode-se ver a msg abaixo.
	DbgPrint("Cagamba, n�o � que funciona mesmo?\n");

	// Aqui informamos ao sistema que nosso driver � capaz de ser descarregado
	// dinamicamente e que a rotina de Callback que vai tratar a finaliza��o de
	// tudo � a OnDriverUnload.
	pDriverObject->DriverUnload = OnDriverUnload;

	// Ufa, conseguimos chegar at� aqui, isso merece um retorno de sucesso.
	return STATUS_SUCCESS;
}


/** @brief Fun��o de callback que trata das finaliza��es necess�rias.
 */
VOID OnDriverUnload(IN PDRIVER_OBJECT pDriverObject)
{
	// Se houver um depurador atachado no sistema, pode-se ver a msg abaixo.
	DbgPrint("Mas j�? Eu nem fiz nada...\n");
}

