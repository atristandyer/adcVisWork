#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{

	ui->setupUi(this);
	ui->centralWidget->layout()->setContentsMargins(0,0,0,0);

	ui->splitter->setStretchFactor(0, 8);
	ui->splitter->setStretchFactor(1, 1);

	// Initialize the layer manager and tell the GLPanel where it is
	layerManager = new LayerManager();
	ui->GLPanel->SetLayerManager(layerManager);

	// Create the data display and show it
	dataDisplay = new Frame_DataDisplay();
	dataDisplay->SetLayerManager(layerManager);
	dataDisplay->SetGLPanel(ui->GLPanel);
	dataDisplayIndex = ui->stackedWidget->addWidget(dataDisplay);
//	ui->stackedWidget->setCurrentIndex(dataDisplayIndex);

	// Create the layer display frame
	layerDisplay = new Frame_LayerDisplay();
	layerDisplay->SetLayerManager(layerManager);
	layerDisplayIndex = ui->stackedWidget->addWidget(layerDisplay);
	ui->stackedWidget->setCurrentIndex(layerDisplayIndex);

	// Create connections to update user interface as actions occur
	connect(ui->GLPanel, SIGNAL(currentTimestep(int)), dataDisplay, SLOT(setCurrentTimestep(int)));
	connect(ui->GLPanel, SIGNAL(currentTimestep(int)), layerDisplay, SLOT(updateWaterGradients(int)));
	connect(ui->GLPanel, SIGNAL(enteringPickingMode()), SLOT(enterPickingMode()));
	connect(ui->GLPanel, SIGNAL(selectedNodeUpdated(Node*)), dataDisplay, SLOT(updateSelectedNode(Node*)));
	connect(ui->GLPanel, SIGNAL(selectedElementUpdated(Element*)), dataDisplay, SLOT(updateSelectedElement(Element*)));
	connect(layerDisplay, SIGNAL(updateGL()), ui->GLPanel, SLOT(updateGL()));


}

MainWindow::~MainWindow()
{
	delete ui;
	delete layerManager;
	delete dataDisplay;
	delete layerDisplay;
}


void MainWindow::on_actionNormal_Mesh_triggered()
{
	if (layerManager && layerDisplay)
	{
		Dialog_NormalMesh dlg;
		QString meshName ("Mesh ");
		meshName.append(QString::number(layerManager->getNumLayers()+1));
		dlg.setDefaultName(meshName);
		if (dlg.exec() == QDialog::Accepted && !dlg.getFort14().isEmpty())
		{
			if (!dlg.getName().isEmpty())
				meshName = dlg.getName();
			TerrainLayer *terrain = new TerrainLayer();
			if (terrain->setFort14Location(dlg.getFort14().toStdString()) == 0)
			{
				WaterLayer *water = new WaterLayer();
				water->setTerrainLayer(terrain);
				if (water->setFort63Location(dlg.getFort63().toStdString()) == 0)
				{
					water->LoadTimestep(1);
					layerManager->addLayer(meshName, terrain, water);
					layerDisplay->addLayer(meshName);
				} else {
					delete water;
					layerManager->addLayer(meshName, terrain, 0);
					layerDisplay->addLayer(meshName);
					std::cout << "Did not open water layer" << std::endl;
					// TODO: Good place for a pop-up notice
				}
			} else {
				delete terrain;
				// TODO: Good place for a pop-up notice
			}
		}
	}

	layerDisplay->UpdateDisplays();
}


void MainWindow::on_actionSub_Full_Domain_triggered()
{
	if (layerManager && layerDisplay)
	{
		Dialog_Compare_Sub_Full dlg;
		QString meshName ("Mesh ");
		meshName.append(QString::number(layerManager->getNumLayers()+1));
		dlg.setDefaultName(meshName);
		if (dlg.exec() == QDialog::Accepted	&& !dlg.getPy140().isEmpty()//!dlg.getFullFort14().isEmpty()
							&& !dlg.getSubFort14().isEmpty()
							&& !dlg.getFullFort63().isEmpty()
							&& !dlg.getSubFort63().isEmpty())
		{
			if (!dlg.getName().isEmpty())
				meshName = dlg.getName();

			TerrainLayer *terrain = new TerrainLayer();
			if (terrain->setFort14Location(dlg.getSubFort14().toStdString()) == 0)
			{
				WaterLayer_Compare_Full_Sub *water = new WaterLayer_Compare_Full_Sub();
				water->SetTerrainLayer(terrain);
				if (water->SubSetFort63Location(dlg.getSubFort63().toStdString()) == 0)
				{
					if (water->SetPy140Location(dlg.getPy140().toStdString()) == 0)
					{
						std::cout << "Got here" << std::endl;
						if (water->FullSetFort63Location(dlg.getFullFort63().toStdString()) == 0)
						{
							water->LoadTimestep(1);
//							delete water;
//							delete terrain;
							layerManager->addCompareLayer("Compare", terrain, water);
							layerDisplay->addLayer("Compare");
						} else {
							std::cout << "Error setting full fort.63" << std::endl;
							delete water;
							delete terrain;
						}
					} else {
						std::cout << "Error setting full fort.14" << std::endl;
						delete water;
						delete terrain;
					}
				} else {
					std::cout << "Error setting sub fort.63" << std::endl;
					delete water;
					delete terrain;
				}
			} else {
				std::cout << "Error creating terrain" << std::endl;
				delete terrain;
			}
		}
	}
//			RefinedTerrainLayer *terrain = new RefinedTerrainLayer();
//			if (terrain->SetOriginalFort14(dlg.getFullFort14().toStdString()) == 0
//					&& terrain->SetRefinedFort14(dlg.getSubFort14().toStdString()) == 0)
//			{
//				tOutline = new RefinedShader();
//				tOutline->SetBottomColor(0, 0, 1.0, 1.0);
//				tOutline->SetTopColor(1.0, 0, 0, 1);
//				tOutline->SetNoErrorColor(1, 1, 1, 1);
//				tOutline->SetErrorRange(-0.05, 0.05);
//				tOutline->SetCamera(ui->GLPanel->GetCameraPointer());

//				solid = new DefaultShader();
//				solid->SetColor(0.0, 0.79, 0.05, 1.0);
//				solid->SetCamera(ui->GLPanel->GetCameraPointer());

//				solid2 = new DefaultShader();
//				solid2->SetColor(0.6, 0.6, 0.6, 0.5);
//				solid2->SetCamera(ui->GLPanel->GetCameraPointer());

//				terrain->SetOriginalFillShader(solid);
//				terrain->SetOriginalOutlineShader(solid2);
//				terrain->SetRefinedFillShader(solid);
//				terrain->SetRefinedOutlineShader(solid2);
//				RefinedWaterLayer *water = new RefinedWaterLayer();
//				water->SetTerrainLayer(terrain);
//				if (water->SetOriginalFort63Location(dlg.getFullFort63().toStdString()) == 0
//						&& water->SetRefinedFort63Location(dlg.getSubFort63().toStdString()) == 0)
//				{
//					water->SetFillShader(tOutline);
//					water->SetOutlineShader(tOutline);
//					water->LoadTimestep(1);
//					layerManager->addLayer(meshName, terrain, water);
//					layerDisplay->addLayer(meshName);
//				} else {
//					delete water;
//					layerManager->addLayer(meshName, terrain, 0);
//					layerDisplay->addLayer(meshName);
//				}
//			} else {
//				delete terrain;
//			}
//		}

//	}
	layerDisplay->UpdateDisplays();
}


void MainWindow::on_actionEditTerrainShader_triggered()
{
	if (layerManager)
	{
		Dialog_ShaderEditor dlg;
		QString defaultName ("Shader ");
		if (layerManager->defaultShadersInitialized())
			defaultName.append(QString::number(layerManager->getNumShaders()+layerManager->getNumGradientShaders()-4));
		else
			defaultName.append(QString::number(layerManager->getNumShaders()+layerManager->getNumGradientShaders()+1));
		dlg.setDefaultShaderName(defaultName);
		dlg.initialTab(0);
		if (layerDisplay && dlg.exec() == QDialog::Accepted)
		{
			// Getting a default shader
			if (dlg.getShaderType() == 0)
			{
				DefaultShader *newShader = dlg.getNewDefaultShader();
				if (newShader)
				{
					if (!dlg.getShaderName().isEmpty())
						defaultName = dlg.getShaderName();
					layerManager->addShader(defaultName, newShader);
					layerDisplay->addShader(defaultName, dlg.getTerrainOrWater());
					layerDisplay->UpdateDisplays();
				}
			}
			// Getting a gradient shader
			else if (dlg.getShaderType() == 1)
			{
				GradientShader *newShader = dlg.getNewGradientShader();
				if (newShader)
				{
					if (!dlg.getShaderName().isEmpty())
						defaultName = dlg.getShaderName();
					layerManager->addShader(defaultName, newShader);
					layerDisplay->addShader(defaultName, dlg.getTerrainOrWater());
					layerDisplay->UpdateDisplays();
				}
			}
//			GLShader *newShader = dlg.getNewShader();
//			if (newShader)
//			{
//				if (!dlg.getShaderName().isEmpty())
//					defaultName = dlg.getShaderName();
//				layerManager->addShader(defaultName, newShader);
//				layerDisplay->addShader(defaultName, dlg.getTerrainOrWater());
//				layerDisplay->UpdateDisplays();
//			}
		}
	}
}


void MainWindow::on_actionEditWaterShader_triggered()
{
	if (layerManager)
	{
		Dialog_ShaderEditor dlg;
		QString defaultName ("Shader ");
		if (layerManager->defaultShadersInitialized())
			defaultName.append(QString::number(layerManager->getNumShaders()+layerManager->getNumGradientShaders()-4));
		else
			defaultName.append(QString::number(layerManager->getNumShaders()+layerManager->getNumGradientShaders()+1));
		dlg.setDefaultShaderName(defaultName);
		dlg.initialTab(1);
		if (layerDisplay && dlg.exec() == QDialog::Accepted)
		{
			// Getting a default shader
			if (dlg.getShaderType() == 0)
			{
				DefaultShader *newShader = dlg.getNewDefaultShader();
				if (newShader)
				{
					if (!dlg.getShaderName().isEmpty())
						defaultName = dlg.getShaderName();
					layerManager->addShader(defaultName, newShader);
					layerDisplay->addShader(defaultName, dlg.getTerrainOrWater());
					layerDisplay->UpdateDisplays();
				}
			}
			// Getting a gradient shader
			else if (dlg.getShaderType() == 1)
			{
				GradientShader *newShader = dlg.getNewGradientShader();
				if (newShader)
				{
					if (!dlg.getShaderName().isEmpty())
						defaultName = dlg.getShaderName();
					layerManager->addShader(defaultName, newShader);
					layerDisplay->addShader(defaultName, dlg.getTerrainOrWater());
					layerDisplay->UpdateDisplays();
				}
			}
			// Getting a gradient shader with w/d
			else if (dlg.getShaderType() == 2)
			{
				GradientCompareShader *newShader = dlg.getNewGradientCompareShader();
				if (newShader)
				{
					if (!dlg.getShaderName().isEmpty())
						defaultName = dlg.getShaderName();
					layerManager->addShader(defaultName, newShader);
					layerDisplay->addShader(defaultName, dlg.getTerrainOrWater());
					layerDisplay->UpdateDisplays();
				}
			}
//			GLShader *newShader = dlg.getNewShader();
//			if (newShader)
//			{
//				if (!dlg.getShaderName().isEmpty())
//					defaultName = dlg.getShaderName();
//				layerManager->addShader(defaultName, newShader);
//				layerDisplay->addShader(defaultName, dlg.getTerrainOrWater());
//				layerDisplay->UpdateDisplays();
//			}
		}
	}
}


void MainWindow::on_selectionModeCombo_currentIndexChanged(int index)
{
	if (index == 1)
		ui->stackedWidget->setCurrentIndex(dataDisplayIndex);
	else if (index == 0)
		ui->stackedWidget->setCurrentIndex(layerDisplayIndex);
}


void MainWindow::enterPickingMode()
{
	ui->selectionModeCombo->setCurrentIndex(1);
}
