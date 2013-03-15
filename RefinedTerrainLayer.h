#ifndef REFINEDTERRAINLAYER_H
#define REFINEDTERRAINLAYER_H

#include "adcData.h"
#include "Layer.h"
#include "GLShader.h"
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

class RefinedWaterLayer;

class RefinedTerrainLayer : public Layer
{
	public:

		friend class RefinedWaterLayer;

		bool	draw;

		// Function Definitions
		RefinedTerrainLayer();
		~RefinedTerrainLayer();
		void	Draw();
		float	GetMinElevation();
		float	GetMaxElevation();

		// Setter Function
		int	SetOriginalFort14(std::string newLoc);
		int	SetRefinedFort14(std::string newLoc);
		int	SetOriginalFillShader(GLShader *newShader);
		int	SetOriginalOutlineShader(GLShader *newShader);
		int	SetRefinedFillShader(GLShader *newShader);
		int	SetRefinedOutlineShader(GLShader *newShader);

	private:

		// Data Variables
		std::string	originalFort14Location;
		std::string	refinedFort14Location;
		int		numNodesOriginal;
		int		numNodesRefined;
		int		numElementsOriginal;
		int		numElementsRefined;
		float		minCoords[3];
		float		midCoords[3];
		float		maxCoords[3];

		std::vector<Node>	nodeList;
		std::vector<Element>	elementList;
		std::vector<unsigned int>	refinedToOriginalMap;
		std::vector<unsigned int>	refinedToOriginalNewMap;
		std::map<unsigned int, unsigned int>	refinedToOriginal;
		std::map<unsigned int, unsigned int>	originalToRefined;

		// Flags
		bool	originalFort14Set;
		bool	refinedFort14Set;
		bool	flipZValue;
		bool	fileLoaded;
		bool	glLoaded;
		bool	originalOutlineShaderSet;
		bool	originalFillShaderSet;
		bool	refinedOutlineShaderSet;
		bool	refinedFillShaderSet;

		// OpenGL Variables
		GLShader	*originalOutlineShader;
		GLShader	*originalFillShader;
		GLShader	*refinedOutlineShader;
		GLShader	*refinedFillShader;
		GLuint		VAOId;
		GLuint		VBOId;
		GLuint		IBOId;

		// Private Functions
		int	ReadFort14();
		int	LoadDataToGPU();
		void	ConvertUnits();
		void	Normalize();

		// Helper Functions
		int	IsInOriginalNodeList(Node *currNode);
		int	ElementHasRefinedNode(Element *currElement);
		int	MapToOriginal(unsigned int nodeNum);
		int	MapToRefined(unsigned int nodeNum);

};

#endif // REFINEDTERRAINLAYER_H
