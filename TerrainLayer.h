#ifndef TERRAINLAYER_H
#define TERRAINLAYER_H

#include "adcData.h"
#include "GLShader.h"
#include "Layer.h"
#include "Quadtree.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

class WaterLayer;

class TerrainLayer : public Layer
{
	public:

		friend class WaterLayer;
		friend class WaterLayer_Compare_Full_Sub;

		// Function Definitions
		TerrainLayer();
		~TerrainLayer();
		void	Draw();
		float	GetMinElevation();
		float	GetMaxElevation();

		// Setter Functions
		int	setFort14Location(std::string newLoc);
		int	setOutlineShader(GLShader *newShader);
		int	setFillShader(GLShader *newShader);
		int	setPointShader(GLShader *newShader);
		int	SetElementShader(GLShader *newShader);

		// Node/Element picking
		Node*		pickNode(float x, float y);
		Node*		pickNode(int nodeNum);
		Element*	pickElement(float x, float y);
		Element*	pickElement(int elementNum);
		void		clearPicking();

		// Data Access Functions
		Node*		getNodePointer(int nodeNumber);

	private:

		// Data Variables
		std::string	fort14Location;
		int		numNodes;
		int		numElements;
		float		minCoords[3];
		float		midCoords[3];
		float		maxCoords[3];
		float		max;

		// Picking variables
		Quadtree	*quadtree;
		int		selectedNode;
		int		selectedElement;

		std::vector<Node>	nodeList;
		std::vector<Element>	elementList;

		// Flags
		bool	fort14Set;
		bool	flipZValue;
		bool	fileLoaded;
		bool	glLoaded;
		bool	outlineShaderSet;
		bool	fillShaderSet;
		bool	pointShaderSet;
		bool	elementShaderSet;

		// OpenGL Variables
		GLShader	*outlineShader;
		GLShader	*fillShader;
		GLShader	*pointShader;
		GLShader	*elementShader;
		GLuint		VAOId;
		GLuint		VBOId;
		GLuint		IBOId;

		// Private Functions
		int	ReadFort14();
		int	LoadDataToGPU();
		void	ConvertUnits();
		void	Normalize();

		// Picking helper functions
		bool	isInside(float x, float y, Element *e);
};

#endif // TERRAINLAYER_H
