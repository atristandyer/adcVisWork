#include "TerrainLayer.h"

TerrainLayer::TerrainLayer()
{
	fort14Set = false;
	flipZValue = true;
	fileLoaded = false;
	glLoaded = false;
	outlineShaderSet = false;
	fillShaderSet = false;
	pointShaderSet = false;
	elementShaderSet = false;

	numNodes = 0;
	numElements = 0;
	minCoords[0] = 0.0;
	minCoords[1] = 0.0;
	minCoords[2] = 0.0;
	maxCoords[0] = 0.0;
	maxCoords[1] = 0.0;
	maxCoords[2] = 0.0;
	max = 0.0;

	quadtree = 0;
	selectedNode = 0;
	selectedElement = 0;

	outlineShader = 0;
	fillShader = 0;
	pointShader = 0;
	elementShader = 0;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
}


TerrainLayer::~TerrainLayer()
{
	if (glLoaded)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &VBOId);
		glDeleteBuffers(1, &VAOId);
		glDeleteBuffers(1, &IBOId);
	}

	if (quadtree)
		delete quadtree;
}


void TerrainLayer::Draw()
{
	if (glLoaded && outlineShaderSet && fillShaderSet)
	{
		glBindVertexArray(VAOId);

		// Draw Fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonOffset(6, 6);
		if (fillShader && fillShader->Use() == 0)
			glDrawElements(GL_TRIANGLES, numElements*3, GL_UNSIGNED_INT, (GLvoid*)0);


		// Draw Outline
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonOffset(4, 4);
		if (outlineShader && outlineShader->Use() == 0)
			glDrawElements(GL_TRIANGLES, numElements*3, GL_UNSIGNED_INT, (GLvoid*)0);


		// Draw selected node
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPolygonOffset(0, 0);
		if (pointShaderSet && selectedNode > 0 && pointShader->Use() == 0)
			glDrawArrays(GL_POINTS, selectedNode-1, 1);


		// Draw selected element
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonOffset(0, 0);
		if (elementShaderSet && selectedElement > 0 && elementShader->Use() == 0)
		{
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLuint*)0+3*(selectedElement-1));
		}

		glBindVertexArray(0);
		glUseProgram(0);

	}
}


float TerrainLayer::GetMinElevation()
{
	if (fileLoaded)
		return minCoords[2];
	return 0.0;
}


float TerrainLayer::GetMaxElevation()
{
	if (fileLoaded)
		return maxCoords[2];
	return 1.0;
}


int TerrainLayer::setFort14Location(std::string newLoc)
{
	fort14Location = newLoc;
	fort14Set = true;

	if (ReadFort14() != 0)
	{
		std::cout << "Error Reading fort.14" << std::endl;
		return 1;
	}

	Normalize();
	ConvertUnits();

	std::cout << nodeList[933].x << ", " << nodeList[933].y << std::endl; ////////////////////////////////////////

	if (LoadDataToGPU() != 0)
	{
		std::cout << "Error Loading Data to GPU" << std::endl;
		return 1;
	}

	quadtree = new Quadtree(nodeList, 500, minCoords[0], maxCoords[0], minCoords[1], maxCoords[1]);

//	std::cout << "Node 1: (" << nodeList[0].x << ", " << nodeList[0].y << ")" << std::endl;

	return 0;

}


int TerrainLayer::setOutlineShader(GLShader *newShader)
{
	outlineShader = newShader;
	outlineShaderSet = true;
	return 0;
}


int TerrainLayer::SetElementShader(GLShader *newShader)
{
	elementShader = newShader;
	elementShaderSet = true;
	return 0;
}


int TerrainLayer::setFillShader(GLShader *newShader)
{
	fillShader = newShader;
	fillShaderSet = true;
	return 0;
}


int TerrainLayer::setPointShader(GLShader *newShader)
{
	pointShader = newShader;
	pointShaderSet = true;
	return 0;
}


Node *TerrainLayer::pickNode(float x, float y)
{
	if (quadtree)
	{
		Node *result = quadtree->FindNode(x, y, 0);
		if (result && result->nodeNumber != -1)
		{
			std::cout << "Node #" << result->nodeNumber << std::endl;
			selectedNode = result->nodeNumber;
			return result;
		} else {
			selectedNode = 0;
		}
	}
	return 0;
}


Node *TerrainLayer::pickNode(int nodeNum)
{
	if (nodeNum > 0 && nodeNum <= (int)nodeList.size())
	{
		selectedNode = nodeNum;
		return &nodeList[nodeNum-1];
	}
	return 0;
}


Element *TerrainLayer::pickElement(float x, float y)
{

	// Possible optimization: put elements in quadtree as well

	if (quadtree)
	{
		Node *result = quadtree->FindNode(x, y, 0);
		if (result && result->nodeNumber != -1)
		{
			for (unsigned int i=0; i<elementList.size(); i++)
			{
				if ((int)elementList[i].n1 == result->nodeNumber || (int)elementList[i].n2 == result->nodeNumber || (int)elementList[i].n3 == result->nodeNumber)
				{
					if (isInside(x, y, &elementList[i]))
					{
						std::cout << "Element #" << elementList[i].elementNumber << std::endl;
						std::cout << "-------------------" << std::endl;
						selectedElement = elementList[i].elementNumber;
						return &elementList[i];
					}
				}
			}
			selectedElement = 0;
		} else {
			selectedElement = 0;
		}
	}
	return 0;
}


Element *TerrainLayer::pickElement(int elementNum)
{
	if (elementNum > 0 && elementNum <= (int)elementList.size())
	{
		selectedElement = elementNum;
		return &elementList[elementNum-1];
	}
	return 0;
}


void TerrainLayer::clearPicking()
{
	selectedNode = 0;
	selectedElement = 0;
}


Node *TerrainLayer::getNodePointer(int nodeNumber)
{
	if (nodeNumber > 0 && nodeNumber <= (int)nodeList.size())
		return &nodeList[nodeNumber-1];
	return 0;
}


int TerrainLayer::ReadFort14()
{
	if (fort14Set)
	{
		std::ifstream fort14 (fort14Location.data());
		if (fort14.is_open())
		{
			// Read Data From File
			std::string line;
			std::getline(fort14, line);
			std::getline(fort14, line);
			std::stringstream(line) >> numElements >> numNodes;

			nodeList.reserve(numNodes);
			elementList.reserve(numElements);

			Node currNode;
			for (int i=0; i<numNodes; i++)
			{
				fort14 >> currNode.nodeNumber;
				fort14 >> currNode.x;
				fort14 >> currNode.y;
				fort14 >> currNode.z;
				if (flipZValue)
					currNode.z *= -1.0;
				nodeList.push_back(currNode);
			}

			Element currElement;
			int trash;
			for (int i=0; i<numElements; i++)
			{
				fort14 >> currElement.elementNumber;
				fort14 >> trash;
				fort14 >> currElement.n1;
				fort14 >> currElement.n2;
				fort14 >> currElement.n3;
				elementList.push_back(currElement);
			}

			fort14.close();

			if (fort14.fail())
			{
				fileLoaded = false;
				return 1;
			}
			fileLoaded = true;
			return 0;
		} else {
			fileLoaded = false;
			return 1;
		}
	} else {
		fileLoaded = false;
		return 1;
	}
}


int TerrainLayer::LoadDataToGPU()
{
	if (fileLoaded)
	{
		const size_t VertexBufferSize = 4*sizeof(GLfloat)*numNodes;
		const size_t IndexBufferSize = 3*sizeof(GLuint)*numElements;

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
			for (int i=0; i<numNodes; i++)
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
			for (int i=0; i<numElements; i++)
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


void TerrainLayer::ConvertUnits()
{
	if (fileLoaded)
	{
		for (int i=0; i<numNodes; i++)
		{
			nodeList[i].x *= EARTH_RADIUS;
			nodeList[i].y *= EARTH_RADIUS;
		}
		for (int i=0; i<2; i++)
		{
			minCoords[i] *= EARTH_RADIUS;
			midCoords[i] *= EARTH_RADIUS;
			maxCoords[i] *= EARTH_RADIUS;
		}
	}
}


void TerrainLayer::Normalize()
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

		for (int i=0; i<numNodes; i++)
		{
			nodeList[i].x = (nodeList[i].x-midCoords[0])/max;
			nodeList[i].y = (nodeList[i].y-midCoords[1])/max;
		}

		minCoords[0] = (minCoords[0]-midCoords[0])/max;
		maxCoords[0] = (maxCoords[0]-midCoords[0])/max;
		minCoords[1] = (minCoords[1]-midCoords[1])/max;
		maxCoords[1] = (maxCoords[1]-midCoords[1])/max;

	}

//	for (int i=0; i<3; i++)
//	{
//		std::cout << "Min[" << i << "] - " << minCoords[i] << std::endl;
//		std::cout << "Mid[" << i << "] - " << midCoords[i] << std::endl;
//		std::cout << "Max[" << i << "] - " << maxCoords[i] << std::endl;
//	}
}


bool TerrainLayer::isInside(float x, float y, Element *e)
{
	float a = (nodeList[e->n2-1].x - nodeList[e->n1-1].x)*(y - nodeList[e->n1-1].y) - (x - nodeList[e->n1-1].x)*(nodeList[e->n2-1].y - nodeList[e->n1-1].y);
	float b = (nodeList[e->n3-1].x - nodeList[e->n2-1].x)*(y - nodeList[e->n2-1].y) - (x - nodeList[e->n2-1].x)*(nodeList[e->n3-1].y - nodeList[e->n2-1].y);
	float c = (nodeList[e->n1-1].x - nodeList[e->n3-1].x)*(y - nodeList[e->n3-1].y) - (x - nodeList[e->n3-1].x)*(nodeList[e->n1-1].y - nodeList[e->n3-1].y);

	if ((a > 0 && b > 0 && c > 0) || (a < 0 && b < 0 && c < 0))
		return true;
	return false;
}
