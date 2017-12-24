//
//  main.cpp
//  DecisionTree
//
//  Created by panghu on 2017/8/7.
//  Copyright © 2017年 panghu. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include "tree.h"

using namespace std;

//Read Watermelon training data
void ReadWatermelonData(std::string filename, std::vector<Watermelon> *Watermelons) {
    std::string line;
    std::string attribute;
    std::ifstream myfile(filename);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            Watermelon watermelon;
            int attribute_value = 0;
            std::string temp;
            std::istringstream attribute(line);
            
            //color
            attribute >> temp;
            if (temp == "青绿")
                attribute_value = 0;
            else if (temp == "乌黑")
                attribute_value = 1;
            else if (temp == "浅白")
                attribute_value = 2;
            watermelon.attribute_values.push_back(attribute_value);
            
            //root
            attribute >> temp;
            if (temp == "蜷缩")
                attribute_value = 0;
            else if (temp == "稍蜷")
                attribute_value = 1;
            else if (temp == "硬挺")
                attribute_value = 2;
            watermelon.attribute_values.push_back(attribute_value);
            
            //sound
            attribute >> temp;
            if (temp == "浊响")
                attribute_value = 0;
            else if (temp == "清脆")
                attribute_value = 1;
            else if (temp == "沉闷")
                attribute_value = 2;
            watermelon.attribute_values.push_back(attribute_value);
            
            //texture
            attribute >> temp;
            if (temp == "清晰")
                attribute_value = 0;
            else if (temp == "稍糊")
                attribute_value = 1;
            else if (temp == "模糊")
                attribute_value = 2;
            watermelon.attribute_values.push_back(attribute_value);
            
            //umbilical
            attribute >> temp;
            if (temp == "凹陷")
                attribute_value = 0;
            else if (temp == "稍凹")
                attribute_value = 1;
            else if (temp == "平坦")
                attribute_value = 2;
            watermelon.attribute_values.push_back(attribute_value);
            
            //touch
            attribute >> temp;
            if (temp == "硬滑")
                attribute_value = 0;
            else if (temp == "软粘")
                attribute_value = 1;
            watermelon.attribute_values.push_back(attribute_value);
            
            //lable
            attribute >> temp;
            if (temp == "是")
                watermelon.label = 1;//好瓜
            else if (temp == "否")//坏瓜
                watermelon.label = 0;
            
            Watermelons->push_back(watermelon);
            
            std::cout << line << '\n';
        }
        myfile.close();
    }
    else {
        std::cout << "unable to open file";
    }
}

//Initialize attribute set
void InitializeAttributes(std::vector<Attribute> *attributes){
    Attribute attribute;
    
    std::string str("色泽");
    attribute.name = str;
    attribute.possible_values = 3;
    attribute.index = 0;
    attributes->push_back(attribute);
    
    str = "根蒂";
    attribute.name = str;
    attribute.possible_values = 3;
    attribute.index = 1;
    attributes->push_back(attribute);
    
    str = "敲声";
    attribute.name = str;
    attribute.possible_values = 3;
    attribute.index = 2;
    attributes->push_back(attribute);
    
    str = "纹理";
    attribute.name = str;
    attribute.possible_values = 3;
    attribute.index = 3;
    attributes->push_back(attribute);
    
    str = "脐部";
    attribute.name = str;
    attribute.possible_values = 3;
    attribute.index = 4;
    attributes->push_back(attribute);
    
    str = "触感";
    attribute.name = str;
    attribute.possible_values = 2;
    attribute.index = 5;
    attributes->push_back(attribute);
}

/*测试新的西瓜是好瓜还是坏瓜*/
//如果当前节点是叶子结点,将叶子结点的label返回
//当前节点如果是分支节点,判断西瓜的属性值属于哪个分支，再次调用TestNewWatermelon
bool TestNewWatermelon(Watermelon new_watermelon, TreeNode* node) {

    if (node->firstChild == NULL) {
        return node->label;
    }
    else {
        int child_index = new_watermelon.attribute_values[node->attribute.index]; //西瓜的属性值属于第几个分支
        TreeNode *node_new = NULL;
        node_new = node->firstChild;
        if (child_index != 0) {
            while (child_index!=0) {
                node_new = node_new->nextSibling;
                child_index--;
            }
        }
        return TestNewWatermelon(new_watermelon, node_new);
    }
}

/*根据WatermelonData训练一颗决策树，并测试一个新的西瓜样本*/
int main(int argc, const char * argv[]) {
    //Read watermelon data
    std::vector<Watermelon> watermelons;
    ReadWatermelonData("/Users/panghu/Desktop/machinelearning/DecisionTree/data/WatermelonData.txt",&watermelons);
    
    //Initialize attributes
    std::vector<Attribute> attributes;
    InitializeAttributes(&attributes);
    
    //Train Decision Tree
    Tree tree;
    tree.root = tree.TreeGenerate(watermelons, attributes);
    
    //Print the trained tree
    tree.print();
    
    //Test new watermelon
    Watermelon watermelon_test;
    watermelon_test.attribute_values.push_back(0); //青绿
    watermelon_test.attribute_values.push_back(0); //蜷缩
    watermelon_test.attribute_values.push_back(0); //浊响
    watermelon_test.attribute_values.push_back(0); //清晰
    watermelon_test.attribute_values.push_back(0); //凹陷
    watermelon_test.attribute_values.push_back(0); //硬滑
    bool good_watermelon = TestNewWatermelon(watermelon_test, tree.root);
    
    if(good_watermelon)
        std::cout << "是好瓜"<< std::endl;
    else
        std::cout << "是坏瓜"<< std::endl;
    
    return 0;
}

