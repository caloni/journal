#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <list>


namespace MV
{

    /// Lista de argumentos de um comando.
    typedef std::vector<std::string> ArgumentList;


    /// Interface que contém uma lista de comandos para serem chamados.
    class ICommandList
    {
    public:
        virtual bool Call(const std::string& commandName, std::ostream& os, const ArgumentList& args, std::string& error) = 0;
        virtual void Help(const std::string& commandName, std::ostream& os) = 0;
    };


    /// Implementação de uma lista de comandos baseada em uma classe. 
    template<typename Class>
    class CommandWrapper : public ICommandList
    {
    public:
        CommandWrapper(Class* cls) : m_class(cls)
        {
        }
        
        typedef bool (Class::* Method)(std::ostream& os, const ArgumentList& args, std::string& error);
        
        void Add(const std::string& commandName, const std::string& description, Method command)
        {
            m_names.push_back(commandName);
            m_methods[commandName] = command;
            m_description[commandName] = description;
        }

        void Add(const std::string& commandName, const std::string& description, const std::string& help, Method command)
        {
            m_names.push_back(commandName);
            m_methods[commandName] = command;
            m_description[commandName] = description;
            m_help[commandName] = help;
        }

        bool Call(const std::string& commandName, std::ostream& os, const ArgumentList& args, std::string& error)
        {
            bool ret = false;
            if( Method m = m_methods[commandName] )
                ret = (m_class->*m)(os, args, error);
            else
                error = ("Command not found");
            os << std::endl;
            return ret;
        }

        void Help(const std::string& commandName, std::ostream& os)
        {
            if( commandName.size() )
            {
                HelpMap::const_iterator help = m_help.find(commandName);
                if( help != m_help.end() && help->second.size() )
                    os << m_description[help->first] << "\n" << help->second << "\n";
                else os << "Help not found" << std::endl;
            }
            else
            {
                for( MethodList::const_iterator names = m_names.begin(); names != m_names.end(); ++names )
                {
                    HelpMap::const_iterator desc = m_description.find(*names);

                    os << *names;
                    if( desc != m_description.end() )
                        os << " - " << desc->second;
                    os << '\n';
                }
            }
        }

    private:
        typedef std::list<std::string> MethodList;
        typedef std::map<std::string, Method> MethodMap;
        typedef std::map<std::string, std::string> HelpMap;

        Class* m_class;
        MethodList m_names;
        MethodMap m_methods;
        HelpMap m_description;
        HelpMap m_help;
    };


    /// Interfaceador de comandos.
    class CommandPrompt
    {
    public:
        CommandPrompt(ICommandList* commands);

        void Interact(std::istream& is, std::ostream& os);
        void Batch(std::istream& is, std::ostream& os, bool echo);
        void Help(std::ostream& os, const std::string& commandName = "");
        void Run(std::ostream& os, const std::string& fileName);

    private:
        bool GetNextArg(std::istream& is, std::string& arg);

        ICommandList* m_commands;
    };

}
