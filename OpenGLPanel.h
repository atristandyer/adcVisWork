#ifndef OPENGLPANEL_H
#define OPENGLPANEL_H

#include "glew.h"
#include "GLCamera2D.h"
#include "Layer.h"
#include "TerrainLayer.h"
#include "WaterLayer.h"
#include "RefinedTerrainLayer.h"
#include "RefinedWaterLayer.h"
#include "LayerManager.h"
#include <QGLWidget>
#include <QMenu>
#include <vector>

#define NO_PICKING 0
#define NODE_MODE 1
#define ELEMENT_MODE 2

class OpenGLPanel : public QGLWidget
{
		Q_OBJECT
	public:
		explicit	OpenGLPanel(QWidget *parent = 0);
		~OpenGLPanel();

		// Function Definitions
		GLCamera2D	*GetCameraPointer();
		void		AddLayer(Layer *newLayer);
		void		ClearLayers();
		void		SetTerrainLayer(TerrainLayer *newLayer);
		void		SetTerrainLayer(RefinedTerrainLayer *newLayer);
		void		SetWaterLayer(WaterLayer *newLayer);
		void		SetWaterLayer(RefinedWaterLayer *newLayer);
		void		SetLayerManager(LayerManager *newManager);


	protected:

		std::vector<Layer*>	layerList;


		void		initializeGL();
		void		resizeGL(int w, int h);
		void		paintGL();

		void		wheelEvent(QWheelEvent *event);
		void		mousePressEvent(QMouseEvent *event);
		void		mouseMoveEvent(QMouseEvent *event);
		void		mouseReleaseEvent(QMouseEvent *event);
		void		keyReleaseEvent(QKeyEvent *event);
		void		contextMenuEvent(QContextMenuEvent *event);


	private:

		GLCamera2D	camera;

		int		oldx, oldy, newx, newy, dx, dy;
		bool		mouseMoved;
		enum		Qt::MouseButton pushedButton;
		int		pickingMode;

		// Pointers to layers
		LayerManager		*layerManager;
		TerrainLayer		*terrainLayer;
		WaterLayer		*waterLayer;
		RefinedTerrainLayer	*refinedTerrainLayer;
		RefinedWaterLayer	*refinedWaterLayer;

		// Context Menu Stuff
		void	createActions();
		QMenu	*rightClickMenu;
		QAction	*nodeModeAction;
		QAction	*elementModeAction;
		QAction	*clearPickingAction;


	signals:
		void	GLUpdated();
		void	currentTimestep(int);
		void	selectedNodeUpdated(Node*);
		void	selectedElementUpdated(Element*);
		void	enteringPickingMode();


	public slots:
		void	pickNode(int nodeNum);
		void	pickElement(int elementNum);


	private slots:
		void	nodePickMode();
		void	elementPickMode();
		void	clearPicking();

};

#endif // OPENGLPANEL_H
