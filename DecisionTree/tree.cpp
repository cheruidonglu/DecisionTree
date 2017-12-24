//
//  tree.cpp
//  CreateDecisionTree
//
//  Created by panghu on 2017/12/23.
//  Copyright © 2017年 panghu. All rights reserved.
//

#include <stdio.h>
#include "tree.h"
#include <math.h>

//打印树的形状
void Tree::print()
{
    print(root, 0);
}

void printSpace(int num)
{
    int i = 0;
    for(i = 0; i < num-3; i++)
        std::cout << " ";
    
    for(; i < num-2; ++i)
        std::cout << "|";
    for(; i < num; ++i)
        std::cout << "_";
}

void Tree::print(TreeNode* node, int num)
{
    if(node != NULL){
        printSpace(num);
        
        if (node->firstChild == NULL) {
            if (node->label == 1) {
                std::cout << "好瓜" << std::endl;
            }
            else {
                std::cout << "坏瓜" << std::endl;
            }
        }
        else
            std::cout << node->attribute.name << std::endl;
        
        print(node->firstChild, num+4);
        print(node->nextSibling, num);
    }
}

//前序遍历
void Tree::preOrder()
{
    std::cout << "前序遍历: ";
    preOrder(root);
    std::cout << std::endl;
}

void Tree::preOrder(TreeNode* parent)
{
    if(parent != NULL){
        
        if (parent->firstChild == NULL)
            std::cout << parent->label << " ";
        else
            std::cout << parent->attribute.name << "=? ";
        
        preOrder(parent->firstChild);
        preOrder(parent->nextSibling);
    }
}

//释放所有节点的内存空间
Tree::~Tree()
{
    FreeTree(root);
}

void Tree::FreeTree(TreeNode* node){
    if(node!=NULL)
    {
        FreeTree(node->firstChild);
        FreeTree(node->nextSibling);
        free(node);	//当左右子结点都为空时，调用free,释放空间
    }
}

//将节点插入到兄弟节点
void Tree::addBrotherNode(TreeNode* bro, TreeNode* node)
{
    if(bro->nextSibling == NULL)
        bro->nextSibling = node;
    else
        addBrotherNode(bro->nextSibling, node);
}

//addNode将父子结点组对
//如果父节点的firstChild==NULL, 则firstChild = node;
//如果父节点的firstChild != NULL, 则
void Tree::addNode(TreeNode* parent, TreeNode* child)
{
    if(parent->firstChild == NULL)
        parent->firstChild = child;
    else
        addBrotherNode(parent->firstChild, child);
}

//choose best partition
Attribute Tree::ChoosePartitionAttribute(std::vector<Watermelon> watermelons, std::vector<Attribute> attributes) {
    //compute entropy of root node
    float entropy_root;
    int positive_number = 0;
    int negative_number = 0;
    int total_number = static_cast<int>(watermelons.size());
    for(int i = 0; i < watermelons.size(); i++){
        if (watermelons[i].label == 1) {  //good watermelon
            positive_number++;
        }
        else
            negative_number++;
    }
    float temp1 = static_cast<float>(positive_number)/static_cast<float>(total_number);
    float temp2 = static_cast<float>(negative_number)/static_cast<float>(total_number);
    if (temp1<10e-3 || temp2<10e-3){
        entropy_root = 0.0f;
    }
    else{
        entropy_root = - temp1 * log2(temp1) - temp2 * log2(temp2);
    }
    
    //compute gain of each attribute
    std::vector<float> gain;
    for (int i = 0; i < attributes.size(); i++) {
        //compute entropy of each possible value of each attribute
        std::vector<float> entropy_attribute_value; //qianbai
        std::vector<int> total_watermelon_per_attribute_value;
        for(int j = 0; j < attributes[i].possible_values; j++) {
            int total_watermelons = 0;
            int positive_watermelons = 0;
            int negative_watermelons = 0;
            for (int k = 0; k < watermelons.size(); k++) {
                if (watermelons[k].attribute_values[attributes[i].index] == j) {
                    total_watermelons++;
                    if 	(watermelons[k].label == 1) //good watermelon
                        positive_watermelons++;
                    else
                        negative_watermelons++;
                }
            }
            float entropy_temp = 0.0f;
            if (total_watermelons != 0) {
                float temp3 = static_cast<float>(positive_watermelons)/static_cast<float>(total_watermelons);
                float temp4 = static_cast<float>(negative_watermelons)/static_cast<float>(total_watermelons);
                if (temp3<10e-3 || temp4<10e-3){
                    entropy_temp = 0.0f;
                }
                else{
                    entropy_temp = - temp3 * log2(temp3) - temp4 * log2(temp4);
                }
            }
            entropy_attribute_value.push_back(entropy_temp);
            total_watermelon_per_attribute_value.push_back(total_watermelons);
        }
        float gain_attribute = entropy_root;
        for (int j = 0; j < attributes[i].possible_values; j++) {
            gain_attribute = gain_attribute - static_cast<float>(total_watermelon_per_attribute_value[j]) /
            static_cast<float>(total_number) * entropy_attribute_value[j];
        }
        gain.push_back(gain_attribute);
    }
    
    //find the attribute with the largest gain
    Attribute partition_attribute;
    float max_gain = gain[0];
    partition_attribute.name = attributes[0].name;
    partition_attribute.index = attributes[0].index;
    partition_attribute.possible_values = attributes[0].possible_values;
    for (int i = 1; i < attributes.size(); i++) {
        if (gain[i] > max_gain) {
            max_gain = gain[i];
            partition_attribute.name = attributes[i].name;
            partition_attribute.index = attributes[i].index;
            partition_attribute.possible_values = attributes[i].possible_values;
        }
    }
    
    return partition_attribute;
}


TreeNode* Tree::TreeGenerate(std::vector<Watermelon> watermelons, std::vector<Attribute> attributes) {
    
    TreeNode *node = new TreeNode;
    
    //if the labels of watermelons are the same
    bool flag = true;
    for (int i = 1; i < watermelons.size(); i++) {
        if (watermelons[i].label != watermelons[0].label) {
            flag = false;
            break;
        }
    }
    if (flag == true) {
        node->label = watermelons[0].label;
        return node;
    }
    
    //if attributes set is empty
    if (attributes.size() == 0) {
        int cnt = 0;
        for (int i = 0; i < watermelons.size(); i++) {
            cnt += watermelons[i].label;
        }
        if (cnt >= (int)watermelons.size() / 2)
            node->label = 1;
        else
            node->label = 0;
        return node;
    }
    
    // if watermelons have same attributes
    bool flag_same_attributes = true;
    for(int j = 0; j < attributes.size(); j++) {
        for (int i = 1; i < watermelons.size(); i++) {
            if (watermelons[i].attribute_values[attributes[j].index] != watermelons[0].attribute_values[attributes[j].index]) {
                flag_same_attributes = false;
                break;
            }
        }
        if (flag_same_attributes == false)
            break;
    }
    if (flag_same_attributes == true) {
        int cnt = 0;
        for (int i = 0; i < watermelons.size(); i++) {
            cnt += watermelons[i].label;
        }
        if (cnt >= (int)watermelons.size() / 2) node->label = 1;
        else node->label = 0;
        return node;
    }
    
    // function to be filled
    Attribute partition_attribute = ChoosePartitionAttribute(watermelons, attributes);
    
    //for each possible value of parition_attribute
    //e.g. qinglv qianbai wuhei
    node->attribute.name = partition_attribute.name;
    node->attribute.index = partition_attribute.index;
    for (int i = 0; i < partition_attribute.possible_values; i++) {
        
        //e.g. which watermelons belong to qinglv?
        std::vector<Watermelon> watermelons_new_node;
        for (int j = 0; j < watermelons.size(); j++) {
            if (watermelons[j].attribute_values[partition_attribute.index] == i) {
                watermelons_new_node.push_back(watermelons[j]);
            }
        }
        
        if (watermelons_new_node.size() == 0) { //e.g. no watermelon with qinglv color
            TreeNode *node_new = new TreeNode;
            addNode(node, node_new);
            int cnt = 0;
            for (int k = 0; k < watermelons.size(); k++) {
                cnt += watermelons[k].label;
            }
            if (cnt >= (int)watermelons.size() / 2)
                node_new->label = 1;
            else 
                node_new->label = 0;
        }
        else {
            std::vector<Attribute> attributes_new_node;
            for (int k = 0; k < attributes.size(); k++){
                if (attributes[k].index != partition_attribute.index){
                    attributes_new_node.push_back(attributes[k]);
                }
            }
            TreeNode *node_new = TreeGenerate(watermelons_new_node, attributes_new_node);
            addNode(node, node_new);
        }
    }
    return node;
}

