#ifndef REFINEDWATERLAYER_H
#define REFINEDWATERLAYER_H

#include "Layer.h"
#include "RefinedTerrainLayer.h"
#include "GLShader.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

#include <time.h>

class RefinedWaterLayer : public Layer
{
	public:

		// Function Definitions
		RefinedWaterLayer();
		~RefinedWaterLayer();
		void	Draw();
		void	LoadTimestep(int ts);

		// Setter Functions
		int	SetOriginalFort63Location(std::string newLoc);
		int	SetRefinedFort63Location(std::string newLoc);
		int	SetOutlineShader(GLShader *newShader);
		int	SetFillShader(GLShader *newShader);
		void	SetTerrainLayer(RefinedTerrainLayer *newLayer);

		// Getter Functions
		int	GetCurrentTimestep();
		float	GetMinOriginal();
		float	GetMaxOriginal();
		float	GetMinRefined();
		float	GetMaxRefined();
		float	GetMinDifference();
		float	GetMaxDifference();

	private:

		// Data Variables
		RefinedTerrainLayer	*terrainLayer;
		std::string		originalFort63Location;
		std::string		refinedFort63Location;
		int			numNodesOriginal;
		int			numNodesRefined;
		int			numTS;
		int			currTS;
		float			minOriginal, maxOriginal;
		float			minRefined, maxRefined;
		float			minDiff, maxDiff;

		std::vector<float>	originalElevations;
		std::vector<float>	refinedElevations;
		std::vector<int>	originalMap;
		std::vector<int>	refinedMap;

		std::ifstream		originalFort;
		std::ifstream		refinedFort;

		// Flags
		bool	originalFort63Set;
		bool	refinedFort63Set;
		bool	terrainSet;
		bool	mapped;
		bool	flipZValue;
		bool	timestepLoaded;
		bool	glLoaded;
		bool	outlineShaderSet;
		bool	fillShaderSet;


		// OpenGL Variables
		GLShader	*OutlineShader;
		GLShader	*FillShader;
		GLuint		VAOId;
		GLuint		VBOId;
		GLuint		IBOId;

		// Private Functions
		int	MapFort63s();
		int	FastMapFort63s();
		int	ReadTimestep(int ts);
		int	LoadDataToGPU();

};

#endif // REFINEDWATERLAYER_H
