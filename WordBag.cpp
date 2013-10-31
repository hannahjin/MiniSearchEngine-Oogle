#include "provided.h"
#include "MyMap.h"
#include <string>
using namespace std;

class WordBagImpl
{
public:
    WordBagImpl(const string& text);
    bool getFirstWord(string& word, int& count);
    bool getNextWord(string& word, int& count);
private:
    MyMap<string, int>* m_token;
};

WordBagImpl::WordBagImpl(const string& text)
{
    m_token = new MyMap<string, int>;
    Tokenizer t(text);
    string w;
    while(t.getNextToken(w))
    {
        strToLower(w);
        int* c = m_token->find(w);
        if (c == NULL)
            m_token->associate(w, 1);
        else
            *c += 1;
    }
}

bool WordBagImpl::getFirstWord(string& word, int& count)
{
    string key;
    int* c = m_token->getFirst(key);
    if (c == NULL)
        return false;
    else
    {
        word = key;
        count = *c;
        return true;
    }
}

bool WordBagImpl::getNextWord(string& word, int& count)
{
    string key;
    int* c = m_token->getNext(key);
    if (c == NULL)
        return false;
    else
    {
        word = key;
        count = *c;
        return true;
    }
}

//******************** WordBag functions *******************************

// These functions simply delegate to WordBagImpl's functions.
// You probably don't want to change any of this code.

WordBag::WordBag(const std::string& text)
{
    m_impl = new WordBagImpl(text);
}

WordBag::~WordBag()
{
    delete m_impl;
}

bool WordBag::getFirstWord(string& word, int& count)
{
    return m_impl->getFirstWord(word, count);
}

bool WordBag::getNextWord(string& word, int& count)
{
    return m_impl->getNextWord(word, count);
}
