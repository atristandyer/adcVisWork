#ifndef WATERLAYER_H
#define WATERLAYER_H

#include "Layer.h"
#include "TerrainLayer.h"
#include "adcData.h"
#include "GLShader.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

class WaterLayer : public Layer
{
	public:

		// Function Definitions
		WaterLayer();
		~WaterLayer();
		void	Draw();
		void	LoadTimestep(int ts);

		// Setter Functions
		int	setFort63Location(std::string newLoc);
		int	setOutlineShader(GLShader *newShader);
		int	setFillShader(GLShader *newShader);
		void	setTerrainLayer(TerrainLayer *newLayer);

		// Getter Functions
		int	GetCurrentTimestep();
		float	GetMinZ();
		float	GetMaxZ();

	private:

		// Data Variables
		TerrainLayer	*terrainLayer;
		std::string	fort63Location;
		int		numNodes;
		int		numTS;
		int		currTS;
		float		minZ;
		float		maxZ;

		std::vector<float>	elevationsList;
		std::vector<int>	fileMap;

		std::ifstream		fort63;

		// Flags
		bool	fort63Set;
		bool	terrainSet;
		bool	mapped;
		bool	flipZValue;
		bool	timestepLoaded;
		bool	glLoaded;
		bool	outlineShaderSet;
		bool	fillShaderSet;

		// OpenGL Variables
		GLShader	*outlineShader;
		GLShader	*fillShader;
		GLuint		VAOId;
		GLuint		VBOId;
		GLuint		IBOId;

		// Private Functions
		int	MapFort63();
		int	FastMapFort63();
		int	ReadTimestep(int ts);
		int	LoadDataToGPU();
};

#endif // WATERLAYER_H
