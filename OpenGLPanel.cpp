#include "OpenGLPanel.h"
#include <QMouseEvent>
#include <iostream>


OpenGLPanel::OpenGLPanel(QWidget *parent) :
	QGLWidget(parent)
{
	setFocus();
	setFocusPolicy(Qt::ClickFocus);

	layerManager = 0;
	terrainLayer = 0;
	waterLayer = 0;
	refinedTerrainLayer = 0;
	refinedWaterLayer = 0;

	createActions();
	pickingMode = NO_PICKING;
}


OpenGLPanel::~OpenGLPanel()
{

}


GLCamera2D *OpenGLPanel::GetCameraPointer()
{
	return &camera;
}


void OpenGLPanel::AddLayer(Layer *newLayer)
{
	layerList.push_back(newLayer);
}


void OpenGLPanel::ClearLayers()
{
	layerList.clear();
}


void OpenGLPanel::SetTerrainLayer(TerrainLayer *newLayer)
{
	if (newLayer != 0)
	{
		waterLayer = 0;
		refinedTerrainLayer = 0;
		refinedWaterLayer = 0;
		terrainLayer = newLayer;
	}
}


void OpenGLPanel::SetWaterLayer(WaterLayer *newLayer)
{
	if (newLayer != 0 && terrainLayer != 0)
	{
		refinedTerrainLayer = 0;
		refinedWaterLayer = 0;
		waterLayer = newLayer;
	}
}


void OpenGLPanel::SetTerrainLayer(RefinedTerrainLayer *newLayer)
{
	if (newLayer != 0)
	{
		refinedWaterLayer = 0;
		terrainLayer = 0;
		waterLayer = 0;
		refinedTerrainLayer = newLayer;
	}
}


void OpenGLPanel::SetWaterLayer(RefinedWaterLayer *newLayer)
{
	if (newLayer != 0 && refinedTerrainLayer != 0)
	{
		terrainLayer = 0;
		waterLayer = 0;
		refinedWaterLayer = newLayer;
	}
}


void OpenGLPanel::SetLayerManager(LayerManager *newManager)
{
	layerManager = newManager;
	layerManager->setCameraPointer(&camera);
}



void OpenGLPanel::initializeGL()
{
	glewExperimental = true;
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::cout << "Error: GLEW unable to load OpenGL Extensions" << std::endl;
		exit(EXIT_FAILURE);
	} else {
//		cout << "OpenGL Version: "  << glGetString(GL_VERSION) << endl;
//		cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
//              cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << endl;
//              cout << "OpenGL Platform Vendor: " << glGetString(GL_VENDOR) << endl;
//              cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << endl;
	}

	glClearColor(0.1, 0.2, 0.3, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(10);
	glEnable(GL_POINT_SMOOTH);
}


void OpenGLPanel::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	camera.SetWindowSize(-1.0*w/h, 1.0*w/h, -1.0, 1.0, -100.0, 100.0);
}


void OpenGLPanel::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	for (unsigned int i=0; i<layerList.size(); i++)
//	{
//		layerList[i]->Draw();
//	}
	if (layerManager && layerManager->getActiveTerrainLayer())
	{
		layerManager->getActiveTerrainLayer()->Draw();
		if (layerManager->getActiveWaterLayer())
		{
			layerManager->getActiveWaterLayer()->Draw();
		}
	}
	else if (layerManager && layerManager->getActiveRefinedTerrainLayer())
	{
		layerManager->getActiveRefinedTerrainLayer()->Draw();
		if (layerManager->getActiveRefinedWaterLayer())
		{
			layerManager->getActiveRefinedWaterLayer()->Draw();
		}
	}
	else if (layerManager && layerManager->getActiveCompareTerrainLayer())
	{
		layerManager->getActiveCompareTerrainLayer()->Draw();
		if (layerManager->getActiveCompareWaterLayer())
		{
			layerManager->getActiveCompareWaterLayer()->Draw();
		}
	}
}


void OpenGLPanel::wheelEvent(QWheelEvent * event)
{
	camera.Zoom(event->delta());
	updateGL();
}


void OpenGLPanel::mousePressEvent(QMouseEvent *event)
{
	pushedButton = event->button();
	oldx = event->x();
	oldy = event->y();
	mouseMoved = false;
}


void OpenGLPanel::mouseMoveEvent(QMouseEvent *event)
{
	mouseMoved = true;
	newx = event->x();
	newy = event->y();
	dx = newx-oldx;
	dy = newy-oldy;

	if (pushedButton == Qt::LeftButton)
	{
		camera.Pan(dx, dy);
	}
	else if (pushedButton == Qt::RightButton)
	{

	}

	oldx = newx;
	oldy = newy;

	updateGL();
}


void OpenGLPanel::mouseReleaseEvent(QMouseEvent *event)
{
	if (!mouseMoved)
	{
		float x = 0.0;
		float y = 0.0;

		camera.GetUnprojectedPoint(event->x(), event->y(), &x, &y);

		if (layerManager && layerManager->getActiveTerrainLayer())
		{
			if (pickingMode == NODE_MODE)
			{
				emit enteringPickingMode();
				emit selectedNodeUpdated(layerManager->getActiveTerrainLayer()->pickNode(x, y));
			}
			else if (pickingMode == ELEMENT_MODE)
			{
				emit enteringPickingMode();
				emit selectedElementUpdated(layerManager->getActiveTerrainLayer()->pickElement(x, y));
			}
			updateGL();
		}
	}
}


void OpenGLPanel::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Up)
	{
		if (layerManager && layerManager->getActiveWaterLayer())
		{
			WaterLayer *currLayer = layerManager->getActiveWaterLayer();
			for (int i=0; i<10; i++)
			{
				currLayer->LoadTimestep(currLayer->GetCurrentTimestep()+1);
				emit currentTimestep(layerManager->getActiveWaterLayer()->GetCurrentTimestep());
				updateGL();
			}
		}
		else if (layerManager && layerManager->getActiveRefinedWaterLayer())
		{
			RefinedWaterLayer *currLayer = layerManager->getActiveRefinedWaterLayer();
			for (int i=0; i<10; i++)
			{
				currLayer->LoadTimestep(currLayer->GetCurrentTimestep()+1);
				emit currentTimestep(layerManager->getActiveRefinedWaterLayer()->GetCurrentTimestep());
				updateGL();
			}
		}
		else if (layerManager && layerManager->getActiveCompareWaterLayer())
		{
			WaterLayer_Compare_Full_Sub *currLayer = layerManager->getActiveCompareWaterLayer();
			for (int i=0; i<10; i++)
			{
				currLayer->LoadTimestep(currLayer->GetCurrentTimestep()+1);
				emit currentTimestep(layerManager->getActiveCompareWaterLayer()->GetCurrentTimestep());
				updateGL();
			}
		}
	}
	else if (event->key() == Qt::Key_Down)
	{
		if (layerManager && layerManager->getActiveWaterLayer())
		{
			WaterLayer *currLayer = layerManager->getActiveWaterLayer();
			for (int i=0; i<10; i++)
			{
				currLayer->LoadTimestep(currLayer->GetCurrentTimestep()-1);
				emit currentTimestep(layerManager->getActiveWaterLayer()->GetCurrentTimestep());
				updateGL();
			}
		}
		else if (layerManager && layerManager->getActiveRefinedWaterLayer())
		{
			RefinedWaterLayer *currLayer = layerManager->getActiveRefinedWaterLayer();
			for (int i=0; i<10; i++)
			{
				currLayer->LoadTimestep(currLayer->GetCurrentTimestep()-1);
				emit currentTimestep(layerManager->getActiveRefinedWaterLayer()->GetCurrentTimestep());
				updateGL();
			}
		}
		else if (layerManager && layerManager->getActiveCompareWaterLayer())
		{
			WaterLayer_Compare_Full_Sub *currLayer = layerManager->getActiveCompareWaterLayer();
			for (int i=0; i<10; i++)
			{
				currLayer->LoadTimestep(currLayer->GetCurrentTimestep()-1);
				emit currentTimestep(layerManager->getActiveCompareWaterLayer()->GetCurrentTimestep());
				updateGL();
			}
		}
	}
	else if (event->key() == Qt::Key_Left)
	{
		if (layerManager && layerManager->getActiveWaterLayer())
		{
			layerManager->getActiveWaterLayer()->LoadTimestep(layerManager->getActiveWaterLayer()->GetCurrentTimestep()-1);
			emit currentTimestep(layerManager->getActiveWaterLayer()->GetCurrentTimestep());
		}
		else if (layerManager && layerManager->getActiveRefinedWaterLayer())
		{
			layerManager->getActiveRefinedWaterLayer()->LoadTimestep(layerManager->getActiveRefinedWaterLayer()->GetCurrentTimestep()-1);
			emit currentTimestep(layerManager->getActiveRefinedWaterLayer()->GetCurrentTimestep());
		}
		else if (layerManager && layerManager->getActiveCompareWaterLayer())
		{
			layerManager->getActiveCompareWaterLayer()->LoadTimestep(layerManager->getActiveCompareWaterLayer()->GetCurrentTimestep()-1);
			emit currentTimestep(layerManager->getActiveCompareWaterLayer()->GetCurrentTimestep());
		}
		updateGL();
	}
	else if (event->key() == Qt::Key_Right)
	{
		if (layerManager && layerManager->getActiveWaterLayer())
		{
			layerManager->getActiveWaterLayer()->LoadTimestep(layerManager->getActiveWaterLayer()->GetCurrentTimestep()+1);
			emit currentTimestep(layerManager->getActiveWaterLayer()->GetCurrentTimestep());
		}
		else if (layerManager && layerManager->getActiveRefinedWaterLayer())
		{
			layerManager->getActiveRefinedWaterLayer()->LoadTimestep(layerManager->getActiveRefinedWaterLayer()->GetCurrentTimestep()+1);
			emit currentTimestep(layerManager->getActiveRefinedWaterLayer()->GetCurrentTimestep());
		}
		else if (layerManager && layerManager->getActiveCompareWaterLayer())
		{
			layerManager->getActiveCompareWaterLayer()->LoadTimestep(layerManager->getActiveCompareWaterLayer()->GetCurrentTimestep()+1);
			emit currentTimestep(layerManager->getActiveCompareWaterLayer()->GetCurrentTimestep());
		}
		updateGL();
	}
	else if (event->key() == Qt::Key_R)
	{
		camera.ResetView();
		updateGL();
		return;
	}
	else if (event->key() == Qt::Key_N)
	{
		emit enteringPickingMode();
		nodePickMode();
	}
	else if (event->key() == Qt::Key_E)
	{
		emit enteringPickingMode();
		elementPickMode();
	}
	else if (event->key() == Qt::Key_C)
	{
		clearPicking();
	}

//	if (layerManager && layerManager->getActiveWaterLayer())
//	{
//		emit currentTimestep(layerManager->getActiveWaterLayer()->GetCurrentTimestep());
//	}
//	else if (layerManager && layerManager->getActiveRefinedWaterLayer())
//	{
//		emit currentTimestep(layerManager->getActiveRefinedWaterLayer()->GetCurrentTimestep());
//	}
//	if (waterLayer)
//		emit currentTimestep(waterLayer->GetCurrentTimestep());
}


void OpenGLPanel::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
	menu.addAction(nodeModeAction);
	menu.addAction(elementModeAction);
	menu.addAction(clearPickingAction);
	menu.exec(event->globalPos());
}


void OpenGLPanel::createActions()
{
	nodeModeAction = new QAction(tr("Select Nodes"), this);
	nodeModeAction->setStatusTip(tr("Left-click to select nodes in mesh"));
	connect(nodeModeAction, SIGNAL(triggered()), this, SLOT(nodePickMode()));

	elementModeAction = new QAction(tr("Select Elements"), this);
	elementModeAction->setStatusTip(tr("Left-click to select elements in mesh"));
	connect(elementModeAction, SIGNAL(triggered()), this, SLOT(elementPickMode()));

	clearPickingAction = new QAction(tr("Clear All Selections"), this);
	clearPickingAction->setStatusTip(tr("Deselects any selected node or element"));
	connect(clearPickingAction, SIGNAL(triggered()), this, SLOT(clearPicking()));
}


void OpenGLPanel::nodePickMode()
{
	if (layerManager && layerManager->getActiveTerrainLayer())
		layerManager->getActiveTerrainLayer()->clearPicking();
//	if (terrainLayer)
//		terrainLayer->clearPicking();
	pickingMode = NODE_MODE;
	emit selectedElementUpdated(0);
	updateGL();
}


void OpenGLPanel::elementPickMode()
{
	if (layerManager && layerManager->getActiveTerrainLayer())
		layerManager->getActiveTerrainLayer()->clearPicking();
//	if (terrainLayer)
//		terrainLayer->clearPicking();
	pickingMode = ELEMENT_MODE;
	emit selectedNodeUpdated(0);
	updateGL();
}


void OpenGLPanel::clearPicking()
{
	if (layerManager && layerManager->getActiveTerrainLayer())
	{
		layerManager->getActiveTerrainLayer()->clearPicking();
//	if (terrainLayer)
//	{
//		terrainLayer->clearPicking();
		emit selectedNodeUpdated(0);
		emit selectedElementUpdated(0);
		updateGL();
	}
}


void OpenGLPanel::pickNode(int nodeNum)
{
	nodePickMode();
	if (layerManager && layerManager->getActiveTerrainLayer())
		emit selectedNodeUpdated(layerManager->getActiveTerrainLayer()->pickNode(nodeNum));
//	if (terrainLayer)
//		emit selectedNodeUpdated(terrainLayer->pickNode(nodeNum));
	updateGL();
}


void OpenGLPanel::pickElement(int elementNum)
{
	elementPickMode();
	if (layerManager && layerManager->getActiveTerrainLayer())
		emit selectedElementUpdated(layerManager->getActiveTerrainLayer()->pickElement(elementNum));
//	if (terrainLayer)
//		emit selectedElementUpdated(terrainLayer->pickElement(elementNum));
	updateGL();
}










































