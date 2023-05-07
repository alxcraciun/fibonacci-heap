#include <iostream>
#include <fstream>

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
    : value(0), parent(nullptr), left(this), right(this), child(nullptr), marked(false), degree(0)
{
}

Node::Node(const int val)
    : value(val), parent(nullptr), left(this), right(this), child(nullptr), marked(false), degree(0)
{
}

Node::Node(const Node& node)
    : value(node.value), parent(node.parent), left(node.left), right(node.right), child(node.child), marked(node.marked), degree(node.degree)
{
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

int main()
{
	Node n1;
	Node n2(30);
	Node n3(n2);
    Node n4 = n3;

    cout << n1 << '\n';
    cout << n2 << '\n';
    cout << n3 << '\n';
    cout << n4 << '\n';

    in.close();
    out.close();

    return 0;
}