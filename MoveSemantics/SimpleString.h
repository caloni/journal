
class SimpleString
{
public:
    SimpleString() { ConstructSentence(nullptr); }

    SimpleString(const char* sentence) { ConstructSentence(sentence); }

    SimpleString(const SimpleString& right) { ConstructSentence(right.m_sentence); }

    SimpleString& operator = (const SimpleString& right) {
        DestroySentence();
        ConstructSentence(right.m_sentence);
        return *this;
    }

    ~SimpleString() { DestroySentence(); }

    static size_t ConstructCounter;

private:
    void ConstructSentence(const char* sentence);
    void DestroySentence();

    char* m_sentence;
};

