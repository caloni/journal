#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <awklib/awk.h>


using namespace std;
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


string TransformHeader(string& content)
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
        ostringstream os;
        istringstream is(headerStr);
        while( is )
        {
            string field;
            is >> field;
            if( field.find("title:") == 0 )
            {
                getline(is, field);
                os << "=" << field << endl;
            }
        }
        content.erase(0, contentBegin);
        while( content.size() && content[0] == '\n' )
            content.erase(0, 1);
        string header = os.str();
        return header;
    }

    return "";
}


int main()
{
    ofstream ofs("content/blog.txt");

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
                    string header = TransformHeader(content);
                    ofs << "\n" << header << "\n" << content << endl;
                }
            }
        }
    }
}

