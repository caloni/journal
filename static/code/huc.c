#include <windows.h>

int main(int argc, char* argv[])
{
  if( argc == 3 )
  {
    PSTR filesIn = argv[1];
    PSTR filesOut = argv[2];
    CHAR filesInFind[MAX_PATH];
    WIN32_FIND_DATAA findData;
    HANDLE findH;

    strcpy(filesInFind, filesIn);
    strcat(filesInFind, "\\*.md");

    if( (findH = FindFirstFileA(filesInFind, &findData)) != INVALID_HANDLE_VALUE )
    {
      BOOL writeOk = FALSE;
      CHAR inPath[MAX_PATH];
      CHAR outPath[MAX_PATH];
      HANDLE inF, outF;
      CHAR fileBuffer[10 * 1024];
      DWORD fileSz;

      do
      {
        strcpy(inPath, filesIn);
        strcat(inPath, "\\");
        strcat(inPath, findData.cFileName);

        strcpy(outPath, filesOut);
        strcat(outPath, "\\");
        strcat(outPath, findData.cFileName);

        inF = CreateFileA(inPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
        outF = CreateFileA(outPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

        if( inF != INVALID_HANDLE_VALUE && outF != INVALID_HANDLE_VALUE )
        {
          DWORD readBytes = 0;

          fileSz = GetFileSize(inF, NULL);

          if( ReadFile(inF, fileBuffer, fileSz, &readBytes, NULL) )
          {
            DWORD writtenBytes = 0;
            writeOk = WriteFile(outF, fileBuffer, readBytes, &writtenBytes, NULL);
          }
        }

        if( inF != INVALID_HANDLE_VALUE )
          CloseHandle(inF);

        if( outF != INVALID_HANDLE_VALUE )
          CloseHandle(outF);
      }
      while( FindNextFileA(findH, &findData) );

      FindClose(findH);
    }
  }
}

