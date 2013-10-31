#include "provided.h"
#include "MyMap.h"
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

class SearcherImpl
{
public:
    vector<string> search(string terms);
    bool load(string filenameBase);
private:
    Indexer m_indexer;
};

bool compUrl(const UrlCount& uc1, const UrlCount& uc2)
{
    return uc1.url <= uc2.url;
}

bool compCount(const UrlCount& uc1, const UrlCount& uc2)
{
    return uc1.count >= uc2.count;
}

vector<string> SearcherImpl::search(string terms)
{
    MyMap<string, bool> m_term;
    int N = 0; //number of words in the terms string
    Tokenizer t(terms);
    string w;
    while(t.getNextToken(w))
    {
        N++;
        strToLower(w);
        m_term.associate(w, true);
    }
    
    int T = 0.7*N;
    if (T < 1)
        T = 1;
    
    vector<UrlCount> v;
    string key;
    bool* bp = m_term.getFirst(key);
    while (bp != NULL) // collect urlcount for each word in terms
    {
        vector<UrlCount> temp = m_indexer.getUrlCounts(key);
        for (vector<UrlCount>::iterator it = temp.begin();it != temp.end(); ++it)
            v.push_back(*it);
        bp = m_term.getNext(key);
    }
    sort(v.begin(),v.end(),compUrl);
    string url;
    int n = 0;
    int score = 0;
    vector<UrlCount> urlScore;
    for (size_t k = 0; k < v.size(); ++k)
    {
        if (url != v[k].url)
        {
            if (n >= T)
            {
                UrlCount uctemp;
                uctemp.url = url;
                uctemp.count = score;
                urlScore.push_back(uctemp);
            }
            
            url = v[k].url;
            n = 1;
            score = v[k].count;
        }
        else
        {
            score += v[k].count;
            n++;
        }
    }
    if (n >= T) // check last distinct url
    {
        UrlCount uctemp;
        uctemp.url = url;
        uctemp.count = score;
        urlScore.push_back(uctemp);
    }
    
    sort(urlScore.begin(),urlScore.end(),compCount);
    vector<string> result;
    for (size_t k = 0; k < urlScore.size(); ++k)
        result.push_back(urlScore[k].url);

    return result;
}

bool SearcherImpl::load(string filenameBase)
{
    return m_indexer.load(filenameBase);
}

//******************** Searcher functions *******************************

// These functions simply delegate to SearcherImpl's functions.
// You probably don't want to change any of this code.

Searcher::Searcher()
{
    m_impl = new SearcherImpl;
}

Searcher::~Searcher()
{
    delete m_impl;
}

vector<string> Searcher::search(string terms)
{
    return m_impl->search(terms);
}

bool Searcher::load(string filenameBase)
{
    return m_impl->load(filenameBase);
}
