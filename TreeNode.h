#ifndef TREENODE_H_INCLUDED
#define TREENODE_H_INCLUDED

#include <map>
using std::map;

class TreeNode {
public:
    TreeNode() {};
    TreeNode(bool internalNode)
    {
        this->internalNode = internalNode;
    };

    bool get_internalNode() {return this->internalNode;};

    void set_featureID(int featureID) {this->featureID = featureID;};
    int get_featureID() {return this->featureID;};

    void set_typeID(int typeID) {this->typeID = typeID;};
    int get_typeID() {return this->typeID;};

    void set_treeNodeMap(map<int, TreeNode> treeNodeMap) {this->treeNodeMap = treeNodeMap;};
    map<int, TreeNode> get_treeNodeMap() {return this->treeNodeMap;};
private:
    bool internalNode;
    int featureID;
    int typeID;
    map<int, TreeNode> treeNodeMap;
};

#endif // TREENODE_H_INCLUDED
