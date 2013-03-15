#include "TerrainLayer_Compare_Full_Sub.h"


TerrainLayer_Compare_Full_Sub::TerrainLayer_Compare_Full_Sub()
{
	SubTerrainLayer = new TerrainLayer();
	FullTerrainLayer = new TerrainLayer();
}


TerrainLayer_Compare_Full_Sub::~TerrainLayer_Compare_Full_Sub()
{
	if (SubTerrainLayer)
		delete SubTerrainLayer;
	if (FullTerrainLayer)
		delete FullTerrainLayer;
}


void TerrainLayer_Compare_Full_Sub::Draw()
{
	if (SubTerrainLayer)
		SubTerrainLayer->Draw();
	if (FullTerrainLayer)
		FullTerrainLayer->Draw();
}


int TerrainLayer_Compare_Full_Sub::SubSetFort14Location(std::string newLoc)
{
	return 0;
}


int TerrainLayer_Compare_Full_Sub::FullSetFort14Location(std::string newLoc)
{
	return 0;
}

//TerrainLayer_Compare_Full_Sub::TerrainLayer_Compare_Full_Sub()
//{
//	subFort14Set = false;
//	fullFort14Set = false;
//	subFileLoaded = false;
//	fullFileLoaded = false;
//	glLoaded = false;
//	subOutlineShaderSet = false;
//	fullOutlineShaderSet = false;
//	subFillShaderSet = false;
//	fullFillShaderSet = false;
//	pointShaderSet = false;
//	elementShaderSet = false;

//	subNumNodes = 0;
//	fullNumNodes = 0;
//	subNumElements = 0;
//	fullNumElements = 0;
//	subMinCoords[0] = 0.0;
//	subMinCoords[1] = 0.0;
//	subMinCoords[2] = 0.0;
//	subMaxCoords[0] = 0.0;
//	subMaxCoords[1] = 0.0;
//	subMaxCoords[2] = 0.0;
//	fullMinCoords[0] = 0.0;
//	fullMinCoords[1] = 0.0;
//	fullMinCoords[2] = 0.0;
//	fullMaxCoords[0] = 0.0;
//	fullMaxCoords[1] = 0.0;
//	fullMaxCoords[2] = 0.0;

//	subQuadtree = 0;
//	fullQuadtree = 0;
//	selectedNode = 0;
//	selectedElement = 0;

//	subOutlineShader = 0;
//	subFillShader = 0;
//	fullOutlineShader = 0;
//	fullFillShader = 0;
//	pointShader = 0;
//	elementShader = 0;
//	subVAOId = 0;
//	subVBOId = 0;
//	subIBOId = 0;
//	fullVAOId = 0;
//	fullVBOId = 0;
//	fullIBOId = 0;
//}


//TerrainLayer_Compare_Full_Sub::~TerrainLayer_Compare_Full_Sub()
//{
//	if (glLoaded)
//	{
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//		glDeleteBuffers(1, &subVBOId);
//		glDeleteBuffers(1, &subVAOId);
//		glDeleteBuffers(1, &subIBOId);
//		glDeleteBuffers(1, &fullVBOId);
//		glDeleteBuffers(1, &fullVAOId);
//		glDeleteBuffers(1, &fullIBOId);
//	}

//	if (subQuadtree)
//		delete subQuadtree;
//	if (fullQuadtree)
//		delete fullQuadtree;
//}


//void TerrainLayer_Compare_Full_Sub::Draw()
//{

//}


//float TerrainLayer_Compare_Full_Sub::GetMinElevationSub()
//{
//	return 0.0;
//}


//float TerrainLayer_Compare_Full_Sub::GetMinElevationFull()
//{
//	return 0.0;
//}


//float TerrainLayer_Compare_Full_Sub::GetMaxElevationSub()
//{
//	return 0.0;
//}


//float TerrainLayer_Compare_Full_Sub::GetMaxElevationFull()
//{
//	return 0.0;
//}


//int TerrainLayer_Compare_Full_Sub::SetSubFort14Location(std::string newLoc)
//{
//	return 0;
//}


//int TerrainLayer_Compare_Full_Sub::SetFullFort14Location(std::string newLoc)
//{
//	return 0;
//}


//int TerrainLayer_Compare_Full_Sub::SetSubOutlineShader(GLShader *newShader)
//{
//	return 0;
//}


//int TerrainLayer_Compare_Full_Sub::SetFullOutlineShader(GLShader *newShader)
//{
//	return 0;
//}


//int TerrainLayer_Compare_Full_Sub::SetSubFillShader(GLShader *newShader)
//{
//	return 0;
//}


//int TerrainLayer_Compare_Full_Sub::SetFullFillShader(GLShader *newShader)
//{
//	return 0;
//}


//int TerrainLayer_Compare_Full_Sub::SetPointShader(GLShader *newShader)
//{
//	return 0;
//}


//int TerrainLayer_Compare_Full_Sub::SetElementShader(GLShader *newShader)
//{
//	return 0;
//}


//Node *TerrainLayer_Compare_Full_Sub::PickNode(float x, float y)
//{
//	return 0;
//}


//Node *TerrainLayer_Compare_Full_Sub::PickNode(int nodeNum)
//{
//	return 0;
//}


//Element *TerrainLayer_Compare_Full_Sub::PickElement(float x, float y)
//{
//	return 0;
//}


//Element *TerrainLayer_Compare_Full_Sub::PickElement(int elementNum)
//{
//	return 0;
//}


//void TerrainLayer_Compare_Full_Sub::ClearPicking()
//{

//}


//Node *TerrainLayer_Compare_Full_Sub::GetNodePointer(int nodeNumber)
//{
//	return 0;
//}


//int TerrainLayer_Compare_Full_Sub::SubReadFort14()
//{
//	return 0;
//}


//int TerrainLayer_Compare_Full_Sub::FullReadFort14()
//{
//	return 0;
//}


//int TerrainLayer_Compare_Full_Sub::LoadDataToGPU()
//{
//	return 0;
//}


//void TerrainLayer_Compare_Full_Sub::ConvertUnits()
//{

//}


//void TerrainLayer_Compare_Full_Sub::Normalize()
//{

//}


//bool TerrainLayer_Compare_Full_Sub::isInside(float x, float y, Element *e)
//{
//	return true;
//}


