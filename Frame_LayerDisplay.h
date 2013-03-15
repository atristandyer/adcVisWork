#ifndef FRAME_LAYERDISPLAY_H
#define FRAME_LAYERDISPLAY_H

#include <QFrame>
#include <vector>
#include "LayerManager.h"


typedef struct {
		QString	meshName;
		int	listIndex;
		int	terrainOutline;
		int	terrainFill;
		int	waterOutline;
		int	waterFill;
} meshRecord;


typedef struct {
		QString shaderName;
		int	listIndex;
		int	type;
} shaderRecord;


namespace Ui {
	class Frame_LayerDisplay;
}

class Frame_LayerDisplay : public QFrame
{
		Q_OBJECT
		
	public:
		explicit Frame_LayerDisplay(QWidget *parent = 0);
		~Frame_LayerDisplay();

		void	SetLayerManager(LayerManager *manager);
		void	UpdateDisplays();

		// Insert functions
		void	addLayer(QString name);
		void	addShader(QString name, int type);
		
	private slots:
		void on_terrainOutline_currentRowChanged(int currentRow);
		void on_terrainFill_currentRowChanged(int currentRow);
		void on_waterOutline_currentRowChanged(int currentRow);
		void on_waterFill_currentRowChanged(int currentRow);
		void on_meshesList_currentRowChanged(int currentRow);

	private:
		Ui::Frame_LayerDisplay		*ui;
		LayerManager			*layerManager;
		std::vector<meshRecord>		meshRecords;
		std::vector<shaderRecord>	shaderRecords;

	signals:
		void updateGL();

	public slots:

		void updateWaterGradients(int);


};

#endif // FRAME_LAYERDISPLAY_H
