#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <chrono>


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


int main(int argc, char* argv[])
{
    auto start = high_resolution_clock::now();
    int postCount = 0;
    int fileCount = 0;
    bool copyImages = false;
    ofstream ofs("content/blog.md");

    for( int i = 1; i < argc; ++i )
    {
        if( strcmp(argv[i], "--copy-images") == 0 )
        {
            cout << "Copy images enabled\n";
            copyImages = true;
        }
    }

    if( ofs )
    {
        std::string path = "content";
        for (const auto& entry : fs::recursive_directory_iterator(path))
        {
            string path = entry.path().string();
            string file = entry.path().filename().string();
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
            else if (path.find(".png") != path.npos 
                || path.find(".jpg") != path.npos 
                || path.find(".gif") != path.npos )
            {
                if (copyImages)
                {
                    fs::path dest("public/blog_awk/img/" + dir + "-" + file);
                    fs::copy_file(entry.path(), dest);
                    ++fileCount;
                }
            }
        }
    }

    auto end = high_resolution_clock::now();
    cout << postCount << " posts merged and " << fileCount << " files copied in " 
        << duration_cast<seconds>(end - start).count() << " seconds\n";
}

