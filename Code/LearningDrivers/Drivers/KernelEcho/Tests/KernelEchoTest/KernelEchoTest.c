/** @file KernelEcho.c
 *
 * @brief Projeto que testa driver KernelEcho.
 * @author Wanderley Caloni <wanderley@caloni.com.br>
 * @date 2008-08
 */
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define BUFFER_SIZE 500
#define CMD_SIZE 50
#define ARGS_SIZE 450

/// @brief String que interpreta linhas digitadas.
/// @remark A string está estreitamente relacionada com os 
/// limites dos buffers CMD_SIZE e BUFFER_SIZE.
#define PARSE_STRING "%49s %449[^\n]"


DWORD ReadBuffer(HANDLE device, char* buffer)
{
	DWORD ret = ERROR_SUCCESS;
	DWORD read = 0;

	if( ! ReadFile(device, buffer, ARGS_SIZE, &read, NULL) )
	{
		ret = GetLastError();
		buffer[0] = 0;
	}

	return ret;
}


DWORD WriteBuffer(HANDLE device, const char* buffer)
{
	DWORD ret = ERROR_SUCCESS;
	DWORD written = 0;
	DWORD bufferSz = strlen(buffer) + 1;

	if( ! WriteFile(device, buffer, bufferSz, &written, NULL) )
		ret = GetLastError();

	return ret;
}


int main()
{
	DWORD ret = ERROR_SUCCESS;
	HANDLE device;

	device = CreateFile(_T("\\\\.\\EchoDevice"), GENERIC_ALL, 0, 
		NULL, OPEN_EXISTING, 0, NULL);

	if( device != INVALID_HANDLE_VALUE )
	{
		char line[BUFFER_SIZE] = "";
		char cmd[CMD_SIZE] = "";
		char args[ARGS_SIZE] = "";
		int argsN;

		printf("Success opening device!\n");
		printf("Type \"read something\" or \"write something\" or \"quit\":\n");

		while( 1 )
		{
			ret = ERROR_SUCCESS;
			fgets(line, sizeof(line), stdin);
			argsN = sscanf(line, PARSE_STRING, cmd, args);

			if( argsN >= 1 )
			{
				if( _strcmpi(cmd, "read") == 0 )
				{
					if( (ret = ReadBuffer(device, args)) == ERROR_SUCCESS )
						printf("%s\n", args);
				}
				else if( _strcmpi(cmd, "write") == 0 )
				{
					ret = WriteBuffer(device, args);
				}
				else if( _strcmpi(cmd, "quit") == 0 )
				{
					break;
				}
			}
			
			if( ret != ERROR_SUCCESS )
				printf("Error %08X\n", ret);
		}

		printf("Exiting... bye!\n");
		CloseHandle(device);
	}
	else
	{
		ret = GetLastError();
		printf("Error %08X trying to open EchoDevice.\n", ret);
	}

	return (int) ret;
}
