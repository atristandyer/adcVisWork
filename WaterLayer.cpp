#include "WaterLayer.h"

WaterLayer::WaterLayer()
{
	numNodes = 0;
	numTS = 0;
	currTS = 0;
	minZ = 0;
	maxZ = 0;
	fort63Set = false;
	terrainSet = false;
	mapped = false;
	flipZValue = false;
	timestepLoaded = false;
	glLoaded = false;
	outlineShaderSet = false;
	fillShaderSet = false;
	outlineShader = 0;
	fillShader = 0;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
}


WaterLayer::~WaterLayer()
{
	if (fort63.is_open())
		fort63.close();

	if (glLoaded)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &VAOId);
		glDeleteBuffers(1, &VBOId);
		// IBO gets deleted by terrain
	}
}


void WaterLayer::Draw()
{
	if (glLoaded && outlineShaderSet && fillShaderSet)
	{
		glBindVertexArray(VAOId);

		// Draw Fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonOffset(2, 2);

		if (fillShader->Use() == 0)
			glDrawElements(GL_TRIANGLES, terrainLayer->numElements*3, GL_UNSIGNED_INT, (GLvoid*)0);


		// Draw Outline
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonOffset(1, 1);

		if (outlineShader->Use() == 0)
			glDrawElements(GL_TRIANGLES, terrainLayer->numElements*3, GL_UNSIGNED_INT, (GLvoid*)0);

		glBindVertexArray(0);
		glUseProgram(0);
	}
}


void WaterLayer::LoadTimestep(int ts)
{
	if (ReadTimestep(ts) == 0)
		LoadDataToGPU();
}


int WaterLayer::setFort63Location(std::string newLoc)
{
	if (!terrainSet)
		return 1;

	// Make sure fort.63 file matches fort.14 file
	fort63.open(newLoc.data());
	if (fort63.is_open())
	{
		std::string line;
		std::getline(fort63, line);
		std::getline(fort63, line);
		std::stringstream(line) >> numTS >> numNodes;
		if (numNodes != terrainLayer->numNodes)
			return 3;

		elevationsList.clear();
		elevationsList.resize(numNodes);

	} else {
		return 2;
	}

	fort63Location = newLoc;
	fort63Set = true;

	if (FastMapFort63() == 0)
		if (ReadTimestep(1) == 0)
			std::cout << LoadDataToGPU();
		else
			std::cout << "Error reading ts" << std::endl;
	else
		std::cout << "Error mapping" << std::endl;

	return 0;
}


int WaterLayer::setOutlineShader(GLShader *newShader)
{
	outlineShader = newShader;
	outlineShaderSet = true;
	return 0;
}


int WaterLayer::setFillShader(GLShader *newShader)
{
	fillShader = newShader;
	fillShaderSet = true;
	return 0;
}


void WaterLayer::setTerrainLayer(TerrainLayer *newLayer)
{
	terrainLayer = newLayer;
	terrainSet = true;
}


int WaterLayer::GetCurrentTimestep()
{
	return currTS;
}


float WaterLayer::GetMinZ()
{
	return minZ;
}


float WaterLayer::GetMaxZ()
{
	return maxZ;
}


int WaterLayer::MapFort63()
{
	if (!fort63Set)
		return 1;

	if (!fort63.is_open())
		fort63.open(fort63Location.data());

	if (fort63.is_open())
	{
		int position;

		fort63.seekg(0, std::ios_base::beg);
		fort63.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		fort63.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		fileMap.clear();

		for (int i=0; i<numTS; i++)
		{
			position = fort63.tellg();
			fileMap.push_back(position);
			fort63.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			for (int j=0; j<numNodes; j++)
			{
				fort63.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}
	} else {
		return 1;
	}

	mapped = true;
	return 0;
}


int WaterLayer::FastMapFort63()
{
	if (!fort63Set)
	{
		mapped = false;
		return 1;
	}

	if (!fort63.is_open())
		fort63.open(fort63Location.data());

	if (fort63.is_open())
	{
		int position, size, tsStart, tsTrash, nodesTrash, nspoolge;
		float dtdp, compVal;
		std::string line;
		std::streamsize maxSize = std::numeric_limits<std::streamsize>::max();

		std::cout << "Mapping " << numTS << " timesteps in fort.63 file" << std::endl;

		fort63.seekg(0, std::ios_base::beg);
		fort63.ignore(maxSize, '\n');
		std::getline(fort63, line);

		fileMap.clear();
		fileMap.reserve(numTS);
		position = fort63.tellg();
		fileMap.push_back(position);

		std::stringstream(line) >> tsTrash >> nodesTrash >> dtdp >> nspoolge;
		compVal = ((float)nspoolge)/dtdp;

		fort63.ignore(maxSize, '\n');
		tsStart = fort63.tellg();
		for (int i=0; i<numNodes; i++)
		{
			fort63.ignore(maxSize, '\n');
		}

		for (int i=0; i<numTS-1; i++)
		{
			position = fort63.tellg();
			size = position-tsStart;
			std::getline(fort63, line);
			tsStart = fort63.tellg();
			std::stringstream(line) >> dtdp >> nspoolge;
			if (((float)nspoolge)/dtdp != compVal)
			{
				std::cout << "miss" << std::endl;
				fort63.seekg(fileMap[fileMap.size()-1], std::ios_base::beg);
				fort63.ignore(maxSize, '\n');
				tsStart = fort63.tellg();
				for (int j=0; j<numNodes; j++)
				{
					fort63.ignore(maxSize, '\n');
				}
				position = fort63.tellg();
				fileMap.push_back(position);
				size = position-tsStart;
				fort63.ignore(maxSize, '\n');
			} else {
				fileMap.push_back(position);
			}
			fort63.seekg(size, std::ios_base::cur);
		}
	} else {
		mapped = false;
		return 1;
	}
	mapped = true;
	return 0;

}


int WaterLayer::ReadTimestep(int ts)
{
	if (!mapped)
		return 1;

	if (ts < 1 || ts > numTS)
		return 3;

	if (!fort63.is_open())
		fort63.open(fort63Location.data());

	if (fort63.is_open())
	{
		int trash;

		fort63.seekg(fileMap[ts-1], std::ios_base::beg);
		fort63.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		minZ = 99999.0;
		maxZ = -99999.0;
		for (int i=0; i<numNodes; i++)
		{
			fort63 >> trash;
			fort63 >> elevationsList[i];
			if (flipZValue)
				elevationsList[i] *= -1.0;
			if (elevationsList[i] < minZ && elevationsList[i] != -99999)
				minZ = elevationsList[i];
			else if (elevationsList[i] > maxZ)
				maxZ = elevationsList[i];
		}
		currTS = ts;
		timestepLoaded = true;
	} else {
		return 2;
	}
	return 0;
}


int WaterLayer::LoadDataToGPU()
{
	if (!timestepLoaded || !terrainSet)
		return 1;

	if (!glLoaded)
	{
		const size_t VertexBufferSize = 4*sizeof(GLfloat)*numNodes;

		glGenVertexArrays(1, &VAOId);
		glGenBuffers(1, &VBOId);
		IBOId = terrainLayer->IBOId;

		glBindVertexArray(VAOId);

		// Send vertex data
		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
		glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_DYNAMIC_DRAW);
		GLfloat * glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glNodeData != 0)
		{
			for (int i=0; i<numNodes; i++)
			{
				glNodeData[4*i+0] = (GLfloat)terrainLayer->nodeList[i].x;
				glNodeData[4*i+1] = (GLfloat)terrainLayer->nodeList[i].y;
				glNodeData[4*i+2] = (GLfloat)elevationsList[i];
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

		// Use the same IBO as the terrain
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
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

		return 0;
	} else {
		glBindVertexArray(VAOId);
		GLfloat * glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glNodeData != 0)
		{
			for (int i=0; i<numNodes; i++)
			{
				glNodeData[4*i+0] = (GLfloat)terrainLayer->nodeList[i].x;
				glNodeData[4*i+1] = (GLfloat)terrainLayer->nodeList[i].y;
				glNodeData[4*i+2] = (GLfloat)elevationsList[i];
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
		glBindVertexArray(0);
		return 0;
	}

}
