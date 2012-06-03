#include "Controller.h"
#include "CommandPrompt.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace MV;
using namespace std;


string Controller::GetNextName()
{
    static int st_nextId = 0;
    ostringstream os;
    os << "Objeto" << ++st_nextId;
    return os.str();
}


Controller::Controller() : m_object(&m_parent)
{
    m_parent.set_name("Universo");

    MV::Property* prop = m_parent.add_properties();
    prop->set_name("Tamanho");
    prop->set_value("grande");
}


Object* Controller::GetObject(string name)
{
    Object* ret = nullptr;

    for( int i = 0; i < m_object->objects_size(); ++i )
    {
        Object* obj = m_object->mutable_objects(i);

        if( obj->name() == name )
        {
            ret = obj;
            break;
        }
    }

    return ret;
}


void Controller::SetObject(Object* object)
{
    if( object )
        m_object = object;
    else
        m_object= &m_parent;
}


bool Controller::Version(ostream& os, const ArgumentList& args, string& error)
{
    os << "MundoVirtual v. " << __DATE__ << "\n";
    return true;
}


bool Controller::Create(ostream& os, const ArgumentList& args, string& error)
{
    string name = args.size() ? args[0] : GetNextName();
    Object* obj = m_object->add_objects();
    obj->set_name(name);
    os << name << " foi criado.\n";
    return true;
}


bool Controller::List(ostream& os, const ArgumentList& args, string& error)
{
    Object* obj = m_object;

    if( args.size() )
        if( Object* o = GetObject(args[0]) )
            obj = o;

    os << "O que " << obj->name() << " tem:\n";
    if( obj->objects_size() )
    {
        for( int i = 0; i < obj->objects_size(); ++i )
        {
            const Object& o = obj->objects(i);
            os << o.name() << "\n";
        }
    }
    else os << "Nada!\n";

    os << endl;

    os << "Como " << obj->name() << " se parece:\n";
    for( int i = 0; i < obj->properties_size(); ++i )
    {
        const MV::Property& prop = obj->properties(i);
        os << prop.name() << ": " << prop.value() << "\n";
    }
    os << endl;

    return true;
}


bool Controller::Set(ostream& os, const ArgumentList& args, string& error)
{
    SetObject(args.size() ? GetObject(args[0]) : nullptr);
    os << "Agora estamos mexendo com o objeto " << m_object->name() << endl;
    return true;
}


bool Controller::Name(ostream& os, const ArgumentList& args, string& error)
{
    bool ret = false;

    if( args.size() == 1 )
    {
        string oldName = m_object->name();
        m_object->set_name(args[0]);
        os << oldName << " agora se chama " << args[0] << ".\n";
    }
    else error = "Voce esqueceu de dizer o nome.";

    return ret;
}


bool Controller::Property(ostream& os, const ArgumentList& args, string& error)
{
    bool ret = false;

    if( args.size() == 2 )
    {
        MV::Property* prop = m_object->add_properties();
        prop->set_name(args[0]);
        prop->set_value(args[1]);

        os << "Agora " << m_object->name() << "\' tem \'" << args[0] << "\' igual a \'" << args[1] << "\'.\n";
        ret = true;
    }
    else error = "Qual o nome da propriedade e seu valor?";

    return ret;
}


bool Controller::Save(ostream& os, const ArgumentList& args, string& error)
{
    bool ret = false;

    ofstream ofs("MundoVirtual.dat");

    if( ofs )
    {
        if( m_parent.SerializeToOstream(&ofs) )
        {
            os << "Todos os objetos foram salvos com sucesso!\n";
            ret = true;
        }
        else error = "Erro juntando os objetos.\n";
    }
    else error = "Erro abrindo o arquivo.\n";

    return ret;
}


bool Controller::Load(ostream& os, const ArgumentList& args, string& error)
{
    bool ret = false;

    ifstream ifs("MundoVirtual.dat");

    if( ifs )
    {
        if( m_parent.ParseFromIstream(&ifs) )
        {
            os << "Todos os objetos foram abertos com sucesso!\n";
            ret = true;
        }
        else error = "Erro espalhando os objetos.\n";
    }
    else error = "Error abrindo o arquivo.\n";

    return ret;
}


int Controller::Start()
{
    static Controller st_singleton;

    CommandWrapper<Controller> cmds(&st_singleton);
    CommandPrompt prompt(&cmds);

    cmds.Add("versao", "Imprime versao do programa.", &Controller::Version);
    cmds.Add("criar", "Cria um novo objeto.", &Controller::Create);
    cmds.Add("listar", "Faz uma lista dos objetos.", &Controller::List);
    cmds.Add("objeto", "Qual o objeto atual?", &Controller::Set);
    cmds.Add("nome", "Qual o novo nome desse objeto?", &Controller::Name);
    cmds.Add("mudar", "Muda uma propriedade do objeto.", &Controller::Property);
    cmds.Add("salvar", "Salva todos os objetos para abrir depois.", &Controller::Save);
    cmds.Add("abrir", "Abre todos os objetos que voce salvou antes.", &Controller::Load);

    cout << "MundoVirtual v. " << __DATE__ << "\n\n";
    prompt.Interact(cin, cout);

    return 0;
}
