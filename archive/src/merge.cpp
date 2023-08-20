#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>


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


string GetDateFromHeader(const string& header)
{
    string date = "";
    size_t label = header.find("date:");
    if( label != header.npos )
    {
        size_t eol = header.find('\n', label);
        if( eol != header.npos )
        {
            size_t fieldBegin = label + 6;
            size_t fieldSize = eol - fieldBegin;
            date = header.substr(fieldBegin, fieldSize);
            if( date[0] == '\'' || date[0] == '\"' )
            {
                date.erase(0, 1);
            }
            date.resize(10);
        }
    }
    return date;
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
        string path = "content";
        map<string, string> postsByDate;
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
                            string date = GetDateFromHeader(header);
                            string post = "\n---\n" + header + "\n---\n" + content;
                            postsByDate[date] = postsByDate[date] + "\n" + post;
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

        for( const auto& post: postsByDate )
        {
            ofs << post.second;
        }
    }

    auto end = high_resolution_clock::now();
    cout << postCount << " posts merged and " << fileCount << " files copied in " 
        << duration_cast<seconds>(end - start).count() << " seconds\n";
}

