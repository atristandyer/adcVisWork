#include "RefinedWaterLayer.h"

RefinedWaterLayer::RefinedWaterLayer()
{
	terrainLayer = 0;
	numNodesOriginal = 0;
	numNodesRefined = 0;
	numTS = 0;
	currTS = 3800;
	minOriginal = 0;
	maxOriginal = 0;
	minRefined = 0;
	maxRefined = 0;
	originalFort63Set = false;
	refinedFort63Set = false;
	terrainSet = false;
	mapped = false;
	flipZValue = false;
	timestepLoaded = false;
	glLoaded = false;
	outlineShaderSet = false;
	fillShaderSet = false;
	OutlineShader = 0;
	FillShader = 0;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
}


RefinedWaterLayer::~RefinedWaterLayer()
{
	if (glLoaded)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &VAOId);
		glDeleteBuffers(1, &VBOId);
		// IBO gets deleted by terrain
	}
	if (originalFort.is_open())
		originalFort.close();
	if (refinedFort.is_open())
		refinedFort.close();
}


void RefinedWaterLayer::Draw()
{
	if (glLoaded && outlineShaderSet && fillShaderSet)
	{
		glBindVertexArray(VAOId);

		// Draw Fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonOffset(2, 2);

		if (FillShader->Use() == 0)
			glDrawElements(GL_TRIANGLES, terrainLayer->numElementsOriginal*3, GL_UNSIGNED_INT, (GLvoid *)0);


		// Draw Outline
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonOffset(0, 0);

		if (OutlineShader->Use() == 0)
			glDrawElements(GL_TRIANGLES, terrainLayer->numElementsOriginal*3, GL_UNSIGNED_INT, (GLvoid *)0);

		glBindVertexArray(0);
		glUseProgram(0);
	}
}


void RefinedWaterLayer::LoadTimestep(int ts)
{
	clock_t start = clock();
	if (ReadTimestep(ts) == 0)
	{
		clock_t total = clock() - start;
		std::cout << double(total)/((double)CLOCKS_PER_SEC) << " seconds to read timestep" << std::endl;
		LoadDataToGPU();
		currTS = ts;
		std::cout << "Timestep: " << currTS << std::endl;
	} else {
		std::cout << "Error reading timestep " << ts << std::endl;
	}
}


int RefinedWaterLayer::SetOriginalFort63Location(std::string newLoc)
{
	if (!terrainSet)
		return 1;

	originalFort.open(newLoc.data());
	if (originalFort.is_open())
	{
		std::string line;
		int timesteps;
		std::getline(originalFort, line);
		std::getline(originalFort, line);
		std::stringstream(line) >> timesteps >> numNodesOriginal;
		if (numNodesOriginal != terrainLayer->numNodesOriginal)
			return 3;
		if (timesteps < numTS || numTS == 0)
			numTS = timesteps;
		originalElevations.clear();
		originalElevations.resize(numNodesOriginal);
	} else {
		return 2;
	}

	originalFort63Location = newLoc;
	originalFort63Set = true;

	if (refinedFort63Set)
	{
		MapFort63s();
		ReadTimestep(1);
		LoadDataToGPU();
	}
	return 0;
}


int RefinedWaterLayer::SetRefinedFort63Location(std::string newLoc)
{
	if (!terrainSet)
		return 1;

	refinedFort.open(newLoc.data());
	if (refinedFort.is_open())
	{
		std::string line;
		int timesteps;
		std::getline(refinedFort, line);
		std::getline(refinedFort, line);
		std::stringstream(line) >> timesteps >> numNodesRefined;
		if (numNodesRefined != terrainLayer->numNodesRefined)
			return 3;
		if (timesteps < numTS || numTS == 0)
			numTS = timesteps;
		refinedElevations.clear();
		refinedElevations.resize(numNodesRefined);
	} else {
		return 2;
	}

	refinedFort63Location = newLoc;
	refinedFort63Set = true;

	if (originalFort63Set)
	{
		FastMapFort63s();
//		MapFort63s();
		ReadTimestep(1);
		LoadDataToGPU();
	}
	return 0;
}


int RefinedWaterLayer::SetFillShader(GLShader *newShader)
{
	FillShader = newShader;
	fillShaderSet = true;
	return 0;
}


int RefinedWaterLayer::SetOutlineShader(GLShader *newShader)
{
	OutlineShader = newShader;
	outlineShaderSet = true;
	return 0;
}


void RefinedWaterLayer::SetTerrainLayer(RefinedTerrainLayer *newLayer)
{
	terrainLayer = newLayer;
	terrainSet = true;
}


int RefinedWaterLayer::GetCurrentTimestep()
{
	return currTS;
}


float RefinedWaterLayer::GetMinOriginal()
{
	return minOriginal;
}


float RefinedWaterLayer::GetMaxOriginal()
{
	return maxOriginal;
}


float RefinedWaterLayer::GetMinRefined()
{
	return minRefined;
}


float RefinedWaterLayer::GetMaxRefined()
{
	return maxRefined;
}


float RefinedWaterLayer::GetMinDifference()
{
	return minDiff;
}


float RefinedWaterLayer::GetMaxDifference()
{
	return maxDiff;
}


int RefinedWaterLayer::MapFort63s()
{
	if (!originalFort63Set || !refinedFort63Set)
		return 1;

	if (!originalFort.is_open())
		originalFort.open(originalFort63Location.data());
	if (originalFort.is_open())
	{
		int position;
		std::streamsize maxSize = std::numeric_limits<std::streamsize>::max();

		std::cout << "Mapping " << numTS <<" timesteps..." << std::endl;

		originalFort.seekg(0, std::ios_base::beg);
		originalFort.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		originalFort.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		originalMap.clear();

		for (int i=0; i<numTS; i++)
		{
			position = originalFort.tellg();
			originalMap.push_back(position);
			originalFort.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			for (int j=0; j<numNodesOriginal; j++)
			{
				originalFort.ignore(maxSize, '\n');
			}
		}
//		originalFort.close();
	}

	if (!refinedFort.is_open())
		refinedFort.open(refinedFort63Location.data());
	if (refinedFort.is_open())
	{
		int position;
		std::streamsize maxSize = std::numeric_limits<std::streamsize>::max();

		refinedFort.seekg(0, std::ios_base::beg);
		refinedFort.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		refinedFort.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		refinedMap.clear();

		for (int i=0; i<numTS; i++)
		{
			position = refinedFort.tellg();
			refinedMap.push_back(position);
			refinedFort.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			for (int j=0; j<numNodesRefined; j++)
			{
				refinedFort.ignore(maxSize, '\n');
			}
		}
//		refinedFort.close();
	}
	std::cout << "Done" << std::endl;
	mapped = true;
	return 0;
}


int RefinedWaterLayer::FastMapFort63s()
{
	clock_t startTime, totalTime;
	startTime = clock();

	if (!originalFort63Set || !refinedFort63Set)
		return 1;

	// Map original fort.63 quickly

	if (!originalFort.is_open())
		originalFort.open(originalFort63Location.data());

	if (originalFort.is_open())
	{
		int position, size, tsStart, tsTrash, nodesTrash, nspoolge;
		float dtdp, compVal;
		std::string line;
		std::streamsize maxSize = std::numeric_limits<std::streamsize>::max();

		std::cout << "Mapping " << numTS << " in original fort.63 file" << std::endl;

		originalFort.seekg(0, std::ios_base::beg);
		originalFort.ignore(maxSize, '\n');
		std::getline(originalFort, line);

		originalMap.clear();
		originalMap.reserve(numTS);
		position = originalFort.tellg();
		originalMap.push_back(position);

		std::stringstream(line) >> tsTrash >> nodesTrash >> dtdp >> nspoolge;
		compVal = ((float)nspoolge)/dtdp;

		originalFort.ignore(maxSize, '\n');
		tsStart = originalFort.tellg();
		for (int i=0; i<numNodesOriginal; i++)
		{
			originalFort.ignore(maxSize, '\n');
		}

		for (int i=0; i<numTS-1; i++)
		{
			position = originalFort.tellg(); // 1
			size = position-tsStart;
			std::getline(originalFort, line);
			tsStart = originalFort.tellg();
			std::stringstream(line) >> dtdp >> nspoolge;
			if (((float)nspoolge)/dtdp != compVal)
			{
				std::cout << "miss" << std::endl;
				originalFort.seekg(originalMap[originalMap.size()-1], std::ios_base::beg);
				originalFort.ignore(maxSize, '\n');
				tsStart = originalFort.tellg();
				for (int j=0; j<numNodesOriginal; j++)
				{
					originalFort.ignore(maxSize, '\n');
				}
				position = originalFort.tellg();
				originalMap.push_back(position);
				size = position-tsStart;
				originalFort.ignore(maxSize, '\n');
			} else {
				originalMap.push_back(position);
			}

			originalFort.seekg(size, std::ios_base::cur);
		}
	}

	// Map refined fort.63 quickly

	if (!refinedFort.is_open())
		refinedFort.open(refinedFort63Location.data());

	if (refinedFort.is_open())
	{
		int position, size, tsStart, tsTrash, nodesTrash, nspoolge;
		float dtdp, compVal;
		std::string line;
		std::streamsize maxSize = std::numeric_limits<std::streamsize>::max();

		std::cout << "Mapping " << numTS << " in refined fort.63 file" << std::endl;

		refinedFort.seekg(0, std::ios_base::beg);
		refinedFort.ignore(maxSize, '\n');
		std::getline(refinedFort, line);

		refinedMap.clear();
		refinedMap.reserve(numTS);
		position = refinedFort.tellg();
		refinedMap.push_back(position);

		std::stringstream(line) >> tsTrash >> nodesTrash >> dtdp >> nspoolge;
		compVal = ((float)nspoolge)/dtdp;

		refinedFort.ignore(maxSize, '\n');
		tsStart = refinedFort.tellg();
		for (int i=0; i<numNodesRefined; i++)
		{
			refinedFort.ignore(maxSize, '\n');
		}

		for (int i=0; i<numTS-1; i++)
		{
			position = refinedFort.tellg(); // 1
			size = position-tsStart;
			std::getline(refinedFort, line);
			tsStart = refinedFort.tellg();
			std::stringstream(line) >> dtdp >> nspoolge;
			if (((float)nspoolge)/dtdp != compVal)
			{
				refinedFort.seekg(refinedMap[refinedMap.size()-1], std::ios_base::beg);
				refinedFort.ignore(maxSize, '\n');
				tsStart = refinedFort.tellg();
				for (int j=0; j<numNodesRefined; j++)
				{
					refinedFort.ignore(maxSize, '\n');
				}
				position = refinedFort.tellg();
				refinedMap.push_back(position);
				size = position-tsStart;
				refinedFort.ignore(maxSize, '\n');
			} else {
				refinedMap.push_back(position);
			}

			refinedFort.seekg(size, std::ios_base::cur);
		}
	}

	std::cout << "Done" << std::endl;
	mapped = true;

	totalTime = clock() - startTime;
	std::cout << double(totalTime)/((double)CLOCKS_PER_SEC) << " seconds to map" << std::endl;

	return 0;
}


int RefinedWaterLayer::ReadTimestep(int ts)
{
	if (!mapped)
		return 1;

	if (!originalFort.is_open())
		originalFort.open(originalFort63Location.data());
	if (originalFort.is_open())
	{
		int trash;

		originalFort.seekg(originalMap[ts-1], std::ios_base::beg);
		originalFort.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		for (int i=0; i<numNodesOriginal; i++)
		{
			originalFort >> trash;
			originalFort >> originalElevations[i];
			if (flipZValue)
				originalElevations[i] *= -1.0;
			if (originalElevations[i] != -99999 && originalElevations[i] < minOriginal)
				minOriginal = originalElevations[i];
			else if (originalElevations[i] > maxOriginal)
				maxOriginal = originalElevations[i];

		}
//		originalFort.close();
	} else {
		return 2;
	}

	if (!refinedFort.is_open())
		refinedFort.open(refinedFort63Location.data());
	if (refinedFort.is_open())
	{
		int trash;
		int originalNode;
		float diff;

		refinedFort.seekg(refinedMap[ts-1], std::ios_base::beg);
		refinedFort.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		for (int i=0; i<numNodesRefined; i++)
		{
			refinedFort >> trash;
			refinedFort >> refinedElevations[i];
			if (flipZValue)
				refinedElevations[i] *= -1.0;
			if (refinedElevations[i] != -99999 && refinedElevations[i] < minRefined)
			{
				minRefined = refinedElevations[i];
				originalNode = terrainLayer->MapToOriginal(i+1);
				if (originalNode != 0 && originalElevations[originalNode+1] != -99999)
				{
					diff = refinedElevations[i] - originalElevations[originalNode+1];
					if (diff < minDiff)
						minDiff = diff;
					else if (diff > maxDiff)
						maxDiff = diff;
				}
			}
			else if (refinedElevations[i] > maxRefined)
				maxRefined = refinedElevations[i];
		}
//		refinedFort.close();
	} else {
		return 3;
	}
	timestepLoaded = true;
	return 0;
}


int RefinedWaterLayer::LoadDataToGPU()
{
	if (!timestepLoaded || !terrainSet)
		return 1;

	if (!glLoaded)
	{
		const size_t VertexBufferSize = 5*sizeof(GLfloat)*numNodesOriginal;

		glGenVertexArrays(1, &VAOId);
		glGenBuffers(1, &VBOId);
		IBOId = terrainLayer->IBOId;

		glBindVertexArray(VAOId);
		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
		glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
		glBindVertexArray(0);

		GLenum errorCheck = glGetError();
		if (errorCheck != GL_NO_ERROR)
		{
			glLoaded = false;
			return 1;
		}
	}

	glBindVertexArray(VAOId);
	GLfloat * glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glNodeData != 0)
	{
		for (int i=0; i<numNodesOriginal; i++)
		{
			glNodeData[4*i+0] = (GLfloat)terrainLayer->nodeList[i].x;
			glNodeData[4*i+1] = (GLfloat)terrainLayer->nodeList[i].y;
			glNodeData[4*i+2] = (GLfloat)originalElevations[i];
			glNodeData[4*i+3] = (GLfloat)refinedElevations[terrainLayer->MapToRefined(i+1)-1];
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
	glLoaded = true;
	return 0;

}

