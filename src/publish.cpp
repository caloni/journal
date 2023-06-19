#include "constants.h"
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
    size_t postBegin = content.find("\n= ");
    size_t nextPost = content.find("\n= ", postBegin + 1);
    bool valid = postBegin != content.npos
        && nextPost != content.npos;

    if( valid )
    {
        string post = content.substr(postBegin, nextPost);
        content.erase(0, nextPost);
        return post;
    }
    else
    {
        string post = content;
        content.clear();
        return post;
    }
}

std::vector<std::string> GetPostLines(const std::string& str)
{
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{ str };

    for (std::string line; std::getline(ss, line, '\n');)
        result.push_back(line);

    return result;
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
            ostringstream os;
            string post = GetNextPost(content);
            vector<string> postLines = GetPostLines(post);
            os << HTML_HEAD << endl
                << HTML_BODY_TOP << endl;
            for( string& line: postLines )
            {
                if( line.size() > 2 && line[0] != '=' && line[1] != ' ' )
                {
                    os << "<p>" << line << "</p>" << endl;
                }
            }
            os << HTML_BODY_BOTTOM << endl;
            post = os.str();

            string ppath = "public/blog_alpha/blog_entry_" + to_string(++counter) + ".html";
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

