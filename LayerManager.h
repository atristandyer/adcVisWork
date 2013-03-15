#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <vector>
#include <QString>
#include "Layer.h"
#include "TerrainLayer.h"
#include "WaterLayer.h"
#include "WaterLayer_Compare_Full_Sub.h"
#include "RefinedTerrainLayer.h"
#include "RefinedWaterLayer.h"

#include "GLShader.h"
#include "DefaultShader.h"
#include "GradientShader.h"
#include "GradientCompareShader.h"
#include "DefaultShader.h"
#include "GLCamera2D.h"


class LayerManager
{
	public:
		LayerManager();
		~LayerManager();

		// Setter Functions
		void	setCameraPointer(GLCamera2D *camera);
		void	setActiveLayer(QString layerName);
		void	setTerrainOutlineShader(QString shaderName);
		void	setTerrainFillShader(QString shaderName);
		void	setWaterOutlineShader(QString shaderName);
		void	setWaterFillShader(QString shaderName);
		void	updateWaterGradientShader(QString shaderName);

		// Getter Functions
		TerrainLayer			*getActiveTerrainLayer();
		WaterLayer			*getActiveWaterLayer();
		RefinedTerrainLayer		*getActiveRefinedTerrainLayer();
		RefinedWaterLayer		*getActiveRefinedWaterLayer();
		TerrainLayer			*getActiveCompareTerrainLayer();
		WaterLayer_Compare_Full_Sub	*getActiveCompareWaterLayer();

		DefaultShader		*getDefaultShader(QString shaderName);
		GradientShader		*getGradientShader(QString shaderName);
		GradientCompareShader	*getGradientCompareShader(QString shaderName);
//////////////////////////
//		GLShader		*getShader(QString shaderName);
//////////////////////////

		int			getNumLayers();
		int			getNumRefinedLayers();
		int			getNumCompareLayers();
		int			getNumShaders();
		int			getNumGradientShaders();
		int			getNumGradientCompareShaders();
		QString			getLayerName(int i);
		QString			getRefinedLayerName(int i);
		QString			getCompareLayerName(int i);
		QString			getShaderName(int i);
		QString			getGradientShaderName(int i);
		QString			getGradientCompareShaderName(int i);

		bool			defaultShadersInitialized();

		// Adding Functions
		void	addLayer(QString layerName, TerrainLayer *newTerrain, WaterLayer *newWater);
		void	addLayer(QString layerName, RefinedTerrainLayer *newTerrain, RefinedWaterLayer *newWater);
		void	addCompareLayer(QString layerName, TerrainLayer *newTerrain, WaterLayer_Compare_Full_Sub *newWater);
		void	addShader(QString shaderName, DefaultShader *newShader);
		void	addShader(QString shaderName, GradientShader *newShader);
		void	addShader(QString shaderName, GradientCompareShader *newShader);

	////////
//		void	addShader(QString shaderName, GLShader *newShader);
	////////

		// Initialization Functions
		void	InitializeDefaultShaders();

		// Clearing Functions
		void	clearAllLayers();
		void	clearAllShaders();

	private:

		GLCamera2D	*cameraPtr;

		// Lists of pointers for memory management
		std::vector<TerrainLayer*>			terrainLayers;
		std::vector<WaterLayer*>			waterLayers;
		std::vector<QString>				layerNames;

		std::vector<RefinedTerrainLayer*>		refinedTerrainLayers;
		std::vector<RefinedWaterLayer*>			refinedWaterLayers;
		std::vector<QString>				refinedLayerNames;

		std::vector<TerrainLayer*>			compareTerrainLayers;
		std::vector<WaterLayer_Compare_Full_Sub*>	compareWaterLayers;
		std::vector<QString>				compareLayerNames;

		std::vector<DefaultShader*>			defaultShaders;
		std::vector<QString>				defaultShaderNames;

		std::vector<GradientShader*>			gradientShaders;
		std::vector<QString>				gradientShaderNames;

		std::vector<GradientCompareShader*>		gradientCompareShaders;
		std::vector<QString>				gradientCompareShaderNames;


	////////// Take out these lines
//		std::vector<GLShader*>	shaders;
//		std::vector<QString>	shaderNames;
	//////////

		// Active layers
		TerrainLayer			*activeTerrain;
		WaterLayer			*activeWater;
		RefinedTerrainLayer		*activeRefinedTerrain;
		RefinedWaterLayer		*activeRefinedWater;
		TerrainLayer			*activeCompareTerrain;
		WaterLayer_Compare_Full_Sub	*activeCompareWater;

		// Flags
		bool	defaultShadersSet;
};

#endif // LAYERMANAGER_H
