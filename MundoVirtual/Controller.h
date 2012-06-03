#include "CommandPrompt.h"
#include "Object.pb.h"
#include <string>
#include <list>
#include <map>
#include <iostream>


namespace MV
{

    class Controller
    {
    public:
        static int Start();

    protected:
        static std::string GetNextName();
        bool Version(std::ostream& os, const ArgumentList& args, std::string& error);
        bool Create(std::ostream& os, const ArgumentList& args, std::string& error);
        bool List(std::ostream& os, const ArgumentList& args, std::string& error);
        bool Set(std::ostream& os, const ArgumentList& args, std::string& error);
        bool Name(std::ostream& os, const ArgumentList& args, std::string& error);
        bool Property(std::ostream& os, const ArgumentList& args, std::string& error);
        bool Save(std::ostream& os, const ArgumentList& args, std::string& error);
        bool Load(std::ostream& os, const ArgumentList& args, std::string& error);

        void SetObject(Object* object);
        Object* GetObject(std::string name);

    private:
        Controller();

        Object m_parent;
        Object* m_object;
    };

}
