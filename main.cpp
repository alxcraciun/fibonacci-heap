#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

ifstream in("mergeheap.in");
ofstream out("mergeheap.out");

class Node
{
public:
    int value;
    Node* parent;
    Node* left;
    Node* right;
    Node* child;
    int degree;
    bool marked;

    Node();
    Node(const int);
    Node(const Node&);

    Node& operator=(const Node&);
    friend ostream& operator<<(ostream&, const Node&);
};

Node::Node()
    : value(0), parent(nullptr), left(this), right(this), child(nullptr), marked(false), degree(0) {}

Node::Node(const int val)
    : value(val), parent(nullptr), left(this), right(this), child(nullptr), marked(false), degree(0) {}

Node::Node(const Node& node)
    : value(node.value), parent(node.parent), left(node.left), right(node.right), child(node.child), marked(node.marked), degree(node.degree) {}

Node& Node::operator=(const Node& node)
{
    this->value = node.value;
    this->parent = node.parent;
    this->left = node.left;
    this->right = node.right;
    this->child = node.child;
    this->marked = node.marked;
    this->degree = node.degree;
    return *this;
}

ostream& operator<<(ostream& out, const Node& node)
{
    out << "Node: " << node.value << "\n";

    out << "P: ";
    if (node.parent != nullptr)
        out << node.parent->value;
    else
        out << "null";

    out << " | L: ";
    if (node.left != nullptr)
        out << node.left->value;
    else
        out << "null";

    out << " | R: ";
    if (node.right != nullptr)
        out << node.right->value;
    else
        out << "null";

    out << " | C: ";
    if (node.child != nullptr)
        out << node.child->value;
    else
        out << "null";

    out << '\n';
    return out;
}

class FibonacciHeap
{
    int size;
    Node* maxNode;

public:
    FibonacciHeap();
    FibonacciHeap(int, Node*);

    void insert(int);
    int extractMax();
    
    FibonacciHeap& operator+=(const FibonacciHeap&);
    FibonacciHeap& operator=(const FibonacciHeap&);
};

FibonacciHeap::FibonacciHeap()
    : maxNode(nullptr), size(0) {}

FibonacciHeap::FibonacciHeap(int size, Node* node)
    : size(size), maxNode(node) {}

void FibonacciHeap::insert(int val)
{
    Node* node = new Node(val);
    if (this->maxNode == nullptr)
    {
        this->maxNode = node;
    }
    else
    {
        Node* last = this->maxNode->left;
        node->left = last;
        last->right = node;
        node->right = this->maxNode;
        this->maxNode->left = node;

        if (this->maxNode->value < node->value)
            this->maxNode = node;
    }
    this->size++;
}

FibonacciHeap& FibonacciHeap::operator+=(const FibonacciHeap& other)
{
    this->size += other.size;

    if (this->maxNode == nullptr)
        this->maxNode = other.maxNode;

    if (this ->maxNode == nullptr || other.maxNode == nullptr)
        return *this;

    Node* root1 = this->maxNode;
    Node* last1 = root1->left;
    
    Node* root2 = other.maxNode;
    Node* last2 = root2->left;

    root1->left = last2;
    last2->right = root1;
    root2->left = last1;
    last1->right = root2;

    if(other.maxNode->value > this->maxNode->value) 
        this->maxNode=other.maxNode;

    return *this;
}

int main()
{
    int N, Q;
    FibonacciHeap fh;
    vector<FibonacciHeap*> heaps;

    in >> N >> Q;
    for (int i = 0; i <= N; i++)
        heaps.push_back(new FibonacciHeap);

    int cer = 0, v1 = 0, v2 = 0;
    for (int i = 1; i <= Q; i++)
    {
        in >> cer;

        if (cer == 2)
            in >> v1;
        else
            in >> v1 >> v2;

        if (cer == 1)
        {
            heaps[v1]->insert(v2);
        }
        else if (cer == 2)
        {
            out << heaps[v1]->extractMax() << '\n';
        }
        else if (cer == 3)
        {
            *heaps[v1] += *heaps[v2];
            heaps[v2] = new FibonacciHeap();
        }
        else
        {
            break;
        }
    }

    in.close();
    out.close();

    return 0;
}