#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LayerManager.h"

// Dialog and Frame includes
#include "Dialog_NormalMesh.h"
#include "Dialog_Compare_Sub_Full.h"
#include "Dialog_ShaderEditor.h"
#include "Frame_DataDisplay.h"
#include "Frame_LayerDisplay.h"

#include "RefinedShader.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
		
	private slots:
		// Menu Bar Actions
		void on_actionNormal_Mesh_triggered();
		void on_actionSub_Full_Domain_triggered();
		void on_actionEditTerrainShader_triggered();
		void on_actionEditWaterShader_triggered();

		// All Other Actions/Slots
		void on_selectionModeCombo_currentIndexChanged(int index);
		void enterPickingMode();

	private:
		Ui::MainWindow		*ui;

		// Frame Data
		Frame_DataDisplay	*dataDisplay;
		Frame_LayerDisplay	*layerDisplay;
		int			dataDisplayIndex;
		int			layerDisplayIndex;

		// Layer Manager
		LayerManager		*layerManager;


};

#endif // MAINWINDOW_H
