#include "stdafx.h"
#include "Node.h"

Node::Node()
{
	childCount = 0;
};

Node::Node(int newValue)
{
	childCount = 0;
	value = newValue;
};

Node::Node(int newValue, int newStrideNum)
{
	childCount = 0;
	value = newValue;
	strideNum = newStrideNum;
};

Node::~Node()
{
	children.clear();
};

vector<Node> Node::getChildren()
{
	return children;
}

int Node::numChildren()
{
	return childCount;
}

int Node::getValue()
{
	return value;
}

void Node::setValue(int newValue)
{
	value = newValue;
}

int Node::getStrideNum()
{
	return strideNum;
}

Node Node::child(int childNum) {
	return children[childNum];
}

void Node::addChild(Node newChild)
{
	children.push_back(newChild);
	childCount++;
}