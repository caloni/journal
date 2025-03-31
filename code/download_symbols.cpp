#include <windows.h>
#include <dbghelp.h>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "dbghelp.lib")

namespace fs = std::filesystem;

int main()
{
    const fs::path RootPath = fs::path("resources");
    auto isBinary = [](const fs::path& path) { return path.extension() == ".exe" || path.extension() == ".dll" || path.extension() == ".sys"; };

    ::SymInitialize(::GetCurrentProcess(), NULL, FALSE);
    try
    {
        for (const auto& entry : fs::directory_iterator(RootPath))
        {
            std::cout << entry.path().string() << std::endl;
            try
            {
                if (entry.is_regular_file() && isBinary(entry.path()))
                {
                    auto imageName = entry.path().string();
                    SYMSRV_INDEX_INFO imageFileInfo = { sizeof(imageFileInfo) };
                    ::SymSrvGetFileIndexInfo(imageName.c_str(), &imageFileInfo, 0);
                    char downloadedImagePath[MAX_PATH + 1] = { 0 };
                    if (::SymFindFileInPath(GetCurrentProcess(), NULL, imageName.c_str(),
                        &imageFileInfo.timestamp, imageFileInfo.size, 0, SSRVOPT_DWORDPTR,
                        downloadedImagePath, nullptr, nullptr))
                    {
                        std::cout << "Image file downloaded to " << downloadedImagePath << std::endl;
                    }

                    char downloadedPdbPath[MAX_PATH + 1] = { 0 };
                    if (::SymFindFileInPath(GetCurrentProcess(), NULL, imageFileInfo.pdbfile,
                        &imageFileInfo.guid, imageFileInfo.age, 0, SSRVOPT_GUIDPTR,
                        downloadedPdbPath, nullptr, nullptr))
                    {
                        std::cout << "PDB file downloaded to " << downloadedPdbPath << std::endl;
                    }
                }
            }
            catch (std::exception& e)
            {
                std::cerr << entry.path().string() << ": " << e.what() << std::endl;
            }
        }
    }
    catch (std::filesystem::filesystem_error& e)
    {
        std::cout << "No resources directory found; nothing to do here: " << e.what();
    }
    ::SymCleanup(::GetCurrentProcess());
    return 0;
}

