#ifndef FRAME_DATADISPLAY_H
#define FRAME_DATADISPLAY_H

#include <QFrame>
#include "adcData.h"
#include "LayerManager.h"
#include "OpenGLPanel.h"

namespace Ui {
	class Frame_DataDisplay;
}

class Frame_DataDisplay : public QFrame
{
		Q_OBJECT
		
	public:

		explicit Frame_DataDisplay(QWidget *parent = 0);
		~Frame_DataDisplay();

		void	SetLayerManager(LayerManager *manager);
		void	SetGLPanel(OpenGLPanel *panel);
		
	private:

		Ui::Frame_DataDisplay	*ui;
		LayerManager		*layerManager;
		OpenGLPanel		*GLPanel;

		void	ClearDataTable();
		void	DisplayNodeData(Node *currNode);
		void	DisplayElementData(Element *currElement);

	public slots:

		void	setCurrentTimestep(int ts);
		void	updateSelectedNode(Node *currNode);
		void	updateSelectedElement(Element *currElement);

	private slots:

		void	on_nodeLineEdit_returnPressed();
		void	on_elementLineEdit_returnPressed();
		void	on_tsLineEdit_returnPressed();
};

#endif // FRAME_DATADISPLAY_H
