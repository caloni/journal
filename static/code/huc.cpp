#include <windows.h>

#include <string>
#include <vector>
#include <thread>


using namespace std;


struct WriteJob
{
  string inPath;
  string outPath;
  vector<string> files;
};


void Writer(const WriteJob& job)
{
  for( const string& file: job.files )
  {
    HANDLE inF, outF;
    string inFile = job.inPath + "\\" + file;
    string outFile = job.outPath + "\\" + file;
    string fileBuffer;

    inF = CreateFileA(inFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    outF = CreateFileA(outFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

    if (inF != INVALID_HANDLE_VALUE && outF != INVALID_HANDLE_VALUE)
    {
      DWORD readBytes = 0;

      DWORD fileSz = GetFileSize(inF, NULL);
      fileBuffer.reserve(fileSz);

      if (ReadFile(inF, (PVOID) fileBuffer.data(), fileSz, &readBytes, NULL))
      {
        DWORD writtenBytes = 0;
        WriteFile(outF, fileBuffer.data(), readBytes, &writtenBytes, NULL);
      }
    }

    if (inF != INVALID_HANDLE_VALUE)
      CloseHandle(inF);

    if (outF != INVALID_HANDLE_VALUE)
      CloseHandle(outF);
  }
}


int main(int argc, char* argv[])
{
  if( argc == 3 )
  {
    WriteJob job = { argv[1], argv[2] };
    vector<thread> jobs;
    string filesInFind = job.inPath + "\\*.md";

    WIN32_FIND_DATAA findData;
    HANDLE findH = FindFirstFileA(filesInFind.c_str(), &findData);

    if( findH != INVALID_HANDLE_VALUE )
    {
      do
      {
        job.files.push_back(findData.cFileName);

        if( job.files.size() > 100 )
        {
          jobs.push_back(thread(Writer, job));
          job.files.clear();
        }
      }
      while( FindNextFileA(findH, &findData) );

      FindClose(findH);
    }

    for( auto& j: jobs )
    {
      j.join();
    }
  }
}

