#ifndef TERRAINLAYER_COMPARE_FULL_SUB_H
#define TERRAINLAYER_COMPARE_FULL_SUB_H

#include "adcData.h"
#include "GLShader.h"
#include "Layer.h"
#include "TerrainLayer.h"
#include "Quadtree.h"
#include <string>


class TerrainLayer_Compare_Full_Sub : public Layer
{
	public:

		friend class WaterLayer_Compare_Full_Sub;

		// Function Definitions
		TerrainLayer_Compare_Full_Sub();
		~TerrainLayer_Compare_Full_Sub();
		void	Draw();

		// Setter Functions
		int	SubSetFort14Location(std::string newLoc);
		int	FullSetFort14Location(std::string newLoc);

	private:

		TerrainLayer	*SubTerrainLayer;
		TerrainLayer	*FullTerrainLayer;
};


//class TerrainLayer_Compare_Full_Sub : public Layer
//{
//	public:

//		friend class WaterLayer_Compare_Full_Sub;

//		// Function Definitions
//		TerrainLayer_Compare_Full_Sub();
//		~TerrainLayer_Compare_Full_Sub();
//		void	Draw();
//		float	GetMinElevationSub(), GetMinElevationFull();
//		float	GetMaxElevationSub(), GetMaxElevationFull();

//		// Setter Functions
//		int	SetSubFort14Location(std::string newLoc), SetFullFort14Location(std::string newLoc);
//		int	SetSubFillShader(GLShader *newShader), SetFullFillShader(GLShader *newShader);
//		int	SetSubOutlineShader(GLShader *newShader), SetFullOutlineShader(GLShader *newShader);
//		int	SetPointShader(GLShader *newShader);
//		int	SetElementShader(GLShader *newShader);

//		// Node/Element picking
//		Node*		PickNode(float x, float y);
//		Node*		PickNode(int nodeNum);
//		Element*	PickElement(float x, float y);
//		Element*	PickElement(int elementNum);
//		void		ClearPicking();

//		// Data Access Functions
//		Node*		GetNodePointer(int nodeNumber);

//	private:

//		// Data Variables
//		std::string	subFort14Location, fullFort14Location;
//		int		subNumNodes, fullNumNodes;
//		int		subNumElements, fullNumElements;
//		float		subMinCoords[3], fullMinCoords[3];
//		float		subMidCoords[3], fullMidCoords[3];
//		float		subMaxCoords[3], fullMaxCoords[3];

//		// Picking variables
//		Quadtree	*subQuadtree, *fullQuadtree;
//		int		selectedNode;
//		int		selectedElement;

//		std::vector<Node>	subNodeList, fullNodeList;
//		std::vector<Element>	subElementList, fullElementList;

//		// Flags
//		bool	subFort14Set, fullFort14Set;
//		bool	flipZValue;
//		bool	subFileLoaded, fullFileLoaded;
//		bool	glLoaded;
//		bool	subOutlineShaderSet, fullOutlineShaderSet;
//		bool	subFillShaderSet, fullFillShaderSet;
//		bool	pointShaderSet;
//		bool	elementShaderSet;

//		// OpenGL Variables
//		GLShader	*subOutlineShader, *fullOutlineShader;
//		GLShader	*subFillShader, *fullFillShader;
//		GLShader	*pointShader;
//		GLShader	*elementShader;
//		GLuint		subVAOId, fullVAOId;
//		GLuint		subVBOId, fullVBOId;
//		GLuint		subIBOId, fullIBOId;

//		// Private Functions
//		int	SubReadFort14(), FullReadFort14();
//		int	LoadDataToGPU();
//		void	ConvertUnits();
//		void	Normalize();

//		// Picking helper functions
//		bool	isInside(float x, float y, Element *e);
//};

#endif // TERRAINLAYER_COMPARE_FULL_SUB_H
