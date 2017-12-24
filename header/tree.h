//
//  tree.h
//  CreateDecisionTree
//
//  Created by panghu on 2017/12/23.
//  Copyright © 2017年 panghu. All rights reserved.
//

#ifndef tree_h
#define tree_h
#include <iostream>
#include <vector>

class Attribute { //属性
public:
    std::string name;
    int index; //original attribute index
    int possible_values;
};

class Watermelon {
public:
    std::vector<int> attribute_values; //西瓜的属性值
    int label; //good watermelon = 1; bad watermelon = 0
};

//树的节点
class TreeNode {
public:
    TreeNode(){firstChild = NULL; nextSibling = NULL;}
    ~TreeNode();
    
    int label; //label of the leafnode, i.e. which class does the TreeNode belong to?
    Attribute attribute; //attribute used for partition
    
    TreeNode *firstChild;//指向该节点的第一个孩子
    TreeNode *nextSibling;//指向该节点的兄弟节点
};

class Tree{
public:
    Tree(TreeNode * r = NULL):root(r){}
    ~Tree();
    
    void preOrder();//前序遍历
    void print();//打印
    void FreeTree(TreeNode* node);
    void addBrotherNode(TreeNode* bro, TreeNode* node); //添加兄弟节点
    void addNode(TreeNode* parent, TreeNode* child); //添加孩子节点
    
    TreeNode *TreeGenerate(std::vector<Watermelon> watermelons, std::vector<Attribute> attributes); //生成树
    Attribute ChoosePartitionAttribute(std::vector<Watermelon> watermelons, std::vector<Attribute> attributes); //选择分支节点的属性
    
    TreeNode * root;//该树的根
    
private:
    void print(TreeNode* node, int num);
    void preOrder(TreeNode* parent);//前序遍历
};

#endif /* tree_h */
