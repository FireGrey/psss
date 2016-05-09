// CSP3343 - Assignment 2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "tinyxml2\tinyxml2.h"
#include "tinyxml2\tinyxml2.cpp"
#include <string>
#include <iostream>
#include <vector>

using namespace tinyxml2;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    XMLDocument useCaseDiagram;
    useCaseDiagram.LoadFile("UseCaseDiagram.xml");

    const char* diagramTitle = NULL;
    int nodeCount = 0;
    vector<string> nodeList;
    int relationshipCount = 0;
    vector<vector<string>> relationships;

    if (useCaseDiagram.Error()) {
        printf("Cannot Parse Xml File");
    }
    else 
    {
        XMLElement* diagram = useCaseDiagram.FirstChildElement("diagram");
        diagramTitle = diagram->Attribute("title");
        cout << "Relationships in " << diagramTitle << ":\n\n";

        for (XMLElement* currentNode = diagram->FirstChildElement("node"); currentNode != NULL; 
            currentNode = currentNode->NextSiblingElement("node")) 
        {
            string nodeName(currentNode->Attribute("id"));
            nodeList.push_back(nodeName);
            for (XMLElement* currentRelationship = currentNode->FirstChildElement("relationship"); currentRelationship != NULL;
                currentRelationship = currentRelationship->NextSiblingElement("relationship")) 
            {
                string reltype(currentRelationship->Attribute("type"));
                string relTarget(currentRelationship->Attribute("target"));
                vector<string> relation;
                relation.push_back(reltype);
                relation.push_back(nodeName);
                relation.push_back(relTarget);
                relationships.push_back(relation);
            }
            nodeCount++;
        }
    }
    for (int relNum = 0; relNum < relationships.size(); relNum++) 
    {
        cout << relationships[relNum][0] << " Between '" << relationships[relNum][1] 
            << "' and '" << relationships[relNum][2] << "'\n";
        //printf("Type: %s Between %s and %s\n", relationships[relNum][0], relationships[relNum][1], relationships[relNum][2]);
    }
    string key;
    cin >> key;
	return 0;
}

