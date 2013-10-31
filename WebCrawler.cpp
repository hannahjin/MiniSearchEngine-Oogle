#include "provided.h"
#include "http.h"
#include <string>
using namespace std;

class WebCrawlerImpl
{
public:
    WebCrawlerImpl();
    void addUrl(string url);
    int getNumberOfUrls() const;
    void crawl(void (*callback)(string url, bool success));
    bool save(string filenameBase);
    bool load(string filenameBase);
private:
    Indexer m_indexer;
    vector<string> m_urls;
    int m_numUrls;
};

WebCrawlerImpl::WebCrawlerImpl()
: m_numUrls(0)   {}

void WebCrawlerImpl::addUrl(string url)
{
    m_urls.push_back(url);
    m_numUrls++;
}

int WebCrawlerImpl::getNumberOfUrls() const
{
    return m_numUrls;
}

void WebCrawlerImpl::crawl(void (*callback)(string url, bool success))
{
    for (size_t k = 0; k < m_urls.size(); k++)
    {
        // connect to webpage specified by URL
        string url = m_urls[k];
        string page;
        bool processingStatus = HTTP().get(url,page);
        
        // successfully downloaded the page, create a wordbag for the url
        if (processingStatus)
        {
            WordBag wb(page);
            processingStatus = m_indexer.incorporate(url, wb);
        }
        
        callback(url, processingStatus);
        m_numUrls--;
    }
    m_urls.clear();
}

bool WebCrawlerImpl::save(string filenameBase)
{
    return m_indexer.save(filenameBase);
}

bool WebCrawlerImpl::load(string filenameBase)
{
    return m_indexer.load(filenameBase);
}

//******************** WebCrawler functions *******************************

// These functions simply delegate to WebCrawlerImpl's functions.
// You probably don't want to change any of this code.

WebCrawler::WebCrawler()
{
    m_impl = new WebCrawlerImpl;
}

WebCrawler::~WebCrawler()
{
    delete m_impl;
}

void WebCrawler::addUrl(string url)
{
    m_impl->addUrl(url);
}

int WebCrawler::getNumberOfUrls() const
{
    return m_impl->getNumberOfUrls();
}

void WebCrawler::crawl(void (*callback)(string url, bool success))
{
    m_impl->crawl(callback);
}

bool WebCrawler::save(string filenameBase)
{
    return m_impl->save(filenameBase);
}

bool WebCrawler::load(string filenameBase)
{
    return m_impl->load(filenameBase);
}
