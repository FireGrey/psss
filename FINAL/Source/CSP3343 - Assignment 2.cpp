// CSP3343 - Assignment 2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "tinyxml2\tinyxml2.h"
#include "tinyxml2\tinyxml2.cpp"
#include "node.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>  

//using namespace tinyxml2;
using namespace std;

void createMisuseCase(vector<vector<string>> &misuseCaseList, string actor, string node, string type) {
    vector<string> misuseTemp;
    misuseTemp.push_back(actor);
    misuseTemp.push_back(node);
    misuseTemp.push_back(type);
    misuseCaseList.push_back(misuseTemp);
}

Node createMaximiser(vector<vector<int>> strideWeightRanges, int loop) {
	Node maximiser;
	if (loop == 8) {
		Node leaf(rand() % (strideWeightRanges[loop][1] - strideWeightRanges[loop][0]) + strideWeightRanges[loop][0], loop);
		maximiser.addChild(leaf);
	}
	else 
	{
		Node leaf1(rand() % (strideWeightRanges[loop][1] - strideWeightRanges[loop][0]) + strideWeightRanges[loop][0], loop);
		maximiser.addChild(leaf1);
		Node leaf2(rand() % (strideWeightRanges[loop + 1][1] - strideWeightRanges[loop + 1][0]) + strideWeightRanges[loop + 1][0], loop + 1);
		maximiser.addChild(leaf2);
	}
	return maximiser;
}

Node createMinimiser(vector<vector<int>> strideWeightRanges, int loop) {
	Node minimiser;
	if (loop == 8) {
		Node max = createMaximiser(strideWeightRanges, loop);
		minimiser.addChild(max);
	}
	else 
	{
		Node max1 = createMaximiser(strideWeightRanges, loop);
		minimiser.addChild(max1);
		Node max2 = createMaximiser(strideWeightRanges, loop + 2);
		minimiser.addChild(max2);
	}
	return minimiser;
}

Node createStrideTree(vector<vector<int>> strideWeightRanges) {
	Node root;
	Node minimiser = createMinimiser(strideWeightRanges, 0);
	root.addChild(minimiser);
	minimiser = createMinimiser(strideWeightRanges, 4);
	root.addChild(minimiser);
	minimiser = createMinimiser(strideWeightRanges, 8);
	root.addChild(minimiser);

	return root;
}

/*vector<vector<string>> alphaBeta(Node node, int depth, int alpha, int beta, bool maximizingPlayer) {
	vector<vector<string>> validMisuseCases;

	if (depth == 0 || node.numChildren() == 0) {

	}

	return validMisuseCases;
}*/

int alphaBeta(vector<vector<string>> misuseCaseList, int loop, vector<vector<string>> &validMisuseCases, Node node, int depth, int alpha, int beta, bool maximizingPlayer) {
	if (depth == 0 || node.numChildren() == 0) {
		validMisuseCases.push_back(misuseCaseList[loop + node.getStrideNum()]);
		return node.getValue();
	}
	if (maximizingPlayer) {
		node.setValue(-101);
		for (unsigned int i = 0; i < node.numChildren(); i++) {
			node.setValue(max(node.getValue(), alphaBeta(misuseCaseList, loop, validMisuseCases, node.child(i), depth - 1, alpha, beta, false)));
			alpha = max(alpha, node.getValue());
			if (beta <= alpha) {
				break;
			}
		}
		return node.getValue();
	}
	else
	{
		node.setValue(101);
		for (unsigned int i = 0; i < node.numChildren(); i++) {
			node.setValue(min(node.getValue(), alphaBeta(misuseCaseList, loop, validMisuseCases, node.child(i), depth - 1, alpha, beta, true)));
			beta = min(beta, node.getValue());
			if (beta <= alpha) {
				break;
			}
		}
		return node.getValue();
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	tinyxml2::XMLDocument useCaseDiagram;
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
	vector<vector<string>> validMisuseCaseList;

	//Misuse case weights
    //User:
    // 0 - Leaks information        16, 55
    //External Mis-actor:
    // 1 - Spoofing Identity        26, 47
    // 2 - Tamper with Data         48, 99
    // 3 - Leaks information        17, 64
    // 4 - Denial of Service        38, 99
    //Internal Mis-actor:
    // 5 - Tamper with Data         3, 46
    // 6 - Repudiation              11, 70
    // 7 - Leaks information        3, 46
    // 8 - Elevation of Privilege   3, 46
	vector<vector<int>> strideWeightRanges = {	{ 16, 55 }, 
												{ 26, 47 }, { 48, 99 }, { 17, 64 }, { 38, 99 }, 
												{ 3, 46 }, { 11, 70 }, { 3, 46 }, { 3, 46 } };

    if (useCaseDiagram.Error()) {
        printf("Cannot Parse Xml File");
    }
    else 
    {
		tinyxml2::XMLElement* diagram = useCaseDiagram.FirstChildElement("diagram");
        diagramTitle = diagram->Attribute("title");

        // For each node
		for (tinyxml2::XMLElement* currentNode = diagram->FirstChildElement("node"); currentNode != NULL;
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
			for (tinyxml2::XMLElement* currentRelationship = currentNode->FirstChildElement("relationship"); currentRelationship != NULL;
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
    for (unsigned int relNum = 0; relNum < relationshipList.size(); relNum++)
    {
        //cout << relationshipList[relNum][0] << " Between '" << relationshipList[relNum][1] 
        //    << "' and '" << relationshipList[relNum][2] << "'\n";
    }
    cout << "Running misuse case generation tool on " << diagramTitle << ".\n\n";
	for (unsigned int nodeNum = 0; nodeNum < nodeList.size(); nodeNum++)
    {
        //cout << nodeList[nodeNum][1] << ": '" << nodeList[nodeNum][0] << "'\n";
        if (nodeList[nodeNum][1].compare("Case") != -1) {
            //cout << nodeList[nodeNum][0] << "\n";

            // Spoofing Identity
            // Tampering
            // Repudiation
            // Information Disclosure
            // Denial of Service
            // Elevation of Privilege

            // Create Misuse Cases
            // User - Information Disclosure
            createMisuseCase(misuseCaseList, "User", nodeList[nodeNum][0], "Leaks information");

            // External Mis-Actor - Spoofing Identity
            createMisuseCase(misuseCaseList, "External Mis-Actor", nodeList[nodeNum][0], "Spoofing Identity");
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
            createMisuseCase(misuseCaseList, "Internal Mis-Actor", nodeList[nodeNum][0], "Elevation of Privilege");
        }
    }

	cout << "-----------------------Candidate Misuse Cases-----------------------\n\n";
    int misuseCaseCount = 0;
	for (unsigned int i = 0; i < misuseCaseList.size(); i++) {
        cout << misuseCaseList[i][0] << " threatens " << misuseCaseList[i][1] << " with " << misuseCaseList[i][2] << ".\n";
        misuseCaseCount++;
    }

    cout << "\n" << misuseCaseCount << " Candidate misuse cases generated\n\n";
	cout << "------------------------Pruned Misuse Cases------------------------\n";
	int validMisuseCaseCount = 0;

	//Test loop to see if prunings change
	for (int i = 0; i < 1; i++) { // Change number here for testing
		//cout << "\nPruning";

		validMisuseCaseList.clear();
		// Loop through each use case
		for (unsigned int i = 0; i < misuseCaseList.size(); i += 9) {
			// Construct the tree
			Node strideTree = createStrideTree(strideWeightRanges);
			// Prune it to get the valid misuse cases
			alphaBeta(misuseCaseList, i, validMisuseCaseList, strideTree, 3, -101, 101, true);
			// Save these misuse cases to the main array (validMisuseCaseList)

			//cout << ".";

		}
		validMisuseCaseCount = validMisuseCaseList.size();
		cout << "\n" << validMisuseCaseCount << " Valid misuse cases found (" << (misuseCaseCount - validMisuseCaseCount) << " invalid)";

	}
	

	cout << "\n\n";
	// Loop through and print valid misuse cases here
	for (unsigned int i = 0; i < validMisuseCaseCount; i++) {
		cout << validMisuseCaseList[i][0] << " threatens " << validMisuseCaseList[i][1] << " with " << validMisuseCaseList[i][2] << ".\n";
	}


    string key;
    std::cin >> key;
	return 0;
}
