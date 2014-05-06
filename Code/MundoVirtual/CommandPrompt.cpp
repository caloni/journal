#include "CommandPrompt.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
using namespace MV;


CommandPrompt::CommandPrompt(ICommandList* commands) : m_commands(commands)
{
}


void CommandPrompt::Help(ostream& os, const string& commandName)
{
    if( commandName.empty() )
    {
        m_commands->Help(commandName, os);
        os << "\n";
        os << "! - Executes a previously saved script file\n";
        os << "# - Just to comment scripts; just printed in output\n";
        os << "? - This list or help for a command (help <command>)\n";
        os << "/ - Quits the aplication\n";
    }
    else
    {
        if( commandName == "?" )
        {
            os << "Prints command list or help for a command (help <command>)\n"
                << "How to use: help [<command>]\n";
        }
        else if( commandName == "/" )
        {
            os << "Quits the aplication\n";
        }
        else if( commandName == "!" )
        {
            os << "Executes a previously saved script file\n"
                << "How to use: run <file-name>\n";
         }
        else
        {
            m_commands->Help(commandName, os);
        }
    }
    os << endl;
}


void CommandPrompt::Run(ostream& os, const string& fileName)
{
    ifstream is(fileName.c_str());

    if( is )
    {
        Batch(is, os, true);
        os << endl;
    }
}


bool CommandPrompt::GetNextArg(std::istream& is, std::string& arg)
{
    bool ret;

    arg.clear();
    is >> arg;
    ret = arg.empty() == false;

    if( ret && arg[0] == '\"' )
    {
        arg.erase(0, 1);
        size_t fnd = arg.find('\"', 0);

        if( fnd == string::npos )
        {
            char c;
            
            while( is.get(c) )
            {
                if( c == '\"' )
                    break;
                arg += c;
            }
        }
        else if( fnd == arg.size() - 1 )
        {
            arg.erase(arg.size() - 1);
        }
    }

    return ret;
}


void CommandPrompt::Interact(istream& is, ostream& os)
{
    string line;
    Help(os);
    Batch(is, os, false);
}


void CommandPrompt::Batch(istream& is, ostream& os, bool echo)
{
    string line;

    while (getline(is, line))
    {
        string commandName;
        istringstream iss(line);

        if( echo )
            os << line << endl;

        if( GetNextArg(iss, commandName) )
        {
            if (commandName == "/" )
                break;
            else if (commandName == "?")
            {
                string arg;
                GetNextArg(iss, arg);
                Help(os, arg);
            }
            else if( commandName == "!" )
            {
                string fileName;
                
                if( GetNextArg(iss, fileName) )
                {
                    Run(os, fileName);
                }
            }
            else if( commandName == "#" )
            {
            }
            else
            {
                string error;
                string arg;
                ArgumentList args;

                while( GetNextArg(iss, arg) )
                    args.push_back(arg);

                if( ! m_commands->Call(commandName, os, args, error) )
                {
                    os << error << "\n\n";
                }
            }
        }
    }
}
