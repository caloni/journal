#include <algorithm>

class SimpleString2
{
public:
    SimpleString2() { ConstructSentence(nullptr); }

    SimpleString2(const char* sentence) { ConstructSentence(sentence); }

    SimpleString2(const SimpleString2& right) { ConstructSentence(right.m_sentence); }

    SimpleString2(SimpleString2&& right) { MoveSentence(std::move(right)); }

    SimpleString2& operator = (const SimpleString2& right) {
        DestroySentence();
        ConstructSentence(right.m_sentence);
        return *this;
    }

    SimpleString2& operator = (SimpleString2&& right) {
        DestroySentence();
        MoveSentence(std::move(right));
        return *this;
    }

    ~SimpleString2() { DestroySentence(); }

    static size_t ConstructCounter;

private:
    void ConstructSentence(const char* sentence);
    void MoveSentence(SimpleString2&& right);
    void DestroySentence();

    char* m_sentence;
};

