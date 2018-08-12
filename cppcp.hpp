#include <iostream>
#include <vector>
#include <array>
#include <cassert>
#include <algorithm>

using namespace std;

// --- DEBUG -------------------------------------------------------

template <typename T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    os << "{";
    for(int i=0; i<v.size()-1; i++)
        os << v[i] << ", ";
    os << v.back() << "}";
}

// --- TREE --------------------------------------------------------

template<typename T>
struct TreeNode
{
    TreeNode<T>* parent;
    vector<TreeNode<T>> children;
    T val;

    TreeNode() {}
    TreeNode(const T& val) : val(val) {}
    TreeNode(const TreeNode& other)
    {
        val = other.val;
        children = other.children;
        for(auto& c : children) c.parent = this;
    }
    TreeNode(T val, const vector<TreeNode>& children)
        : children(children)
        , val(val)
    {
        for(auto c : this->children) c.parent = this;
    }

    string stringify()const {
        stringstream ss;
        addMeToStringStream(ss, 0, true);
        return ss.str();
    }

private:
    const char* indentString = "    ";
    void addMeToStringStream(stringstream& ss, int indent, bool isLast)const {
        for(int i=0; i<indent; i++) ss << indentString;
        if(children.empty()) {
            ss << val;
        }
        else {
            ss << "{ " << val << ", {" << endl;
            for(int i=0; i<children.size(); i++) {
                children[i].addMeToStringStream(ss, indent + 1, i == children.size() - 1);
            }
            for(int i=0; i<indent; i++) ss << indentString;
            ss << "}}";
        }
        if(!isLast) ss << ",";
        ss << endl;
    }
};

template <typename T>
struct Tree
{
    TreeNode<T> root;
    Tree() {}
    Tree(const Tree& other) : root(other.root) {}
    Tree(T val, const vector<TreeNode<T>>& children)
        : root(val, children)
    {}

    string stringify()const { return root.stringify(); }
};

// --- TABLE -----------------------------------------------------------------------------

template <typename T, unsigned DIMENSIONS>
struct Table
{
    static_assert(DIMENSIONS > 0);

    Table() {}
    Table(array<int, DIMENSIONS> dims)
        : dims(dims)
    {
        int n = 1;
        for(int d : dims) n *= d;
        values.resize(n);
    }

    void setAll(const T& val) { for(auto& x : values) x = val; }

    T& operator()(const array<int, DIMENSIONS>& indices) {
        int i = getOneIndex(indices);
        return values[i];
    }
    const T& operator()(const array<int, DIMENSIONS>& indices)const {
        int i = getOneIndex(indices);
        return values[i];
    }

    template <typename... Args>
    T& operator()(Args... indices)
    {
        int i = getOneIndex({indices...});
        return values[i];
    }

    template <typename... Args>
    const T& operator()(Args... indices)const
    {
        int i = getOneIndex({indices...});
        return values[i];
    }

    int getOneIndex(const array<int, DIMENSIONS>& indices)const {
        int i = 0;
        int n = 1;
        for(int j = 0; j < indices.size(); j++) {
            assert(indices[j] >= 0 && indices[j] < dims[j]);
            i *= n;
            i += indices[DIMENSIONS - j - 1];
            n *= dims[j];
        }
        return i;
    }

    int size(int i)const {
        assert(i >= 0 && i < DIMENSIONS);
        return dims[i];
    }

    int rows()const{ return dims[0]; }
    int cols()const{ return dims[1]; }

    int W()const{ return dims[0]; }
    int H()const{ return dims[1]; }

    array<int, DIMENSIONS> dims;
    vector<T> values;
};