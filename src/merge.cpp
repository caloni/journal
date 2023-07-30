#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <awklib/awk.h>


using namespace std;
using namespace chrono;
namespace fs = std::filesystem;

struct PostHeader
{
    string date;
    string title;
    vector<string> categories;
    vector<string> tags;
    string link;
};


std::string ReadEntireFile(std::ifstream& in)
{
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}


istringstream g_is;
static int myinproc()
{
    return g_is.get();
}

void setfield(AWKINTERP *pinter, awksymb* ret, int nargs, awksymb* args)
{
}

PostHeader ReadYamlHeader(const string& content)
{
    PostHeader header;
    size_t yamlBegin = content.find("---\n");
    size_t yamlEnd = content.find("\n---\n", yamlBegin);
    bool valid = yamlBegin != content.npos
        && yamlEnd != content.npos;

    if( valid )
    {
        size_t headerBegin = yamlBegin + 4;
        size_t headerEnd = yamlEnd - headerBegin;
        string headerStr = content.substr(yamlBegin + 4, headerEnd);

        AWKINTERP* interp = awk_init(NULL);
        awk_setprog(interp, "/^title:/ {setfield($0)}");
        awk_compile(interp);
        g_is = istringstream(headerStr);
        awk_infunc(interp, myinproc);
        awk_addfunc(interp, "setfield", setfield, 1);
        awk_exec(interp);
        awksymb symb{};
        symb.name = "title";
        int res = awk_getvar(interp, &symb);
        awk_end(interp);
    }

    return header;
}


string TransformHeader(const string& slug, string& content)
{
    size_t yamlBegin = content.find("---\n");
    size_t yamlEnd = content.find("\n---\n", yamlBegin);
    bool valid = yamlBegin != content.npos
        && yamlEnd != content.npos;

    if( valid )
    {
        size_t contentBegin = yamlEnd + 5;
        size_t headerBegin = yamlBegin + 4;
        size_t headerEnd = yamlEnd - headerBegin;
        string headerStr = content.substr(yamlBegin + 4, headerEnd);
        headerStr = headerStr + "\nslug: " + slug;
        content = content.substr(contentBegin);
        return headerStr;
    }

    return "";
}


int main()
{
    auto start = high_resolution_clock::now();
    int postCount = 0;
    ofstream ofs("content/blog.md");

    if( ofs )
    {
        std::string path = "content";
        for (const auto& entry : fs::recursive_directory_iterator(path))
        {
            string path = entry.path().string();
            string dir = entry.path().parent_path().filename().string();
            if (path.find(".md") != path.npos)
            {
                ifstream ifs(path);

                if (ifs)
                {
                    string content = ReadEntireFile(ifs);
                    if( content.size() )
                    {
                        string header = TransformHeader(dir, content);
                        if (header.size())
                        {
                            ofs << "\n---\n" << header << "\n---\n" << content << endl;
                            ++postCount;
                        }
                    }
                }
            }
        }
    }

    auto end = high_resolution_clock::now();
    cout << postCount << " posts merged in " << duration_cast<seconds>(end - start).count() << " seconds\n";
}

