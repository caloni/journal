typedef map<string, Method> MethodList; // uma vari�vel desse tipo armazena todas as fun��es

void CommandPrompt::Interact(ostream& os, istream& is)
{
        while( is )
        {
                string func;
                vector<string> args;

                if( ParseLine(is, func, args) )
                {
                        // se a fun��o desejada est� em nossa lista,
                        // podemos cham�-la, mesmo sem conhecer qual �
                        if( Method m = m_funcs[func] )
                                m(args);
                }
        }
}
