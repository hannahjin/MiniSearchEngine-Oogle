#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED

#include <queue>

template <class KeyType, class ValueType>
class MyMap
{
public:
    MyMap()
    : m_size(0), m_root(NULL) { }
    
    ~MyMap()
    {
        clear();
    }
    
    void clear()
    {
        if (m_root != NULL) // tree is not empty
        {
            clearTree(m_root->left);
            clearTree(m_root->right);
            delete m_root;
            m_root = NULL;
        }
    }
    
    int size() const
    {  return m_size;  }
    
    void associate(const KeyType& key, const ValueType& value);
    
    const ValueType* find(const KeyType& key) const
    {
        Node* result= lookForNode(m_root,key);
        if (result != NULL) // found a node matching key
            return &(result->value);
        else // no node contains key
            return NULL;
    }
    
    ValueType* find(const KeyType& key)
    {
        // Do not change the implementation of this overload of find
        const MyMap<KeyType,ValueType>* constThis = this;
        return const_cast<ValueType*>(constThis->find(key));
    }
    
    ValueType* getFirst(KeyType& key)
    {
        if (m_root == NULL) // empty tree
            return NULL;
        key = m_root->key;
        
        while (!m_queue.empty()){
            m_queue.pop();
        }
        m_queue.push(m_root->left);
        m_queue.push(m_root->right);
        
        return &(m_root->value);
    }
    
    ValueType* getNext(KeyType& key)
    {
        if(m_queue.empty())
            return NULL;
        Node* toBe;
        do
        {
            toBe = m_queue.front();
            m_queue.pop();
        } while (toBe == NULL && !m_queue.empty());
        
        if (toBe != NULL)
        {
            m_queue.push(toBe->left);
            m_queue.push(toBe->right);
            key = toBe->key;
            return &(toBe->value);
        }
        else // no Node after
            return NULL;
    }
    
private:
    struct Node
    {
        KeyType key;
        ValueType value;
        Node* left;
        Node* right;
    };
    int m_size;
    Node* m_root;
    std::queue<Node*> m_queue;
    
    void clearTree (Node* tp);
    Node* lookForNode(Node* p, const KeyType& key) const
    {
        if (p == NULL)
            return NULL;
        if (p->key == key)
            return p;
        Node* lookLeft = lookForNode(p->left, key); // search in left subtree
        if (lookLeft != NULL)
            return lookLeft;
        else
            return lookForNode(p->right, key); // search in right subtree
        
    }
    
    // declared to avoid random access
    MyMap(const MyMap &other);
    MyMap &operator=(const MyMap &other);
};

template <class KeyType, class ValueType>
void MyMap<KeyType,ValueType>::clearTree(Node* tp)
{
    if (tp == NULL)
        return;
    clearTree(tp->left);
    clearTree(tp->right);
    delete tp;
}

template <class KeyType, class ValueType>
void MyMap<KeyType,ValueType>::associate(const KeyType& key, const ValueType& value)
{
    Node* create = new Node;
    create->left = NULL;
    create->right = NULL;
    create->key = key;
    create->value = value;
    if (m_root == NULL)
    {
        m_root = create;
        m_size++;
        return;
    }
    Node* parent = m_root;
    while (true)
    {
        if (parent->key > key)
        {
            if (parent->left == NULL)
            {
                parent->left = create;
                m_size++;
                break;
            }
            else
                parent = parent->left;
                }
        else if (parent->key < key)
        {
            if (parent->right == NULL)
            {
                parent->right = create;
                m_size++;
                break;
            }
            else
                parent = parent->right;
                }
        else // key exists already
        {
            parent->value = value;
            delete create;
            return;
        }
    }
}


#endif // MYMAP_INCLUDED
