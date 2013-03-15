#include "LayerManager.h"

LayerManager::LayerManager()
{
	cameraPtr = 0;
	activeTerrain = 0;
	activeWater = 0;
	activeRefinedTerrain = 0;
	activeRefinedWater = 0;
	activeCompareTerrain = 0;
	activeCompareWater = 0;

	defaultShadersSet = false;
}


LayerManager::~LayerManager()
{
	clearAllShaders();
	clearAllLayers();
}


void LayerManager::setCameraPointer(GLCamera2D *camera)
{
	cameraPtr = camera;

	for (unsigned int i=0; i<defaultShaders.size(); i++)
	{
		defaultShaders[i]->SetCamera(camera);
	}

	for (unsigned int i=0; i<gradientShaders.size(); i++)
	{
		gradientShaders[i]->SetCamera(camera);
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	for (unsigned int i=0; i<shaders.size(); i++)
//	{
//		shaders[i]->SetCamera(camera);
//	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


void LayerManager::setActiveLayer(QString layerName)
{
	for (unsigned int i=0; i<layerNames.size(); i++)
	{
		if (layerName == layerNames[i])
		{
			activeTerrain = terrainLayers[i];
			activeWater = waterLayers[i];
			activeRefinedTerrain = 0;
			activeRefinedWater = 0;
			activeCompareTerrain = 0;
			activeCompareWater = 0;
			return;
		}
	}
	for (unsigned int i=0; i<refinedLayerNames.size(); i++)
	{
		if (layerName == refinedLayerNames[i])
		{
			activeRefinedTerrain = refinedTerrainLayers[i];
			activeRefinedWater = refinedWaterLayers[i];
			activeTerrain = 0;
			activeWater = 0;
			activeCompareTerrain = 0;
			activeCompareWater = 0;
			return;
		}
	}
	for (unsigned int i=0; i<compareLayerNames.size(); i++)
	{
		if (layerName == compareLayerNames[i])
		{
			activeCompareTerrain = compareTerrainLayers[i];
			activeCompareWater = compareWaterLayers[i];
			activeTerrain = 0;
			activeWater = 0;
			activeRefinedTerrain = 0;
			activeRefinedWater = 0;
			std::cout << "Active Compare Water Layer Set" << std::endl;
		}
	}

	activeTerrain = 0;
	activeWater = 0;
	activeRefinedTerrain = 0;
	activeRefinedWater = 0;
}


void LayerManager::setTerrainOutlineShader(QString shaderName)
{
	if (activeTerrain)
	{
		if (getDefaultShader(shaderName))
		{
			activeTerrain->setOutlineShader(getDefaultShader(shaderName));
		} else if (getGradientShader(shaderName))
		{
			GradientShader *shader = getGradientShader(shaderName);
			if (!shader->UseCustomHeightRange)
			{
				shader->SetHeightRange(activeTerrain->GetMinElevation(), activeTerrain->GetMaxElevation());
			}
			activeTerrain->setOutlineShader(shader);
		}
	}
	else if (activeCompareTerrain)
	{
		if (getDefaultShader(shaderName))
		{
			activeCompareTerrain->setOutlineShader(getDefaultShader(shaderName));
		} else if (getGradientShader(shaderName))
		{
			GradientShader *shader = getGradientShader(shaderName);
			if (!shader->UseCustomHeightRange)
			{
				shader->SetHeightRange(activeCompareTerrain->GetMinElevation(), activeCompareTerrain->GetMaxElevation());
			}
			activeCompareTerrain->setOutlineShader(shader);
		}
	}
}


void LayerManager::setTerrainFillShader(QString shaderName)
{
	if (activeTerrain)
	{
		if (getDefaultShader(shaderName))
		{
			activeTerrain->setFillShader(getDefaultShader(shaderName));
		} else if (getGradientShader(shaderName))
		{
			GradientShader *shader = getGradientShader(shaderName);
			if (!shader->UseCustomHeightRange)
			{
				shader->SetHeightRange(activeTerrain->GetMinElevation(), activeTerrain->GetMaxElevation());
			}
			activeTerrain->setFillShader(shader);
		}
	}
	else if (activeCompareTerrain)
	{
		if (getDefaultShader(shaderName))
		{
			activeCompareTerrain->setFillShader(getDefaultShader(shaderName));
		} else if (getGradientShader(shaderName))
		{
			GradientShader *shader = getGradientShader(shaderName);
			if (!shader->UseCustomHeightRange)
			{
				shader->SetHeightRange(activeCompareTerrain->GetMinElevation(), activeCompareTerrain->GetMaxElevation());
			}
			activeCompareTerrain->setFillShader(shader);
		}
	}
}


void LayerManager::setWaterOutlineShader(QString shaderName)
{
	if (activeWater)
	{
		if (getDefaultShader(shaderName))
		{
			activeWater->setOutlineShader(getDefaultShader(shaderName));
		} else if (getGradientShader(shaderName))
		{
			GradientShader *shader = getGradientShader(shaderName);
			if (!shader->UseCustomHeightRange)
			{
				shader->SetHeightRange(activeWater->GetMinZ(), activeWater->GetMaxZ());
			}
			activeWater->setOutlineShader(shader);
		} else if (getGradientCompareShader(shaderName))
		{
			GradientCompareShader *shader = getGradientCompareShader(shaderName);
			if (!shader->UseCustomHeightRange)
			{
				shader->SetHeightRange(activeWater->GetMinZ(), activeWater->GetMaxZ());
			}
			activeWater->setOutlineShader(shader);
		}
	}
	else if (activeCompareWater)
	{
		if (getDefaultShader(shaderName))
		{
			activeCompareWater->setOutlineShader(getDefaultShader(shaderName));
		} else if (getGradientShader(shaderName))
		{
			GradientShader *shader = getGradientShader(shaderName);
			if (!shader->UseCustomHeightRange)
			{
				shader->SetHeightRange(activeCompareWater->GetMinZ(), activeCompareWater->GetMaxZ());
			}
			activeCompareWater->setOutlineShader(shader);
		} else if (getGradientCompareShader(shaderName))
		{
			GradientCompareShader *shader = getGradientCompareShader(shaderName);
			if (!shader->UseCustomHeightRange)
			{
				shader->SetHeightRange(activeCompareWater->GetMinZ(), activeCompareWater->GetMaxZ());
			}
			activeCompareWater->setOutlineShader(shader);
		}
	}
}


void LayerManager::setWaterFillShader(QString shaderName)
{
	if (activeWater)
	{
		if (getDefaultShader(shaderName))
		{
			activeWater->setFillShader(getDefaultShader(shaderName));
		} else if (getGradientShader(shaderName))
		{
			GradientShader *shader = getGradientShader(shaderName);
			if (!shader->UseCustomHeightRange)
			{
				shader->SetHeightRange(activeWater->GetMinZ(), activeWater->GetMaxZ());
			}
			activeWater->setFillShader(shader);
		} else if (getGradientCompareShader(shaderName))
		{
			GradientCompareShader *shader = getGradientCompareShader(shaderName);
			if (!shader->UseCustomHeightRange)
			{
				shader->SetHeightRange(activeWater->GetMinZ(), activeWater->GetMaxZ());
			}
			activeWater->setFillShader(shader);
		}
	}
	else if (activeCompareWater)
	{
		if (getDefaultShader(shaderName))
		{
			std::cout << "Found Default Shader with name: " << shaderName.toStdString() << std::endl;
			activeCompareWater->setFillShader(getDefaultShader(shaderName));
		} else if (getGradientShader(shaderName))
		{
			std::cout << "Found Gradient Shader with name: " << shaderName.toStdString() << std::endl;
			GradientShader *shader = getGradientShader(shaderName);
			if (!shader->UseCustomHeightRange)
			{
				shader->SetHeightRange(activeCompareWater->GetMinZ(), activeCompareWater->GetMaxZ());
			}
			activeCompareWater->setFillShader(shader);
		} else if (getGradientCompareShader(shaderName))
		{
			std::cout << "Found Gradient Compare Shader with name: " << shaderName.toStdString() << std::endl;
			GradientCompareShader *shader = getGradientCompareShader(shaderName);
			if (!shader->UseCustomHeightRange)
			{
				shader->SetHeightRange(activeCompareWater->GetMinZ(), activeCompareWater->GetMaxZ());
			}
			activeCompareWater->setFillShader(shader);
		}
	}
}


void LayerManager::updateWaterGradientShader(QString shaderName)
{
	GradientShader *shader = getGradientShader(shaderName);
	if (activeWater && shader)
	{
		if (!shader->UseCustomHeightRange)
			shader->SetHeightRange(activeWater->GetMinZ(), activeWater->GetMaxZ());
		return;
	}
	else if (activeCompareWater && shader)
	{
		std::cout << "Updated Gradient Shader: " << shaderName.toStdString() << std::endl;
		if (!shader->UseCustomHeightRange)
			shader->SetHeightRange(activeCompareWater->GetMinZ(), activeCompareWater->GetMaxZ());
		return;
	}

	GradientCompareShader *shaderComp = getGradientCompareShader(shaderName);
	if (activeCompareWater && shaderComp)
	{
		std::cout << "Updated Gradient Compare Shader: " << shaderName.toStdString() << std::endl;
		if (!shaderComp->UseCustomHeightRange)
			shaderComp->SetHeightRange(activeCompareWater->GetMinZ(), activeCompareWater->GetMaxZ());
		return;
	}
}


TerrainLayer *LayerManager::getActiveTerrainLayer()
{
	return activeTerrain;
}


WaterLayer *LayerManager::getActiveWaterLayer()
{
	return activeWater;
}


RefinedTerrainLayer *LayerManager::getActiveRefinedTerrainLayer()
{
	return activeRefinedTerrain;
}


RefinedWaterLayer *LayerManager::getActiveRefinedWaterLayer()
{
	return activeRefinedWater;
}


TerrainLayer *LayerManager::getActiveCompareTerrainLayer()
{
	return activeCompareTerrain;
}


WaterLayer_Compare_Full_Sub *LayerManager::getActiveCompareWaterLayer()
{
	return activeCompareWater;
}


DefaultShader *LayerManager::getDefaultShader(QString shaderName)
{
	for (unsigned int i=0; i<defaultShaderNames.size(); i++)
	{
		if (shaderName == defaultShaderNames[i])
		{
			return defaultShaders[i];
		}
	}
	return 0;
}


GradientShader *LayerManager::getGradientShader(QString shaderName)
{
	for (unsigned int i=0; i<gradientShaderNames.size(); i++)
	{
		if (shaderName == gradientShaderNames[i])
		{
			return gradientShaders[i];
		}
	}
	return 0;
}


GradientCompareShader *LayerManager::getGradientCompareShader(QString shaderName)
{
	for (unsigned int i=0; i<gradientCompareShaderNames.size(); i++)
	{
		std::cout << "Loop: " << i << std::endl;
		if (shaderName == gradientCompareShaderNames[i])
		{
			return gradientCompareShaders[i];
		}
	}
	return 0;
}


//GLShader *LayerManager::getShader(QString shaderName)
//{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////	for (unsigned int i=0; i<shaderNames.size(); i++)
////	{
////		if (shaderName == shaderNames[i])
////		{
////			return shaders[i];
////		}
////	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	std::cout << "Shader not found" << std::endl;
//	return 0;
//}


int LayerManager::getNumLayers()
{
	return (int)layerNames.size();
}


int LayerManager::getNumRefinedLayers()
{
	return (int)refinedLayerNames.size();
}


int LayerManager::getNumCompareLayers()
{
	return (int)compareLayerNames.size();
}


int LayerManager::getNumShaders()
{
	return (int)defaultShaderNames.size();
}


int LayerManager::getNumGradientShaders()
{
	return (int)gradientShaderNames.size();
}


int LayerManager::getNumGradientCompareShaders()
{
	return (int)gradientCompareShaderNames.size();
}


QString LayerManager::getLayerName(int i)
{
	if (i < (int)layerNames.size())
		return layerNames[i];
	return QString("");
}


QString LayerManager::getRefinedLayerName(int i)
{
	if (i < (int)refinedLayerNames.size())
		return refinedLayerNames[i];
	return QString("");
}


QString LayerManager::getCompareLayerName(int i)
{
	if (i < (int)compareLayerNames.size())
		return compareLayerNames[i];
	return QString("");
}


QString LayerManager::getShaderName(int i)
{
	if (i < (int)defaultShaderNames.size())
		return defaultShaderNames[i];
	return QString("");
}


QString LayerManager::getGradientShaderName(int i)
{
	if (i < (int)gradientShaderNames.size())
		return gradientShaderNames[i];
	return QString("");
}


QString LayerManager::getGradientCompareShaderName(int i)
{
	if (i < (int)gradientCompareShaderNames.size())
		return gradientCompareShaderNames[i];
	return QString("");
}


bool LayerManager::defaultShadersInitialized()
{
	return defaultShadersSet;
}


void LayerManager::addLayer(QString layerName, TerrainLayer *newTerrain, WaterLayer *newWater)
{
	terrainLayers.push_back(newTerrain);
	waterLayers.push_back(newWater);
	layerNames.push_back(layerName);

	// Make this layer active
	activeTerrain = newTerrain;
	activeWater = newWater;

	// Set shaders to default
	if (!defaultShadersSet)
		InitializeDefaultShaders();
	newTerrain->setPointShader(getDefaultShader(QString("PickerShader")));
	newTerrain->SetElementShader(getDefaultShader(QString("PickerShader")));
}


void LayerManager::addLayer(QString layerName, RefinedTerrainLayer *newTerrain, RefinedWaterLayer *newWater)
{
	refinedTerrainLayers.push_back(newTerrain);
	refinedWaterLayers.push_back(newWater);
	refinedLayerNames.push_back(layerName);

	// Make this layer active
	activeRefinedTerrain = newTerrain;
	activeRefinedWater = newWater;
}


void LayerManager::addCompareLayer(QString layerName, TerrainLayer *newTerrain, WaterLayer_Compare_Full_Sub *newWater)
{
	compareTerrainLayers.push_back(newTerrain);
	compareWaterLayers.push_back(newWater);
	compareLayerNames.push_back(layerName);

	// Make this layer active
	activeCompareTerrain = newTerrain;
	activeCompareWater = newWater;

	// Set shaders to default
	if (!defaultShadersSet)
		InitializeDefaultShaders();
	newTerrain->setPointShader(getDefaultShader(QString("PickerShader")));
	newTerrain->SetElementShader(getDefaultShader(QString("PickerShader")));

}


void LayerManager::addShader(QString shaderName, DefaultShader *newShader)
{
	defaultShaderNames.push_back(shaderName);
	defaultShaders.push_back(newShader);
	if (cameraPtr)
		newShader->SetCamera(cameraPtr);
}


void LayerManager::addShader(QString shaderName, GradientShader *newShader)
{
	std::cout << "Adding Gradient Shader" << std::endl;
	gradientShaderNames.push_back(shaderName);
	gradientShaders.push_back(newShader);
	if (cameraPtr)
		newShader->SetCamera(cameraPtr);
}


void LayerManager::addShader(QString shaderName, GradientCompareShader *newShader)
{
	std::cout << "Adding Gradient Compare Shader" << std::endl;
	gradientCompareShaderNames.push_back(shaderName);
	gradientCompareShaders.push_back(newShader);
	if (cameraPtr)
		newShader->SetCamera(cameraPtr);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void LayerManager::addShader(QString shaderName, GLShader *newShader)
//{
//	shaderNames.push_back(shaderName);
//	shaders.push_back(newShader);
//	if (cameraPtr)
//		newShader->SetCamera(cameraPtr);
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


void LayerManager::InitializeDefaultShaders()
{
	DefaultShader *defaultTerrainOutline = new DefaultShader();
	DefaultShader *defaultTerrainFill = new DefaultShader();
	DefaultShader *defaultWaterOutline = new DefaultShader();
	DefaultShader *defaultWaterFill = new DefaultShader();
	DefaultShader *pickerShader = new DefaultShader();
	defaultTerrainOutline->SetColor(0.6, 0.6, 0.6, 0.5);
	defaultTerrainFill->SetColor(0.0, 0.79, 0.05, 1.0);
	defaultWaterOutline->SetColor(0.0, 0.0, 1.0, 0.2);
	defaultWaterFill->SetColor(0.0, 0.0, 1.0, 0.5);
	pickerShader->SetColor(0.1, 0.1, 0.1, 0.7);
	defaultShaders.push_back(defaultTerrainOutline);
	defaultShaders.push_back(defaultTerrainFill);
	defaultShaders.push_back(defaultWaterOutline);
	defaultShaders.push_back(defaultWaterFill);
	defaultShaders.push_back(pickerShader);
	defaultShaderNames.push_back(QString("DefaultTerrainOutline"));
	defaultShaderNames.push_back(QString("DefaultTerrainFill"));
	defaultShaderNames.push_back(QString("DefaultWaterOutline"));
	defaultShaderNames.push_back(QString("DefaultWaterFill"));
	defaultShaderNames.push_back(QString("PickerShader"));

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	shaders.push_back(defaultTerrainOutline);
//	shaders.push_back(defaultTerrainFill);
//	shaders.push_back(defaultWaterOutline);
//	shaders.push_back(defaultWaterFill);
//	shaders.push_back(pickerShader);
//	shaderNames.push_back(QString("DefaultTerrainOutline"));
//	shaderNames.push_back(QString("DefaultTerrainFill"));
//	shaderNames.push_back(QString("DefaultWaterOutline"));
//	shaderNames.push_back(QString("DefaultWaterFill"));
//	shaderNames.push_back(QString("PickerShader"));
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (cameraPtr)
	{
		defaultTerrainOutline->SetCamera(cameraPtr);
		defaultTerrainFill->SetCamera(cameraPtr);
		defaultWaterOutline->SetCamera(cameraPtr);
		defaultWaterFill->SetCamera(cameraPtr);
		pickerShader->SetCamera(cameraPtr);
	}

	defaultShadersSet = true;
}


void LayerManager::clearAllLayers()
{
	for (unsigned int i=0; i<terrainLayers.size(); i++)
		if (terrainLayers[i])
			delete terrainLayers[i];
	for (unsigned int i=0; i<waterLayers.size(); i++)
		if (waterLayers[i])
			delete waterLayers[i];
	for (unsigned int i=0; i<refinedTerrainLayers.size(); i++)
		if (refinedTerrainLayers[i])
			delete refinedTerrainLayers[i];
	for (unsigned int i=0; i<refinedWaterLayers.size(); i++)
		if (refinedWaterLayers[i])
			delete refinedWaterLayers[i];
	for (unsigned int i=0; i<compareTerrainLayers.size(); i++)
		if (compareTerrainLayers[i])
			delete compareTerrainLayers[i];
	for (unsigned int i=0; i<compareWaterLayers.size(); i++)
		if (compareWaterLayers[i])
			delete compareWaterLayers[i];
}


void LayerManager::clearAllShaders()
{
	for (unsigned int i=0; i<defaultShaders.size(); i++)
	{
		delete defaultShaders[i];
		std::cout << "Shader deleted" << std::endl;
	}
	for (unsigned int i=0; i<gradientShaders.size(); i++)
	{
		delete gradientShaders[i];
		std::cout << "Shader deleted" << std::endl;
	}
	for (unsigned int i=0; i<gradientCompareShaders.size(); i++)
	{
		delete gradientCompareShaders[i];
		std::cout << "Shader deleted" << std::endl;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	for (unsigned int i=0; i<shaders.size(); i++)
//	{
//		delete shaders[i];
//		std::cout << "Shader deleted" << std::endl;
//	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
