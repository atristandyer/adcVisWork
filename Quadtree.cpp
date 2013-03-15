#include <iostream>
#include "Quadtree.h"

//#define DEBUG_OUTPUT

Quadtree::Quadtree(std::vector<Node> nodes, int size, float minX, float maxX, float minY, float maxY)
{

	nodeList = nodes;
	binSize = size;

	// Create root branch
	root = newBranch(minX, maxX, minY, maxY);

	if (binSize > 0)
	{
		for (unsigned int i=0; i<nodeList.size(); i++)
		{
			addNode(&nodeList[i], root);
		}
	}

#ifdef DEBUG_OUTPUT
	std::cout << "Num branches: " << branchList.size() << std::endl;
	std::cout << "Num leaves: " << leafList.size() << std::endl;
#endif
}


Quadtree::~Quadtree()
{
	for (unsigned int i=0; i<leafList.size(); i++)
	{
		if (leafList[i] != 0)
			delete leafList[i];
	}

	for (unsigned int i=0; i<branchList.size(); i++)
	{
		if (branchList[i] != 0)
			delete branchList[i];
	}
}


Node *Quadtree::FindNode(float x, float y, branch *currBranch)
{
	// Searching is initiated with currBranch = 0 by the user
	if (currBranch == 0)
		return FindNode(x, y, root);

	// If point falls into a sub-branch, start recursion by calling
	// this function on that sub-branch
	for (int i=0; i<4; i++)
	{
		if (currBranch->branches[i] != 0 && pointIsInside(currBranch->branches[i], x, y))
		{
			return FindNode(x, y, currBranch->branches[i]);
		}
	}

	// If point falls into a leaf, find the closest Node to the
	// provided point and return it
	for (int i=0; i<4; i++)
	{
		if (currBranch->leaves[i] != 0 && pointIsInside(currBranch->leaves[i], x, y) && currBranch->leaves[i]->nodes.size() > 0)
		{
			Node *currClosest = currBranch->leaves[i]->nodes[0];
			float newDistance, currDistance = distance(currBranch->leaves[i]->nodes[0], x, y);

			for (unsigned int j=1; j<currBranch->leaves[i]->nodes.size(); j++)
			{
				newDistance = distance(currBranch->leaves[i]->nodes[j], x, y);
				if (newDistance < currDistance)
				{
					currClosest = currBranch->leaves[i]->nodes[j];
					currDistance = newDistance;
				}
			}
			return currClosest;
		}
	}

	// The point is not inside the quadtree bounds or there are no nodes
	// in the deepest level reached (possible improvements - continue searching
	// other leaves/branches if the parser reaches a leaf with no nodes)
	return 0;
}


leaf *Quadtree::newLeaf(float l, float r, float b, float t)
{
	// Create leaf in heap and put pointer into leaf list
	leaf *currLeaf = new leaf;
	leafList.push_back(currLeaf);

	// Set the boundaries of the leaf
	currLeaf->bounds[0] = l;
	currLeaf->bounds[1] = r;
	currLeaf->bounds[2] = b;
	currLeaf->bounds[3] = t;

#ifdef DEBUG_OUTPUT
	std::cout << "Created new leaf with bounds: " << l << ", " << r << ", " << b << ", " << t << std::endl;
#endif

	return currLeaf;
}


branch *Quadtree::newBranch(float l, float r, float b, float t)
{
	// Create branch in heap and put pointer into branch list
	branch *currBranch = new branch;
	branchList.push_back(currBranch);

	// Set the boundaries of the branch
	currBranch->bounds[0] = l;
	currBranch->bounds[1] = r;
	currBranch->bounds[2] = b;
	currBranch->bounds[3] = t;

#ifdef DEBUG_OUTPUT
	std::cout << "Created new branch with bounds: " << l << ", " << r << ", " << b << ", " << t << std::endl;
#endif

	// Set all branch pointers in the branch to 0
	for (int i=0; i<4; i++)
	{
		currBranch->branches[i] = 0;
	}

	// Create all four leaves in the branch
	float x = l+(r-l)/2.0;	// middle x-value
	float y = b+(t-b)/2.0;	// middle y-value
	currBranch->leaves[0] = newLeaf(x, r, y, t);
	currBranch->leaves[1] = newLeaf(l, x, y, t);
	currBranch->leaves[2] = newLeaf(l, x, b, y);
	currBranch->leaves[3] = newLeaf(x, r, b, y);

	return currBranch;
}


branch *Quadtree::leafToBranch(leaf *currLeaf)
{

#ifdef DEBUG_OUTPUT
	std::cout << "Turning leaf into branch" << std::endl;
#endif

	// Create new branch with the same bounds as the old leaf
	branch *currBranch = newBranch(currLeaf->bounds[0], currLeaf->bounds[1], currLeaf->bounds[2], currLeaf->bounds[3]);

	// Add all of the nodes that were in the old leaf to the new branch
	for (unsigned int i=0; i<currLeaf->nodes.size(); i++)
	{
		addNode(currLeaf->nodes[i], currBranch);
	}

	// Remove the old leaf pointer from the leaf list (just change to 0, removing is inefficient) and delete it
	for (unsigned int i=0; i<leafList.size(); i++)
	{
		if (leafList[i] == currLeaf && leafList[i] != 0)
		{
			leafList[i] = 0;
			delete currLeaf;
		}
	}

	return currBranch;
}


branch *Quadtree::addNode(Node *currNode, leaf *currLeaf)
{
	// Make sure the leaf has room for the new node. If so, add it. Otherwise,
	// turn the leaf into a branch and add the node to the new branch.
	if ((int)currLeaf->nodes.size() < binSize)
	{
		currLeaf->nodes.push_back(currNode);
		return 0;
	} else {
		branch *currBranch = leafToBranch(currLeaf);
		addNode(currNode, currBranch);
		return currBranch;
	}
}


void Quadtree::addNode(Node *currNode, branch *currBranch)
{
	// Loop through the branches to find a fit
	for (int i=0; i<4; i++)
	{
		if (currBranch->branches[i] != 0)
		{
			// Found a fit, add the node to this branch
			if (nodeIsInside(currNode, currBranch->branches[i]))
			{
				addNode(currNode, currBranch->branches[i]);
				return;
			}
		}
	}

	// Couldn't find a branch to go into, now look for a leaf to go into
	for (int i=0; i<4; i++)
	{
		if (currBranch->leaves[i] != 0)
		{
			// Found a fit, add the node to this leaf
			if (nodeIsInside(currNode, currBranch->leaves[i]))
			{
				branch *result = addNode(currNode, currBranch->leaves[i]);

				// Result is 0, so leaf still exists
				if (result == 0)
					return;

				// Leaf was turned into a branch, so update the current branch to include it
				else
				{
					currBranch->leaves[i] = 0;
					currBranch->branches[i] = result;
					return;
				}
			}
		}
	}

	// This node doesn't fit anywhere, let the user know
	std::cout << "Node " << currNode->nodeNumber << " not added: does not fit into quadtree bounds - (" << currNode->x << ", " << currNode->y << ")" << std::endl;
}


bool Quadtree::nodeIsInside(Node *currNode, leaf *currLeaf)
{
	if (currNode->x <= currLeaf->bounds[1] && currNode->x >= currLeaf->bounds[0])
		if (currNode->y <= currLeaf->bounds[3] && currNode->y >= currLeaf->bounds[2])
			return true;
	return false;
}


bool Quadtree::nodeIsInside(Node *currNode, branch *currBranch)
{
	if (currNode->x <= currBranch->bounds[1] && currNode->x >= currBranch->bounds[0])
		if (currNode->y <= currBranch->bounds[3] && currNode->y >= currBranch->bounds[2])
			return true;
	return false;
}


float Quadtree::distance(Node *currNode, float x, float y)
{
	return sqrt(pow(y-currNode->y, 2)+pow(x-currNode->x, 2));
}


bool Quadtree::pointIsInside(leaf *currLeaf, float x, float y)
{
	if (x <= currLeaf->bounds[1] && x >= currLeaf->bounds[0])
		if (y <= currLeaf->bounds[3] && y >= currLeaf->bounds[2])
			return true;
	return false;
}


bool Quadtree::pointIsInside(branch *currBranch, float x, float y)
{
	if (x <= currBranch->bounds[1] && x >= currBranch->bounds[0])
		if (y <= currBranch->bounds[3] && y >= currBranch->bounds[2])
			return true;
	return false;
}







































