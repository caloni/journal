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

string GetNextPost(string& content)
{
    size_t yamlBegin = content.find("---");
    size_t yamlEnd = content.find("\n---", yamlBegin);
    size_t nextYaml = content.find("\n---", yamlEnd + 1);
    bool valid = yamlBegin != content.npos
        && yamlEnd != content.npos
        && nextYaml != content.npos;

    if( valid )
    {
        string post = content.substr(yamlBegin, nextYaml);
        content.erase(0, nextYaml);
        return post;
    }
    else
    {
        string post = content;
        content.clear();
        return post;
    }
}

int main()
{
    fs::create_directories("public/blog_alpha");
    ifstream ifs("content/blog.txt");

    if (ifs)
    {
        string content = ReadEntireFile(ifs);
        int counter = 0;
        while (content.size())
        {
            string post = GetNextPost(content);
            string ppath = "public/blog_alpha/blog_entry_" + to_string(++counter) + ".md";
            ifstream pifs(ppath);
            if (pifs)
            {
                string postContent = ReadEntireFile(pifs);

                if( post != postContent )
                {
                    ofstream ofs(ppath);
                    if (ofs)
                    {
                        ofs << post;
                    }
                }
            }
            else
            {
                ofstream ofs(ppath);
                if (ofs)
                {
                    ofs << post;
                }
            }
        }
    }
}

