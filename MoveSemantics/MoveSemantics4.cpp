#include "MoveSemantics.h"
#include "SimpleString2.h"

using namespace std;

vector<SimpleString2> GetAnswers(const SimpleString2& question)
{
    vector<SimpleString2> answers;
    answers.push_back("42");
    answers.push_back("What?");
    answers.push_back("NDA");
    return answers;
}

vector<SimpleString2> GetLotOfAnswers(const SimpleString2& question)
{
    vector<SimpleString2> answers;
    for( size_t i = 0; i < 1000; ++i )
        answers.push_back("42");
    return answers;
}

void MoveSemantics4()
{
    SimpleString2::ConstructCounter = 0;

    SimpleString2 question("A resposta para a Vida, o Universo e tudo mais.");
    vector<SimpleString2> answers;
    cout << "Construimos ate agora " << SimpleString2::ConstructCounter << " instancia(s)\n";

    answers.push_back(SimpleString2());
    answers.push_back(SimpleString2());
    answers.push_back(SimpleString2());
    cout << "Construimos ate agora " << SimpleString2::ConstructCounter << " instancia(s)\n";

    answers = GetAnswers(question); // EVIL!!!
    cout << "Construimos ate agora " << SimpleString2::ConstructCounter << " instancia(s)\n";

    SimpleString2::ConstructCounter = 0;

    answers = GetLotOfAnswers(question); // MORE EVIL!!!
    cout << "Construimos ate agora " << SimpleString2::ConstructCounter << " instancia(s)\n";
}
