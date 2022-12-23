typedef void (Method*)(vector<string>& args);

class CommandPrompt
{
public:
        void Add(string name, Method m); // adiciona novo m�todo
        void Interact(ostream& os, istream& is); // come�a intera��o com usu�rio
};
