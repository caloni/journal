#include "MoveSemantics.h"
#include "SimpleString.h"

using namespace std;

vector<SimpleString> GetAnswers(const SimpleString& question)
{
    vector<SimpleString> answers;
    answers.push_back("42");
    answers.push_back("What?");
    answers.push_back("NDA");
    return answers;
}

vector<SimpleString> GetLotOfAnswers(const SimpleString& question)
{
    vector<SimpleString> answers;
    for( size_t i = 0; i < 1000; ++i )
        answers.push_back("42");
    return answers;
}

void MoveSemantics3()
{
    SimpleString::ConstructCounter = 0;

    SimpleString question("A resposta para a Vida, o Universo e tudo mais.");
    vector<SimpleString> answers;
    cout << "Construimos ate agora " << SimpleString::ConstructCounter << " instancia(s)\n";

    answers.push_back(SimpleString());
    answers.push_back(SimpleString());
    answers.push_back(SimpleString());
    cout << "Construimos ate agora " << SimpleString::ConstructCounter << " instancia(s)\n";

    answers = GetAnswers(question); // EVIL!!!
    cout << "Construimos ate agora " << SimpleString::ConstructCounter << " instancia(s)\n";

    SimpleString::ConstructCounter = 0;

    answers = GetLotOfAnswers(question); // MORE EVIL!!!
    cout << "Construimos ate agora " << SimpleString::ConstructCounter << " instancia(s)\n";
}
