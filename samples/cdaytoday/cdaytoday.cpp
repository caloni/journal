#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

using namespace std;


bool IsUTF8(const string& header)
{
    if (header.size() >= 3)
    {
        unsigned int c1 = (unsigned char) header[0];
        bool b1 = c1 == 0xef;
        unsigned int c2 = (unsigned char) header[1];
        bool b2 = c2 == 0xbb;
        unsigned int c3 = (unsigned char) header[2];
        bool b3 = c3 == 0xbf;

        bool ret = b1 && b2 && b3;
        return ret;
    }
    return false;
}


int main(int argc, char* argv[])
{
    vector<string> logs;

    ifstream ifs("c:\\daytoday\\daytoday.txt");
    string line;

    if (getline(ifs, line))
    {
        if ( IsUTF8(line) )
        {
            line = line.substr(3);

            do
            {
                logs.push_back(line);
            } 
            while (getline(ifs, line));
        }
    }

    int curr = logs.size();
    int dir = 0;
    cin >> dir;

    while (dir != 0)
    {
        curr += dir;
        int idx = curr % logs.size();
        string line = logs[idx];
        cout << line << endl;
        cin >> dir;
    }

    return 0;
}
