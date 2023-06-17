#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;


std::string ReadEntireFile(std::ifstream& in)
{
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}


int main()
{
    ofstream ofs("blog.txt");

    if( ofs )
    {
        std::string path = ".";
        for (const auto& entry : fs::directory_iterator(path))
        {
            string path = entry.path().string();
            ifstream ifs(path);

            if (ifs)
            {
                string content = ReadEntireFile(ifs);
                ofs << "\n" << content << endl;
            }
        }
    }
}

