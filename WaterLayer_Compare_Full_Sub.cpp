#include "WaterLayer_Compare_Full_Sub.h"

WaterLayer_Compare_Full_Sub::WaterLayer_Compare_Full_Sub()
{
	terrainLayer = 0;
	subNumNodes = 0;
	fullNumNodes = 0;
	numTS = 0;
	currTS = 0;
	minZ = 0;
	maxZ = 0;

	terrainLayerSet = false;
	subFort63Set = false;
	fullFort63Set = false;
	py140Set = false;
//	fullFort14Set = false;
	submapped = false;
	fullmapped = false;
	timestepmapped = false;
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


WaterLayer_Compare_Full_Sub::~WaterLayer_Compare_Full_Sub()
{
	if (subFort63.is_open())
		subFort63.close();
	if (fullFort63.is_open())
		fullFort63.close();

	if (glLoaded)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &VAOId);
		glDeleteBuffers(1, &VBOId);
		// IBO gets deleted by terrain
	}
}


void WaterLayer_Compare_Full_Sub::Draw()
{
	if (glLoaded && outlineShaderSet && fillShaderSet && terrainLayerSet)
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


void WaterLayer_Compare_Full_Sub::LoadTimestep(int ts)
{
	int result = ReadTimestep(ts);
	if (result == 0)
	{
		std::cout << "Read Timestep: " << ts << std::endl;
		std::cout << "Load to gpu result: " << LoadDataToGPU() << std::endl;
	} else {
		std::cout << "ReadTimestep Error: " << result << std::endl;
		GLenum errorCheck = glGetError();
		if (errorCheck != GL_NO_ERROR)
		{
			std::cout << "GL Error" << std::endl;
		}
	}
}


int WaterLayer_Compare_Full_Sub::SubSetFort63Location(std::string newLoc)
{
	if (!terrainLayerSet)
		return 1;

	// Make sure subdomain fort.63 matches fort.14 file
	subFort63.open(newLoc.data());
	if (subFort63.is_open())
	{
		int tempNumTS;
		std::string line;
		std::getline(subFort63, line);
		std::getline(subFort63, line);
		std::stringstream(line) >> tempNumTS >> subNumNodes;
		if (numTS == 0 || tempNumTS < numTS)
			numTS = tempNumTS;
		if (subNumNodes != terrainLayer->numNodes)
		{
			subFort63.close();
			return 3;
		}
		differencesList.clear();
	} else {
		return 2;
	}

	subFort63Location = newLoc;
	subFort63Set = true;

	MapFort63(&subFileMap, &subFort63, &submapped);

	return 0;
}


int WaterLayer_Compare_Full_Sub::FullSetFort63Location(std::string newLoc)
{
//	if (!fullFort14Set)
//		return 1;

//	// Get some data from the full fort.14 file
//	int numElementsFull;
//	std::ifstream fullFort14 (fullFort14Location.data());
//	if (fullFort14.is_open())
//	{
//		std::string line;
//		std::getline(fullFort14, line);
//		std::getline(fullFort14, line);
//		std::stringstream(line) >> numElementsFull >> fullNumNodes;
//	} else {
//		return 2;
//	}
//	fullFort14.close();

	// Make sure full domain fort.63 matches fort.14 file
	fullFort63.open(newLoc.data());
	if (fullFort63.is_open())
	{
		int tempNumTS, tempFullNumNodes;
		std::string line;
		std::getline(fullFort63, line);
		std::getline(fullFort63, line);
		std::stringstream(line) >> tempNumTS >> tempFullNumNodes;
		if (numTS == 0 || tempNumTS < numTS)
			numTS = tempNumTS;
//		if (tempFullNumNodes != fullNumNodes)
//		{
//			fullFort63.close();
//			return 3;
//		}
	}

	fullFort63Location = newLoc;
	fullFort63Set = true;

	MapFort63(&fullFileMap, &fullFort63, &fullmapped);
	std::cout << "Map Timestep Result: " << CreateTimestepMap() << std::endl;

	return 0;

}


int WaterLayer_Compare_Full_Sub::SetPy140Location(std::string newLoc)
{
	py140Location = newLoc;
	py140Set = true;
	return 0;
}


//int WaterLayer_Compare_Full_Sub::FullSetFort14Location(std::string newLoc)
//{
//	fullFort14Location = newLoc;
//	fullFort14Set = true;
//	return 0;
//}


int WaterLayer_Compare_Full_Sub::SetOutlineShader(GLShader *newShader)
{
	outlineShader = newShader;
	outlineShaderSet = true;
	return 0;
}


int WaterLayer_Compare_Full_Sub::SetFillShader(GLShader *newShader)
{
	fillShader = newShader;
	fillShaderSet = true;
	return 0;
}


void WaterLayer_Compare_Full_Sub::SetTerrainLayer(TerrainLayer *newLayer)
{
	terrainLayer = newLayer;
	terrainLayerSet = true;
}


int WaterLayer_Compare_Full_Sub::setOutlineShader(GLShader *newShader)
{
	outlineShader = newShader;
	outlineShaderSet = true;
	return 0;
}


int WaterLayer_Compare_Full_Sub::setFillShader(GLShader *newShader)
{
	fillShader = newShader;
	fillShaderSet = true;
	return 0;
}


int WaterLayer_Compare_Full_Sub::GetCurrentTimestep()
{
	return currTS;
}


float WaterLayer_Compare_Full_Sub::GetMinZ()
{
	return minZ;
}


float WaterLayer_Compare_Full_Sub::GetMaxZ()
{
	return maxZ;
}


int WaterLayer_Compare_Full_Sub::MapFort63(std::vector<long> *map, std::ifstream *file, bool *flag)
{
	if (file->is_open())
	{
		long position, size, tsStart, tsTrash, tempNumNodes, nspoolge;
		float dtdp, compVal;
		std::string line;
		std::streamsize maxSize = std::numeric_limits<std::streamsize>::max();

		std::cout << "Mapping " << numTS << " timesteps in fort.63 file" << std::endl;

		file->seekg(0, std::ios_base::beg);
		file->ignore(maxSize, '\n');
		std::getline(*file, line);

		map->clear();
		map->reserve(numTS);
		position = file->tellg();
		map->push_back(position);

		std::stringstream(line) >> tsTrash >> tempNumNodes >> dtdp >> nspoolge;
		compVal = ((float)nspoolge)/dtdp;

		file->ignore(maxSize, '\n');
		tsStart = file->tellg();
		for (int i=0; i<tempNumNodes; i++)
		{
			file->ignore(maxSize, '\n');
		}

		for (int i=0; i<numTS-1; i++)
		{
			position = file->tellg();
			size = position-tsStart;
			std::getline(*file, line);
			tsStart = file->tellg();
			std::stringstream(line) >> dtdp >> nspoolge;
			if (((float)nspoolge)/dtdp != compVal)
			{
				std::cout << "miss" << std::endl;
				file->seekg((*map)[map->size()-1], std::ios_base::beg);
				file->ignore(maxSize, '\n');
				tsStart = file->tellg();
				for (int j=0; j<tempNumNodes; j++)
				{
					file->ignore(maxSize, '\n');
				}
				position = file->tellg();
				map->push_back(position);
				size = position - tsStart;
				file->ignore(maxSize, '\n');
			} else {
				map->push_back(position);
			}
			file->seekg(size, std::ios_base::cur);
		}
	} else {
		*flag = false;
		return 1;
	}

	*flag = true;
	return 0;
}


int WaterLayer_Compare_Full_Sub::SubMapFort63()
{
	return 0;
}


int WaterLayer_Compare_Full_Sub::FullMapFort63()
{
	return 0;
}


int WaterLayer_Compare_Full_Sub::CreateTimestepMap()
{
	// Things we need to create the timestep map:
	// - fort.14 sub/full to determine common nodes
	// - fort.63 full to map to
	// Guaranteed state if all 3 flags are met:
	// - access to terrainLayer nodeList
	// - full fort.14 location is set (will need to open/close stream here)
	// - full fort.63 file is open (leave open)
	if (!terrainLayerSet || !py140Set || !fullFort63Set)
	{
		if (!terrainLayerSet)
			std::cout << "Terrain Layer not set" << std::endl;
		if (!py140Set)
			std::cout << "py.140 not set" << std::endl;
//		if (!fullFort14Set)
//			std::cout << "Full fort.14 not set" << std::endl;
		if (!fullFort63Set)
			std::cout << "Full fort.63 not set" << std::endl;
		return 1;
	}

	std::vector<int> fullNodes;
	std::map<int, int> newToOldMap;
	std::ifstream py140 (py140Location.data());
	if (py140.is_open())
	{
		commonNodes.clear();

		std::string line;
		std::getline(py140, line);

		int subNum, fullNum;

		for (int i=0; i<terrainLayer->numNodes; i++)
		{
			py140 >> subNum;
			py140 >> fullNum;
			fullNodes.push_back(fullNum);
			newToOldMap[fullNum] = subNum;
		}

		py140.close();

		std::sort(fullNodes.begin(), fullNodes.end());

		for (int i=0; i<fullNodes.size(); i++)
		{
			commonNodes.push_back(newToOldMap[fullNodes[i]]);
		}


	} else {
		return 2;
	}

	// Create list of subdomain coordinates
	// - map<x-coord, vector<nodeNumber>>
	// - map<y-coord, vector<nodeNumber>>
//	std::map<float, std::vector<int> > subXCoords;
//	std::map<float, std::vector<int> > subYCoords;
//	std::map<std::string, std::vector<int> > subXCoords;
//	std::map<std::string, std::vector<int> > subYCoords;
//	std::ifstream fort14 (terrainLayer->fort14Location.data());
//	if (fort14.is_open())
//	{
//		std::string line;
//		std::getline(fort14, line);
//		std::getline(fort14, line);
//		std::getline(fort14, line);

////		int nodeNumber;
////		float x, y, z;
//		int nodeNumber;
//		std::string x, y, z;


//		for (int i=0; i<terrainLayer->numNodes; i++)
//		{
////			fort14 >> nodeNumber;
////			fort14 >> x;
////			fort14 >> y;
////			fort14 >> z;

//			std::getline(fort14, line);
//			std::stringstream linestream(line);

//			linestream >> nodeNumber;
//			linestream >> x;
//			linestream >> y;
//			linestream >> z;

//			if (subXCoords.count(x) == 0)
//			{
//				std::vector<int> newVec;
//				newVec.push_back(nodeNumber);
//				subXCoords[x] = newVec;
//			} else {
////				std::cout << "Common x: " << subXCoords[x].size() << std::endl;
//				subXCoords[x].push_back(nodeNumber);
//			}

//			if (subYCoords.count(y) == 0)
//			{
//				std::vector<int> newVec;
//				newVec.push_back(nodeNumber);
//				subYCoords[y] = newVec;
//			} else {
////				std::cout << "Common y: " << subYCoords[y].size() << std::endl;
//				subYCoords[y].push_back(nodeNumber);
//			}
//		}

//		fort14.close();

//	} else {
//		return 5;
//	}

//	// Create list of common node numbers by comparing node locations
//	std::cout << "Finding common nodes between full and sub domains" << std::endl;


//	std::ifstream fullFort14 (fullFort14Location.data());
//	if (fullFort14.is_open())
//	{
//		// Skip the info lines
//		fullFort14.ignore(maxSize, '\n');
//		fullFort14.ignore(maxSize, '\n');

//		// Read every data line until we have the full subdomain
//		int nodeNumber;
////		float x, y, z;
//		std::string x, y, z;
//		std::string line;

//		for (int i=0; i<fullNumNodes; i++)
//		{
////			fullFort14 >> nodeNumber;
////			fullFort14 >> x;
////			fullFort14 >> y;
////			fullFort14 >> z;

//			std::getline(fullFort14, line);
//			std::stringstream linestream(line);
//			linestream >> nodeNumber;
//			linestream >> x;
//			linestream >> y;
//			linestream >> z;


//			// Has the same x coordinate
//			if (subXCoords.count(x) > 0)
//			{
//				// Has the same y coordinate
//				if (subYCoords.count(y) > 0)
//				{
//					// Find the correct node
//					for (int i=0; i<subXCoords[x].size(); i++)
//					{
//						for (int j=0; j<subYCoords[y].size(); j++)
//						{
//							if (subXCoords[x][i] == subYCoords[y][j])
//							{
//								commonNodes.push_back(subXCoords[x][i]);
//							}
//						}
//					}
//				}
//			}


////			for (unsigned int j=0; j<terrainLayer->nodeList.size(); j++)
////			{

////				if (terrainLayer->nodeList[j].x == x &&
////				    terrainLayer->nodeList[j].y == y)
////				{
////					commonNodes.push_back(nodeNumber);
////				}
////			}

//			// We've got all of the common nodes, no need to keep looking
//			if ((int)commonNodes.size() == subNumNodes)
//			{
//				std::sort(commonNodes.begin(), commonNodes.end());
//				break;
//			}
//		}

//		// Close the full fort.14 file
//		fullFort14.close();

//	} else {
//		return 2;
//	}


	// Parse a single timestep in full fort.63, saving jump sizes between
	// the common nodes
	std::cout << "Mapping the full domain timestep" << std::endl;
	std::streamsize maxSize = std::numeric_limits<std::streamsize>::max();
	if (!fullFort63.is_open())
	{
		fullFort63.open(fullFort63Location.data());
	}
	if (fullFort63.is_open())
	{
		// Go to the beginning of the file and skip the info lines
		// and the first timestep info line
		fullFort63.seekg(0, std::ios_base::beg);
		fullFort63.ignore(maxSize, '\n');
		fullFort63.ignore(maxSize, '\n');
		fullFort63.ignore(maxSize, '\n');

		int start, finish, numLinesToSkip;
		int currNode = 1;
		int nodeNum;
		float elevation;
		std::cout << "Number of common nodes: " << fullNodes.size() << "\tExpected: " << terrainLayer->numNodes << std::endl;
		for (unsigned int i=0; i<fullNodes.size(); i++)
		{
			start = fullFort63.tellg();
			numLinesToSkip = fullNodes[i] - currNode; // Might have to do a +1 here if newline isn't already skipped at the end of node list parsing
			for (int j=0; j<numLinesToSkip; j++)
				fullFort63.ignore(maxSize, '\n');
			finish = fullFort63.tellg();
			timestepMap.push_back(finish-start);

			// Read the data like we would when parsing a timestep
			fullFort63 >> nodeNum;
			fullFort63 >> elevation;
			if (nodeNum != fullNodes[i])
			{
				std::cout << "Miss\tExpected: " << fullNodes[i] << "\tActual: " << nodeNum << std::endl;
				return 3;
			}

			// We're on the line after the current common node
			currNode = fullNodes[i];
		}
	} else {
		return 4;
	}

	timestepmapped = true;
	std::cout << "Finished mapping timestep: Size = " << timestepMap.size() << std::endl;
	return 0;

}


int WaterLayer_Compare_Full_Sub::ReadTimestep(int ts)
{
	if (!submapped || !fullmapped || !timestepmapped)
	{
		if (!submapped)
			std::cout << "Sub domain not mapped" << std::endl;
		if (!fullmapped)
			std::cout << "Full domain not mapped" << std::endl;
		if (!timestepmapped)
			std::cout << "Timestep not mapped" << std::endl;
		return 1;
	}

	if (ts < 1 || ts > numTS)
		return 2;

	if (!subFort63.is_open() || !fullFort63.is_open())
		return 3;

	int nodeNum;
	float elevation;
	std::streamsize maxSize = std::numeric_limits<std::streamsize>::max();

	// Read in the sub domain data first
	subFort63.seekg(subFileMap[ts-1], std::ios_base::beg);
	subFort63.ignore(maxSize, '\n');
	for (int i=0; i<subNumNodes; i++)
	{
		subFort63 >> nodeNum;
		subFort63 >> elevation;
		if (flipZValue)
			elevation *= -1.0;
		differencesList[nodeNum] = elevation;
	}


	// Read in the full domain data and subtract the values from the sub domain data
	fullFort63.seekg(fullFileMap[ts-1], std::ios_base::beg);
	fullFort63.ignore(maxSize, '\n');
	minZ = 99999;
	maxZ = -99999;
//	std::cout << "Timestep map size: " << timestepMap.size() << std::endl;
	for (int i=0; i<timestepMap.size(); i++)
	{
		fullFort63.seekg(timestepMap[i], std::ios_base::cur);
		fullFort63 >> nodeNum;
		fullFort63 >> elevation;

//		std::cout << "Node Number is: " << nodeNum << "\tShould be: " << commonNodes[i] << std::endl;

		if (flipZValue)
			elevation *= -1.0;
		if (elevation != -99999 && differencesList[commonNodes[i]] != -99999)
		{
			differencesList[commonNodes[i]] = differencesList[commonNodes[i]]-elevation;
			if (differencesList[commonNodes[i]] < minZ)
				minZ = differencesList[commonNodes[i]];
			else if (differencesList[commonNodes[i]] > maxZ)
				maxZ = differencesList[commonNodes[i]];
		}
		else if (elevation == -99999 && differencesList[commonNodes[i]] == -99999)
			differencesList[commonNodes[i]] = -99999;
		else
			differencesList[commonNodes[i]] = 99999;
	}

	std::cout << "Max Difference: " << maxZ << "\tMin Difference: " << minZ << std::endl;

	currTS = ts;
	timestepLoaded = true;
	return 0;
}


int WaterLayer_Compare_Full_Sub::LoadDataToGPU()
{
	if (!timestepLoaded || !terrainLayerSet)
		return 1;

	if (!glLoaded)
	{
		const size_t VertexBufferSize = 4*sizeof(GLfloat)*subNumNodes;

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
			for (int i=0; i<subNumNodes; i++)
			{
				glNodeData[4*i+0] = (GLfloat)terrainLayer->nodeList[i].x;
				glNodeData[4*i+1] = (GLfloat)terrainLayer->nodeList[i].y;
				glNodeData[4*i+2] = (GLfloat)differencesList[commonNodes[i]];
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
			for (int i=0; i<subNumNodes; i++)
			{
				glNodeData[4*i+0] = (GLfloat)terrainLayer->nodeList[i].x;
				glNodeData[4*i+1] = (GLfloat)terrainLayer->nodeList[i].y;
				glNodeData[4*i+2] = (GLfloat)differencesList[commonNodes[i]];
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






























