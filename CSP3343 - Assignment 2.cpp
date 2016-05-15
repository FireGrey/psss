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


void createMisuseCase(vector<vector<string>> &misuseCaseList, string actor, string node, string type) {
    vector<string> misuseTemp;
    misuseTemp.push_back(actor);
    misuseTemp.push_back(node);
    misuseTemp.push_back(type);
    misuseCaseList.push_back(misuseTemp);
}


int _tmain(int argc, _TCHAR* argv[])
{
    XMLDocument useCaseDiagram;
    useCaseDiagram.LoadFile("UseCaseDiagram.xml");

    const char* diagramTitle = NULL;
    int nodeCount = 0;
    // Node List
    // [i][0] == nodeName
    // [i][1] == nodeType (eg. Actor)
    vector<vector<string>> nodeList;
    int relationshipCount = 0;
    vector<vector<string>> relationshipList;
    // Misuse Case List
    // [i][0] == actor
    // [i][1] == node
    // [i][2] == type
    // ('actor' threatens 'node' with 'type')
    vector<vector<string>> misuseCaseList;

    if (useCaseDiagram.Error()) {
        printf("Cannot Parse Xml File");
    }
    else 
    {
        XMLElement* diagram = useCaseDiagram.FirstChildElement("diagram");
        diagramTitle = diagram->Attribute("title");

        // For each node
        for (XMLElement* currentNode = diagram->FirstChildElement("node"); currentNode != NULL; 
            currentNode = currentNode->NextSiblingElement("node")) 
        {
            // Get node details
            string nodeName(currentNode->Attribute("id"));
            string nodeType(currentNode->Attribute("type"));
            // Insert them into array
            vector<string> nodeTemp;
            nodeTemp.push_back(nodeName);
            nodeTemp.push_back(nodeType);
            nodeList.push_back(nodeTemp);
            // For each relationship in this node
            for (XMLElement* currentRelationship = currentNode->FirstChildElement("relationship"); currentRelationship != NULL;
                currentRelationship = currentRelationship->NextSiblingElement("relationship")) 
            {
                // Get relationship details
                string relType(currentRelationship->Attribute("type"));
                string relTarget(currentRelationship->Attribute("target"));
                // Insert them into array
                vector<string> relationTemp;
                relationTemp.push_back(relType);
                relationTemp.push_back(nodeName);
                relationTemp.push_back(relTarget);
                relationshipList.push_back(relationTemp);
            }
            nodeCount++;
        }
    }
    for (int relNum = 0; relNum < relationshipList.size(); relNum++)
    {
        //cout << relationshipList[relNum][0] << " Between '" << relationshipList[relNum][1] 
        //    << "' and '" << relationshipList[relNum][2] << "'\n";
    }
    cout << "Nodes in " << diagramTitle << ":\n\n";
    for (int nodeNum = 0; nodeNum < nodeList.size(); nodeNum++)
    {
        //cout << nodeList[nodeNum][1] << ": '" << nodeList[nodeNum][0] << "'\n";
        if (nodeList[nodeNum][1].compare("Case") != -1) {
            //cout << nodeList[nodeNum][0] << "\n";

            // Spoofing Identity
            // Tampering
            // Repudiation
            // Information Disclosure
            // Denial of Service
            // Elevation of Privilege)

            // Create Misuse Cases
            // User - Information Disclosure
            createMisuseCase(misuseCaseList, "User", nodeList[nodeNum][0], "Leaks information");

            // External Mis-Actor - Spoofing Identity
            createMisuseCase(misuseCaseList, "External Mis-Actor", nodeList[nodeNum][0], "Falsify Identity");
            // External Mis-Actor - Tampering
            createMisuseCase(misuseCaseList, "External Mis-Actor", nodeList[nodeNum][0], "Tamper with Data");
            // External Mis-Actor - Information Disclosure
            createMisuseCase(misuseCaseList, "External Mis-Actor", nodeList[nodeNum][0], "Leaks information");
            // External Mis-Actor - Denial of Service
            createMisuseCase(misuseCaseList, "External Mis-Actor", nodeList[nodeNum][0], "Denial of Service");

            // Internal Mis-Actor - Tampering
            createMisuseCase(misuseCaseList, "Internal Mis-Actor", nodeList[nodeNum][0], "Tamper with Data");
            // Internal Mis-Actor - Repudiation
            createMisuseCase(misuseCaseList, "Internal Mis-Actor", nodeList[nodeNum][0], "Repudiation");
            // Internal Mis-Actor - Information Disclosure
            createMisuseCase(misuseCaseList, "Internal Mis-Actor", nodeList[nodeNum][0], "Leaks information");
            // Internal Mis-Actor - Elevation of Privilege
            createMisuseCase(misuseCaseList, "Internal Mis-Actor", nodeList[nodeNum][0], "Elevate Privilege");
        }
    }

    int misuseCaseCount = 0;
    for (int i = 0; i < misuseCaseList.size(); i++) {
        cout << misuseCaseList[i][0] << " threatens " << misuseCaseList[i][1] << " with " << misuseCaseList[i][2] << ".\n";
        misuseCaseCount++;
    }

    cout << misuseCaseCount;

    string key;
    cin >> key;
	return 0;
}
