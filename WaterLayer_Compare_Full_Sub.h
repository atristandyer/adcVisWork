#ifndef WATERLAYER_COMPARE_FULL_SUB_H
#define WATERLAYER_COMPARE_FULL_SUB_H

#include "Layer.h"
#include "TerrainLayer.h"
#include "GLShader.h"
#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

class WaterLayer_Compare_Full_Sub : public Layer
{
	public:

		// Function Definitions
		WaterLayer_Compare_Full_Sub();
		~WaterLayer_Compare_Full_Sub();
		void	Draw();
		void	LoadTimestep(int ts);

		// Setter Functions
		int	SubSetFort63Location(std::string newLoc);
		int	FullSetFort63Location(std::string newLoc);
		int	SetPy140Location(std::string newLoc);
//		int	FullSetFort14Location(std::string newLoc);	// Remove this
		int	SetOutlineShader(GLShader *newShader);
		int	SetFillShader(GLShader *newShader);
		void	SetTerrainLayer(TerrainLayer *newLayer);
		int	setOutlineShader(GLShader *newShader);
		int	setFillShader(GLShader *newShader);

		// Getter Functions
		int	GetCurrentTimestep();
		float	GetMinZ();
		float	GetMaxZ();

	private:

		// Data Variables
		TerrainLayer	*terrainLayer;
		std::string	subFort63Location;
		std::string	fullFort63Location;
		std::string	py140Location;
//		std::string	fullFort14Location;	// Remove this
		int		subNumNodes;
		int		fullNumNodes;
		int		numTS;
		int		currTS;
		float		minZ;
		float		maxZ;

		std::map<int, float>	differencesList;
		std::vector<long>	subFileMap;
		std::vector<long>	fullFileMap;
		std::vector<long>	timestepMap;
		std::vector<long>	commonNodes;

		std::ifstream		subFort63;
		std::ifstream		fullFort63;

		// Flags
		bool	terrainLayerSet;
		bool	subFort63Set;
		bool	fullFort63Set;
		bool	py140Set;
//		bool	fullFort14Set;	// Remove this
		bool	submapped;
		bool	fullmapped;
		bool	timestepmapped;
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
		int	MapFort63(std::vector<long> *map, std::ifstream *file, bool *flag);
		int	SubMapFort63();
		int	FullMapFort63();
		int	CreateTimestepMap();
		int	ReadTimestep(int ts);
		int	LoadDataToGPU();

};

#endif // WATERLAYER_COMPARE_FULL_SUB_H
