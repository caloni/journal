#define _CRT_SECURE_NO_WARNINGS // tranqueiras da microsoft
#include "SimpleString2.h"
#include <cstdio>
#include <cstring>

using namespace std;

size_t SimpleString2::ConstructCounter = 0;

void SimpleString2::ConstructSentence(const char* sentence)
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

void SimpleString2::MoveSentence(SimpleString2&& right)
{
    if( right.m_sentence )
    {
        m_sentence = right.m_sentence;
        right.m_sentence = nullptr;
    }
    else m_sentence = nullptr;
}

void SimpleString2::DestroySentence()
{
    if( m_sentence )
    {
        delete [] m_sentence;
        m_sentence = nullptr;
    }
}
