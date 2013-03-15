#include "RefinedTerrainLayer.h"

RefinedTerrainLayer::RefinedTerrainLayer()
{
	draw = true;

	numNodesOriginal = 0;
	numNodesRefined = 0;
	numElementsOriginal = 0;
	numElementsRefined = 0;

	originalFort14Set = false;
	refinedFort14Set = false;
	flipZValue = true;
	fileLoaded = false;
	glLoaded = false;
	originalOutlineShaderSet = false;
	originalFillShaderSet = false;
	refinedOutlineShaderSet = false;
	refinedFillShaderSet = false;

	originalOutlineShader = 0;
	originalFillShader = 0;
	refinedOutlineShader = 0;
	refinedFillShader = 0;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
}


RefinedTerrainLayer::~RefinedTerrainLayer()
{
	if (glLoaded)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &VBOId);
		glDeleteBuffers(1, &VAOId);
		glDeleteBuffers(1, &IBOId);
	}
}


void RefinedTerrainLayer::Draw()
{
	if (glLoaded && draw)
	{
		glBindVertexArray(VAOId);
		if (originalFillShaderSet && originalOutlineShaderSet)
		{
			// Draw Fill
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glPolygonOffset(8, 8);

			if (originalFillShader->Use() == 0)
				glDrawElements(GL_TRIANGLES, numElementsOriginal*3, GL_UNSIGNED_INT, (GLvoid*)0);

			// Draw Outline
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPolygonOffset(2, 2);

			if (originalOutlineShader->Use() == 0)
				glDrawElements(GL_TRIANGLES, numElementsOriginal*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		if (refinedFillShaderSet && refinedOutlineShaderSet)
		{
			// Draw Fill
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glPolygonOffset(6, 6);

			if (refinedFillShader->Use() == 0)
				glDrawElements(GL_TRIANGLES, 3*(elementList.size()-numElementsOriginal), GL_UNSIGNED_INT, (GLvoid*)(0+numElementsOriginal*3*sizeof(GLuint)));

			// Draw Outline
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPolygonOffset(0, 0);

			if (refinedOutlineShader->Use() == 0)
				glDrawElements(GL_TRIANGLES, 3*(elementList.size()-numElementsOriginal), GL_UNSIGNED_INT, (GLvoid*)(0+numElementsOriginal*3*sizeof(GLuint)));
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}
}


float RefinedTerrainLayer::GetMinElevation()
{
	return minCoords[2];
}


float RefinedTerrainLayer::GetMaxElevation()
{
	return maxCoords[2];
}


int RefinedTerrainLayer::SetOriginalFort14(std::string newLoc)
{
	originalFort14Location = newLoc;
	originalFort14Set = true;

	if (refinedFort14Set)
	{
		ReadFort14();
		Normalize();
		ConvertUnits();
		LoadDataToGPU();
	}
	return 0;
}


int RefinedTerrainLayer::SetRefinedFort14(std::string newLoc)
{
	refinedFort14Location = newLoc;
	refinedFort14Set = true;

	if (originalFort14Set)
	{
		std::cout << ReadFort14() << std::endl;
		Normalize();
		ConvertUnits();
		LoadDataToGPU();
	}
	return 0;
}


int RefinedTerrainLayer::SetOriginalFillShader(GLShader *newShader)
{
	originalFillShader = newShader;
	originalFillShaderSet = true;
	return 0;
}


int RefinedTerrainLayer::SetOriginalOutlineShader(GLShader *newShader)
{
	originalOutlineShader = newShader;
	originalOutlineShaderSet = true;
	return 0;
}


int RefinedTerrainLayer::SetRefinedFillShader(GLShader *newShader)
{
	refinedFillShader = newShader;
	refinedFillShaderSet = true;
	return 0;
}


int RefinedTerrainLayer::SetRefinedOutlineShader(GLShader *newShader)
{
	refinedOutlineShader = newShader;
	refinedOutlineShaderSet = true;
	return 0;
}


int RefinedTerrainLayer::ReadFort14()
{
	if (!originalFort14Set || !refinedFort14Set)
		return 1;

	std::ifstream ofort14 (originalFort14Location.data());
	if (ofort14.is_open())
	{
		std::string line;
		std::getline(ofort14, line);
		std::getline(ofort14, line);
		std::stringstream(line) >> numElementsOriginal >> numNodesOriginal;

		nodeList.reserve(numNodesOriginal);
		elementList.reserve(numElementsOriginal);

		Node currNode;
		for (int i=0; i<numNodesOriginal; i++)
		{
			ofort14 >> currNode.nodeNumber;
			ofort14 >> currNode.x;
			ofort14 >> currNode.y;
			ofort14 >> currNode.z;
			if (flipZValue)
				currNode.z *= -1.0;
			nodeList.push_back(currNode);
		}

		Element currElement;
		int trash;
		for (int i=0; i<numElementsOriginal; i++)
		{
			ofort14 >> currElement.elementNumber;
			ofort14 >> trash;
			ofort14 >> currElement.n1;
			ofort14 >> currElement.n2;
			ofort14 >> currElement.n3;
			elementList.push_back(currElement);
		}
		ofort14.close();

		std::ifstream rfort14 (refinedFort14Location.data());
		if (rfort14.is_open())
		{
			std::string line;
			std::getline(rfort14, line);
			std::getline(rfort14, line);
			std::stringstream(line) >> numElementsRefined >> numNodesRefined;

			int originalNodeNum;
			for (int i=0; i<numNodesRefined; i++)
			{
				rfort14 >> currNode.nodeNumber;
				rfort14 >> currNode.x;
				rfort14 >> currNode.y;
				rfort14 >> currNode.z;
				if (flipZValue)
					currNode.z *= -1.0;


				originalNodeNum = IsInOriginalNodeList(&currNode);
				if (originalNodeNum)
				{
					refinedToOriginal[currNode.nodeNumber] = originalNodeNum;
					originalToRefined[originalNodeNum] = currNode.nodeNumber;
					//
					refinedToOriginalMap.push_back(currNode.nodeNumber);
					refinedToOriginalMap.push_back(originalNodeNum);
				} else {
					refinedToOriginal[currNode.nodeNumber] = nodeList.size() + 1;
					originalToRefined[nodeList.size() + 1] = 0;
					//
					refinedToOriginalNewMap.push_back(currNode.nodeNumber);
					currNode.nodeNumber = nodeList.size() + 1;
					refinedToOriginalNewMap.push_back(currNode.nodeNumber);
					nodeList.push_back(currNode);
				}
			}

			for (int i=0; i<numElementsRefined; i++)
			{
				rfort14 >> currElement.elementNumber;
				rfort14 >> trash;
				rfort14 >> currElement.n1;
				rfort14 >> currElement.n2;
				rfort14 >> currElement.n3;

				if (ElementHasRefinedNode(&currElement))
				{
					currElement.elementNumber = elementList.size() + 1;
					currElement.n1 = MapToOriginal(currElement.n1);
					currElement.n2 = MapToOriginal(currElement.n2);
					currElement.n3 = MapToOriginal(currElement.n3);
					if (currElement.n1 == 0 || currElement.n2 == 0 || currElement.n3 == 0)
					{
						fileLoaded = false;
						return 3;
					}
					elementList.push_back(currElement);
				}
			}
			rfort14.close();

			std::cout << "Original Num Nodes: " << numNodesOriginal << std::endl;
			std::cout << "Refined Num Nodes: " << numNodesRefined << std::endl;
			std::cout << "Size of nodelist: " << nodeList.size() << std::endl;
			std::cout << "Original Num Elements: " << numElementsOriginal << std::endl;
			std::cout << "Refined Num Elements: " << numElementsRefined << std::endl;
			std::cout << "Size of elementList: " << elementList.size() << std::endl;

			fileLoaded = true;
			return 0;
		}
		fileLoaded = false;
		return 1;
	}
	fileLoaded = false;
	return 2;
}


int RefinedTerrainLayer::LoadDataToGPU()
{
	if (fileLoaded)
	{
		const size_t VertexBufferSize = 4*sizeof(GLfloat)*nodeList.size();
		const size_t IndexBufferSize = 3*sizeof(GLuint)*elementList.size();

		glGenVertexArrays(1, &VAOId);
		glGenBuffers(1, &VBOId);
		glGenBuffers(1, &IBOId);

		glBindVertexArray(VAOId);

		// Send Vertex Data
		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
		glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_STATIC_DRAW);
		GLfloat * glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glNodeData != 0)
		{
			for (unsigned int i=0; i<nodeList.size(); i++)
			{
				glNodeData[4*i+0] = (GLfloat)nodeList[i].x;
				glNodeData[4*i+1] = (GLfloat)nodeList[i].y;
				glNodeData[4*i+2] = (GLfloat)nodeList[i].z;
				glNodeData[4*i+3] = (GLfloat)1.0;
			}
		} else {
			glLoaded = false;
			return 1;
		}

		if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
		{
			glLoaded = false;
			return 1;
		}

		// Send Index Data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
		GLuint * glElementData = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glElementData != 0)
		{
			for (unsigned int i=0; i<elementList.size(); i++)
			{
				glElementData[3*i+0] = (GLuint)elementList[i].n1-1;
				glElementData[3*i+1] = (GLuint)elementList[i].n2-1;
				glElementData[3*i+2] = (GLuint)elementList[i].n3-1;
			}
		} else {
			glLoaded = false;
			return 1;
		}

		if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
		{
			glLoaded = false;
			return 1;
		}

		glBindVertexArray(0);

		GLenum errorCheck = glGetError();
		if (errorCheck == GL_NO_ERROR)
		{
			glLoaded = true;
			return 0;
		} else {
			glLoaded = false;
			return 1;
		}
	} else {
		glLoaded = false;
		return 1;
	}
}


void RefinedTerrainLayer::ConvertUnits()
{
	if (fileLoaded)
	{
		for (unsigned int i=0; i<nodeList.size(); i++)
		{
			nodeList[i].x *= EARTH_RADIUS;
			nodeList[i].y *= EARTH_RADIUS;
		}
	}
}


void RefinedTerrainLayer::Normalize()
{
	if (fileLoaded)
	{
		minCoords[0] = nodeList[0].x;
		minCoords[1] = nodeList[0].y;
		minCoords[2] = nodeList[0].z;
		maxCoords[0] = nodeList[0].x;
		maxCoords[1] = nodeList[0].y;
		maxCoords[2] = nodeList[0].z;

		float currCoords[3];

		for (unsigned int i=1; i<nodeList.size(); i++)
		{
			currCoords[0] = nodeList[i].x;
			currCoords[1] = nodeList[i].y;
			currCoords[2] = nodeList[i].z;

			for (int j=0; j<3; j++)
			{
				if (currCoords[j] < minCoords[j])
					minCoords[j] = currCoords[j];
				else if (currCoords[j] > maxCoords[j])
					maxCoords[j] = currCoords[j];
			}
		}

		for (int i=0; i<3; i++)
			midCoords[i] = minCoords[i] + (maxCoords[i]-minCoords[i])/2.0;

		float max;
		if (fabs(maxCoords[0]) >= fabs(minCoords[0]))
		{
			max = fabs(maxCoords[0])-fabs(midCoords[0]);
		} else {
			max = fabs(minCoords[0])-fabs(midCoords[0]);
		}

		if (fabs(maxCoords[1]) >= fabs(minCoords[1]) && fabs(maxCoords[1]) > max)
		{
			max = fabs(maxCoords[1])-fabs(midCoords[1]);
		} else if (fabs(minCoords[1]) >= fabs(maxCoords[1]) && fabs(minCoords[1]) > max)
		{
			max = fabs(minCoords[1])-fabs(midCoords[1]);
		}

		for (unsigned int i=0; i<nodeList.size(); i++)
		{
			nodeList[i].x = (nodeList[i].x-midCoords[0])/max;
			nodeList[i].y = (nodeList[i].y-midCoords[1])/max;
		}
	}
}


int RefinedTerrainLayer::IsInOriginalNodeList(Node *currNode)
{
	// First check the same node number just in case - a hit will speed things up big time
	if (currNode->nodeNumber < numNodesOriginal)
	{
		bool xMatch, yMatch;
		xMatch = currNode->x == nodeList[currNode->nodeNumber-1].x;
		yMatch = currNode->y == nodeList[currNode->nodeNumber-1].y;
		if (xMatch && yMatch)
			return currNode->nodeNumber;
	}

	for (int i=0; i<numNodesOriginal; i++)
	{
		if (currNode->x == nodeList[i].x)
			if (currNode->y == nodeList[i].y)
				return nodeList[i].nodeNumber;
	}

	return 0;
}


int RefinedTerrainLayer::ElementHasRefinedNode(Element *currElement)
{
	for (unsigned int i=0; i<refinedToOriginalNewMap.size()/2.0; i++)
	{
		if (currElement->n1 == refinedToOriginalNewMap[2*i] || currElement->n2 == refinedToOriginalNewMap[2*i] || currElement->n3 == refinedToOriginalNewMap[2*i])
			return 1;
	}
	return 0;
}


int RefinedTerrainLayer::MapToOriginal(unsigned int nodeNum)
{
	std::map<unsigned int, unsigned int>::iterator result = refinedToOriginal.find(nodeNum);
	if (result != refinedToOriginal.end())
		return result->second;
	return 0;
}


int RefinedTerrainLayer::MapToRefined(unsigned int nodeNum)
{
	std::map<unsigned int, unsigned int>::iterator result = originalToRefined.find(nodeNum);
	if (result != originalToRefined.end())
		return result->second;
	return 0;
}
