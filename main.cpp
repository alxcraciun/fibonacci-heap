#include <cmath>
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
    int value = 0;
    Node* parent = nullptr;
    Node* left = this;
    Node* right = this;
    Node* child = nullptr;
    int degree = 0;
    bool marked = false;

    Node(const int);
    Node(const Node&);

    void merge(Node&);

    Node& operator=(const Node&);
    friend ostream& operator<<(ostream&, const Node&);
};

Node::Node(const int val)
{
    this->value = val;
}

Node::Node(const Node& node)
    : value(node.value), parent(node.parent), left(node.left), right(node.right), child(node.child), marked(node.marked), degree(node.degree) {}

void Node::merge(Node& other)
{
    Node* left = other.left;
    Node* right = other.right;

    left->right = right;
    right->left = left;

    if (this->child != nullptr)
    {
        Node* child = this->child;
        Node* last = child->left;

        other.right = child;
        other.left = last;
        last->right = &other;
        child->left = &other;
        other.parent = this;
    }
    else
    {
        this->child = &other;
        other.parent = this;
        other.left = &other;
        other.right = &other;
    }
    this->degree++;
}

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
    Node* head;
    Node* maxNode;

public:
    FibonacciHeap();
    FibonacciHeap(int, Node*);

    void insert(int);

    int extractMax();
    void removeMax();
    void consolidate();
    void link(Node*, vector<Node*>&);

    FibonacciHeap& operator+=(const FibonacciHeap&);
    FibonacciHeap& operator=(const FibonacciHeap&);
};

FibonacciHeap::FibonacciHeap()
{
    maxNode = nullptr;
    size = 0;
}

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
        {
            this->maxNode = node;
        }
    }
    this->size++;
}

FibonacciHeap& FibonacciHeap::operator+=(const FibonacciHeap& other)
{
    this->size += other.size;

    if (this->maxNode == nullptr)
    {
        this->maxNode = other.maxNode;
    }

    if (this->maxNode == nullptr || other.maxNode == nullptr)
    {
        return *this;
    }

    Node* root1 = this->maxNode;
    Node* last1 = root1->left;

    Node* root2 = other.maxNode;
    Node* last2 = root2->left;

    root1->left = last2;
    last2->right = root1;
    root2->left = last1;
    last1->right = root2;

    if (other.maxNode->value > this->maxNode->value)
    {
        this->maxNode = other.maxNode;
    }   
    return *this;
}

void FibonacciHeap::removeMax()
{
    Node* left = this->maxNode->left;
    Node* right = this->maxNode->right;
    left->right = right;
    right->left = left;
}

void FibonacciHeap::link(Node* node, vector<Node*>& next_roots)
{
    Node* next = node->right;
    int d = node->degree;

    while (next_roots[d] != nullptr)
    {
        Node* node2 = next_roots[d];
        if (node->value < node2->value)
        {
            swap(node, node2);
        }
        node->merge(*node2);
        next_roots[d] = nullptr;
        d++;
    }
    next_roots[d] = node;
    if (!next->marked)
    {
        next->marked = true;
        this->link(next, next_roots);
        next->marked = false;
    }
}

void FibonacciHeap::consolidate()
{
    int height = ceil(log2(this->size)) + 1;
    vector<Node*> next_roots(height);
    for (int i = 0; i < height; i++)
        next_roots.push_back(nullptr);

    this->maxNode->marked = true;
    this->link(this->maxNode, next_roots);
    this->maxNode->marked = false;

    this->maxNode = nullptr;
    this->head = nullptr;
    for (unsigned long long i = 0; i < next_roots.size(); i++)
    {
        if (next_roots[i] != nullptr)
        {
            if (this->head == nullptr)
            {
                this->head = next_roots[i];
            }
            Node* last = this->head->left;
            next_roots[i]->right = head;
            next_roots[i]->left = last;
            head->left = next_roots[i];
            last->right = next_roots[i];
            next_roots[i]->parent = nullptr;
            if (this->maxNode == nullptr || next_roots[i]->value > this->maxNode->value)
            {
                this->maxNode = next_roots[i];
            }
        }
    }
}

int FibonacciHeap::extractMax()
{
    Node* root = this->maxNode;
    if (root != nullptr)
    {
        if (root->child != nullptr)
        {
            Node* last = this->maxNode->left;
            Node* lastChild = root->child->left;

            root->child->left = this->maxNode;
            last->right = root->child;
            lastChild->right = root;
            root->left = lastChild;

            root->child->parent->child = nullptr;
        }
        this->removeMax();
        if (root == root->right)
        {
            this->maxNode = nullptr;
        }
        else
        {
            this->maxNode = root->right;
            this->consolidate();
        }
        this->size--;
    }
    return root->value;
}

int main()
{
    int N, Q;
    vector<FibonacciHeap*> heaps;

    in >> N >> Q;
    for (int i = 0; i <= N; i++)
        heaps.push_back(new FibonacciHeap);

    int cer = 0, v1 = 0, v2 = 0;
    for (int i = 1; i <= Q; i++)
    {
        in >> cer;

        if (cer == 2)
        {
            in >> v1;
        }
        else
        {
            in >> v1 >> v2;
        }

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