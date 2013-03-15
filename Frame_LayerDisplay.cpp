#include "Frame_LayerDisplay.h"
#include "ui_Frame_LayerDisplay.h"

Frame_LayerDisplay::Frame_LayerDisplay(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::Frame_LayerDisplay)
{
	ui->setupUi(this);

	layerManager = 0;
	UpdateDisplays();
}


Frame_LayerDisplay::~Frame_LayerDisplay()
{
	delete ui;
}


void Frame_LayerDisplay::SetLayerManager(LayerManager *manager)
{
	layerManager = manager;
}


void Frame_LayerDisplay::addLayer(QString name)
{
	meshRecord newRecord;
	newRecord.meshName = name;
	newRecord.listIndex = (int)meshRecords.size();
	newRecord.terrainOutline = 0;
	newRecord.terrainFill = 0;
	newRecord.waterOutline = 0;
	newRecord.waterFill = 0;
	meshRecords.push_back(newRecord);
}


void Frame_LayerDisplay::addShader(QString name, int type)
{
	shaderRecord newRecord;
	newRecord.shaderName = name;
	newRecord.listIndex = (int)shaderRecords.size();
	newRecord.type = type;
	shaderRecords.push_back(newRecord);
}


void Frame_LayerDisplay::UpdateDisplays()
{
	ui->terrainOutline->clear();
	ui->terrainFill->clear();
	ui->waterOutline->clear();
	ui->waterFill->clear();

	if (layerManager && layerManager->getNumLayers() + layerManager->getNumRefinedLayers() + layerManager->getNumCompareLayers() > 0)
	{
		ui->terrainOutline->addItem(tr("Default"));
		ui->terrainFill->addItem(tr("Default"));
		ui->waterOutline->addItem(tr("Default"));
		ui->waterFill->addItem(tr("Default"));
	}

	for (unsigned int i=0; i<shaderRecords.size(); i++)
	{
		if (shaderRecords[i].type == 0)
		{
			ui->terrainOutline->addItem(shaderRecords[i].shaderName);
			ui->terrainFill->addItem(shaderRecords[i].shaderName);
		}
		else if (shaderRecords[i].type == 1)
		{
			ui->waterOutline->addItem(shaderRecords[i].shaderName);
			ui->waterFill->addItem(shaderRecords[i].shaderName);
		} else {
			std::cout << shaderRecords[i].shaderName.toStdString() << " " << shaderRecords[i].type << std::endl;
		}
	}

	ui->meshesList->clear();
	if (layerManager)
	{
		for (unsigned int i=0; i<meshRecords.size(); i++)
		{
			ui->meshesList->addItem(meshRecords[i].meshName);
		}
		ui->meshesList->setCurrentRow((int)meshRecords.size()-1);
	}
}


void Frame_LayerDisplay::on_terrainOutline_currentRowChanged(int currentRow)
{
	if (layerManager && currentRow != -1)
	{
		if (currentRow == 0)
		{
			layerManager->setTerrainOutlineShader(tr("DefaultTerrainOutline"));
		} else {
			layerManager->setTerrainOutlineShader(ui->terrainOutline->currentItem()->text());
		}
		for (unsigned int i=0; i<meshRecords.size(); i++)
		{
			if (ui->meshesList->currentItem()->text() == meshRecords[i].meshName)
			{
				meshRecords[i].terrainOutline = currentRow;
				break;
			}
		}
		emit updateGL();
	}
}


void Frame_LayerDisplay::on_terrainFill_currentRowChanged(int currentRow)
{
	if (layerManager && currentRow != -1)
	{
		if (currentRow == 0)
		{
			layerManager->setTerrainFillShader(tr("DefaultTerrainFill"));
		} else {
			layerManager->setTerrainFillShader(ui->terrainFill->currentItem()->text());
		}
		for (unsigned int i=0; i<meshRecords.size(); i++)
		{
			if (ui->meshesList->currentItem()->text() == meshRecords[i].meshName)
			{
				meshRecords[i].terrainFill = currentRow;
				break;
			}
		}
		emit updateGL();
	}
}

void Frame_LayerDisplay::on_waterOutline_currentRowChanged(int currentRow)
{
	if (layerManager && currentRow != -1)
	{
		if (currentRow == 0)
		{
			layerManager->setWaterOutlineShader(tr("DefaultWaterOutline"));
		} else {
			layerManager->setWaterOutlineShader(ui->waterOutline->currentItem()->text());
		}
		for (unsigned int i=0; i<meshRecords.size(); i++)
		{
			if (ui->meshesList->currentItem()->text() == meshRecords[i].meshName)
			{
				meshRecords[i].waterOutline = currentRow;
				break;
			}
		}
		emit updateGL();
	}
}

void Frame_LayerDisplay::on_waterFill_currentRowChanged(int currentRow)
{
	if (layerManager && currentRow != -1)
	{
		if (currentRow == 0)
		{
			layerManager->setWaterFillShader(tr("DefaultWaterFill"));
		} else {
			layerManager->setWaterFillShader(ui->waterFill->currentItem()->text());
		}
		for (unsigned int i=0; i<meshRecords.size(); i++)
		{
			if (ui->meshesList->currentItem()->text() == meshRecords[i].meshName)
			{
				meshRecords[i].waterFill = currentRow;
				break;
			}
		}
		emit updateGL();
	}
}

void Frame_LayerDisplay::on_meshesList_currentRowChanged(int currentRow)
{
	if (layerManager && currentRow != -1)
	{
		for (unsigned int i=0; i<meshRecords.size(); i++)
		{
			if (meshRecords[i].meshName == ui->meshesList->currentItem()->text())
			{
				layerManager->setActiveLayer(meshRecords[i].meshName);
				ui->terrainOutline->setCurrentRow(meshRecords[i].terrainOutline);
				ui->terrainFill->setCurrentRow(meshRecords[i].terrainFill);
				ui->waterOutline->setCurrentRow(meshRecords[i].waterOutline);
				ui->waterFill->setCurrentRow(meshRecords[i].waterFill);
				emit updateGL();
				return;
			}
		}
	}
}


void Frame_LayerDisplay::updateWaterGradients(int)
{
	for (int i=0; i<ui->waterFill->count(); i++)
	{
		layerManager->updateWaterGradientShader(ui->waterFill->item(i)->text());
	}
	emit updateGL();
}
