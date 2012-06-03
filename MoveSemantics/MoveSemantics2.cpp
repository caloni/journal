#include "MoveSemantics.h"
#include "SimpleString.h"

using namespace std;

SimpleString GetAnswer(const SimpleString& question)
{
    SimpleString answer("42");
    return answer;
}

void MoveSemantics2()
{
    SimpleString::ConstructCounter = 0;

    SimpleString question("A resposta para a Vida, o Universo e tudo mais.");
    SimpleString answer;
    cout << "Construimos ate agora " << SimpleString::ConstructCounter << " instancia(s)\n";

    answer = GetAnswer(question); // EVIL!!!
    cout << "Construimos ate agora " << SimpleString::ConstructCounter << " instancia(s)\n";
}
