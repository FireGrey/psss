#pragma once
#ifndef NODE
#define NODE

#include <vector>

using namespace std;

class Node
{
public:
	Node();
	Node(int newValue);
	Node(int newValue, int newStrideNum);
	~Node();
	vector<Node> getChildren();
	void addChild(Node newChild);
	int numChildren();
	int getValue();
	void setValue(int newValue);
	int getStrideNum();
	Node child(int childNum);
private:
	vector<Node> children;
	int childCount;
	int value;
	int strideNum;
};

#endif
