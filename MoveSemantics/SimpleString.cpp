#define _CRT_SECURE_NO_WARNINGS // tranqueiras da microsoft
#include "SimpleString.h"
#include <cstdio>
#include <cstring>

using namespace std;

size_t SimpleString::ConstructCounter = 0;

void SimpleString::ConstructSentence(const char* sentence)
{
    if( sentence )
    {
        ConstructCounter++;
        size_t sz = strlen(sentence);
        m_sentence = new char[sz + 1];
        strcpy(m_sentence, sentence);
    }
    else m_sentence = nullptr;
}

void SimpleString::DestroySentence()
{
    if( m_sentence )
    {
        delete [] m_sentence;
        m_sentence = nullptr;
    }
}
