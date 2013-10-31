#include "provided.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

class IndexerImpl
{
public:
    // IndexerImpl();
    bool incorporate(string url, WordBag& wb);
    vector<UrlCount> getUrlCounts(string word);
    bool save(string filenameBase);
    bool load(string filenameBase);
private:
    MyMap<string,vector<int> > wordToIdCounts;
    MyMap<string, int> urlToId;
    MyMap<int,string> idToUrl;
};

bool IndexerImpl::incorporate(string url, WordBag& wb)
{
    // has url been incorporated before
    int* ip = urlToId.find(url);
    if (ip != NULL)
        return false;
    
    int id;
    string*  tempsp = NULL;
    do {
        id = rand();
        tempsp = idToUrl.find(id);
    } while (tempsp != NULL);
    
    urlToId.associate(url, id);
    idToUrl.associate(id, url);
        // add map each word and its count from wb to a url
    string word;
    int count;
    bool gotWord = wb.getFirstWord(word, count);
    while (gotWord)
    {
        vector<int>* ptr = wordToIdCounts.find(word);
        if (ptr == NULL) // add new association
        {
            vector<int> v;
            v.push_back(id);
            v.push_back(count);
            wordToIdCounts.associate(word,v);
        }
        else  // add a idcount to the vector of the corresponding word
        {
            ptr->push_back(id);
            ptr->push_back(count);
        }
        
        gotWord = wb.getNextWord(word,count);
    }
    return true;
}

vector<UrlCount> IndexerImpl::getUrlCounts(string word)
{
    strToLower(word);
    vector<int>* ptr = wordToIdCounts.find(word);
    vector<UrlCount> result;
    if (ptr != NULL)
    {
        for (vector<int>::iterator it = ptr->begin(); it != ptr->end(); it++)
        {
            string* urlptr = idToUrl.find(*it);
            if (urlptr != NULL)
            {
                UrlCount uc;
                uc.url = *urlptr;
                uc.count = *(++it);
                result.push_back(uc);
            }
            else ++it;
        }
    }
    return result;
}

void writeItem(ostream& stream, const string& s)
{
    stream << s << endl;
}

void writeItem(ostream& stream, const int& i)
{
    stream << i << endl;
}

void writeItem(ostream& stream, const vector<int> iv)
{
    writeItem(stream, static_cast<int>(iv.size()) );
    for (vector<int>::const_iterator it = iv.begin(); it != iv.end(); it++)
    {
        writeItem(stream, *it);
    }
}

template<typename KeyType, typename ValueType>
bool saveMyMap(string filename, MyMap<KeyType,ValueType>& m)
{
    ofstream outfile(filename.c_str());
    if (! outfile)
        return false;
    writeItem(outfile, m.size());
    KeyType key;
    ValueType* vptr= m.getFirst(key);
    while (vptr != NULL)
    {
        writeItem(outfile, key);
        writeItem(outfile, *vptr);
        vptr = m.getNext(key);
    }
    
    return true;
}

bool IndexerImpl::save(string filenameBase)
{
    return saveMyMap(filenameBase + ".u2i", urlToId) &&
    saveMyMap(filenameBase + ".i2u", idToUrl) &&
    saveMyMap(filenameBase + ".w2ic", wordToIdCounts);
}

bool readItem(istream& stream, string& s)
{
    getline(stream,s);
    if (! stream)
        return false;
    else return true;
}
bool readItem(istream& stream, int& i)
{
    string line;
    if ( ! getline(stream, line)) // return false if getline fails
        return false;
    // create an input stringstream whose text will be a copy of line
    istringstream iss(line);
    iss >> i;
    if  (! iss)
        return false;
    else return true;
}
bool readItem(istream& stream, vector<int>& iv)
{
    int size;
    if (! readItem(stream, size))
        return false;
    for (int k = 0; k < size/2; k++)
    {
        int id;
        if (! readItem(stream, id))
            return false;
        int count;
        if (! readItem(stream, count))
            return false;
        iv.push_back(id);
        iv.push_back(count);
    }
    return true;
}

template<typename KeyType, typename ValueType>
bool loadMyMap(string filename, MyMap<KeyType,ValueType>& m)
{
    m.clear();
    ifstream infile(filename.c_str());
    if (! infile)
        return false;
    int size;
    if (! readItem(infile, size) )
        return false;
    for (int k = 0; k != size; k++)
    {
        KeyType key;
        if (! readItem(infile, key) )
            return false;
        ValueType value;
        if (! readItem(infile, value) )
            return false;
        m.associate(key,value);
    }
    return true;
}

bool IndexerImpl::load(string filenameBase)
{
    return loadMyMap(filenameBase + ".u2i", urlToId) &&
    loadMyMap(filenameBase + ".i2u", idToUrl) &&
    loadMyMap(filenameBase + ".w2ic", wordToIdCounts);
}

//******************** Indexer functions *******************************

// These functions simply delegate to IndexerImpl's functions.
// You probably don't want to change any of this code.

Indexer::Indexer()
{
    m_impl = new IndexerImpl;
}

Indexer::~Indexer()
{
    delete m_impl;
}

bool Indexer::incorporate(string url, WordBag& wb)
{
    return m_impl->incorporate(url, wb);
}

vector<UrlCount> Indexer::getUrlCounts(string word)
{
    return m_impl->getUrlCounts(word);
}

bool Indexer::save(string filenameBase)
{
    return m_impl->save(filenameBase);
}

bool Indexer::load(string filenameBase)
{
    return m_impl->load(filenameBase);
}
