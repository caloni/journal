#include <windows.h>
#include <tchar.h>

#include <stdio.h>


DWORD Backup(PCTSTR in, PCTSTR out)
{
	DWORD ret = ERROR_SUCCESS;
	HANDLE fOut = CreateFile(out, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);

	if( fOut )
	{
		HANDLE fIn = CreateFile(in, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

		if( fIn )
		{
			BYTE buf[0x1000];
			DWORD read;

			while( ReadFile(fIn, buf, sizeof(buf), &read, NULL) && read )
			{
				DWORD written = 0;

				if( ! WriteFile(fOut, buf, read, &written, NULL) || written != read )
				{
					printf("Error writing to backup file.\n");
					break;
				}
				
				printf(".");
			}

			CloseHandle(fIn);
		}
		else
		{
			ret = GetLastError();
			printf("Error %d opening input device.\n", int(ret));
		}

		CloseHandle(fOut);
	}
	else
	{
		ret = GetLastError();
		printf("Error %d opening output device.\n", int(ret));
	}

	return ret;
}


int _tmain(int argc, PTSTR argv[])
{
	if( argc == 3 )
		return (int) Backup(argv[1], argv[2]);
	else
		printf("Usage: rawcopy.exe inputDevice outputDevice\n");
		
	return -1;
}
