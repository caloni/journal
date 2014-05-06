#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <iphlpapi.h>
#include <iostream>

#pragma comment(lib, "iphlpapi.lib")


using namespace std;


int main()
{
	DWORD adaptInfoSize = 0;
	DWORD err = GetAdaptersInfo(NULL, &adaptInfoSize);

	if( err == ERROR_BUFFER_OVERFLOW )
	{
		PIP_ADAPTER_INFO adaptInfo = (PIP_ADAPTER_INFO) new BYTE[adaptInfoSize];
		DWORD err = GetAdaptersInfo(adaptInfo, &adaptInfoSize);

		if( err == ERROR_SUCCESS )
		{
			PIP_ADAPTER_INFO currAdaptInfo = adaptInfo;

			while( currAdaptInfo )
			{
				char buf[100];

				if( currAdaptInfo->Type == MIB_IF_TYPE_ETHERNET )
				{
					// Primeiro endereço IP
					if( currAdaptInfo->IpAddressList.IpAddress.String )
						cout << currAdaptInfo->IpAddressList.IpAddress.String << '\n';

					// Endereço MAC
					if( currAdaptInfo->AddressLength == 6 ) // MAC Address
					{
						sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", 
							currAdaptInfo->Address[0], currAdaptInfo->Address[1],
							currAdaptInfo->Address[2], currAdaptInfo->Address[3],
							currAdaptInfo->Address[4], currAdaptInfo->Address[5]);
						cout << buf << '\n';
					}

					// Descrição
					cout << currAdaptInfo->Description << '\n';

					// Máscara de sub-rede
					if( currAdaptInfo->IpAddressList.IpMask.String )
						cout << currAdaptInfo->IpAddressList.IpMask.String << '\n';

					// Gateway padrão
					if( currAdaptInfo->GatewayList.IpAddress.String )
						cout << currAdaptInfo->GatewayList.IpAddress.String << '\n';

					// Servidores DNS
					// ???

					// DHCP Ativado
					if( currAdaptInfo->DhcpEnabled )
						cout << "1\n";
					else
						cout << "0\n";

					currAdaptInfo = currAdaptInfo->Next;
				}
			}
		}

		delete [] (PBYTE) adaptInfo;
	}
}
