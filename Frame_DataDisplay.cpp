#include "Frame_DataDisplay.h"
#include "ui_Frame_DataDisplay.h"

Frame_DataDisplay::Frame_DataDisplay(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::Frame_DataDisplay)
{
	ui->setupUi(this);
}


Frame_DataDisplay::~Frame_DataDisplay()
{
	delete ui;
}


void Frame_DataDisplay::SetLayerManager(LayerManager *manager)
{
	layerManager = manager;
}


void Frame_DataDisplay::SetGLPanel(OpenGLPanel *panel)
{
	GLPanel = panel;
}


void Frame_DataDisplay::ClearDataTable()
{
	ui->dataTable->clear();
	ui->dataTable->setRowCount(0);
}


void Frame_DataDisplay::DisplayNodeData(Node *currNode)
{
	ui->dataTable->setColumnCount(2);

	ui->dataTable->insertRow(ui->dataTable->rowCount());
	ui->dataTable->setSpan(ui->dataTable->rowCount()-1, 0, 1, 2);
	QTableWidgetItem *numText  = new QTableWidgetItem(tr("Node #").append(QString::number(currNode->nodeNumber)));
	numText->setTextAlignment(Qt::AlignCenter);
	numText->setBackgroundColor(QColor("darkCyan"));
	numText->setTextColor(QColor("white"));
	QFont font;
	font.setBold(true);
	numText->setFont(font);
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 0, numText);

	ui->dataTable->insertRow(ui->dataTable->rowCount());
	QTableWidgetItem *xText  = new QTableWidgetItem("x");
	QTableWidgetItem *xVal = new QTableWidgetItem(QString::number(currNode->x));
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 0, xText);
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 1, xVal);

	ui->dataTable->insertRow(ui->dataTable->rowCount());
	QTableWidgetItem *yText  = new QTableWidgetItem("y");
	QTableWidgetItem *yVal = new QTableWidgetItem(QString::number(currNode->y));
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 0, yText);
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 1, yVal);

	ui->dataTable->insertRow(ui->dataTable->rowCount());
	QTableWidgetItem *zText  = new QTableWidgetItem("z");
	QTableWidgetItem *zVal = new QTableWidgetItem(QString::number(currNode->z));
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 0, zText);
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 1, zVal);
}


void Frame_DataDisplay::DisplayElementData(Element *currElement)
{
	ui->dataTable->setColumnCount(2);

	ui->dataTable->insertRow(ui->dataTable->rowCount());
	ui->dataTable->setSpan(ui->dataTable->rowCount()-1, 0, 1, 2);
	QTableWidgetItem *numText  = new QTableWidgetItem(tr("Element #").append(QString::number(currElement->elementNumber)));
	numText->setTextAlignment(Qt::AlignCenter);
	numText->setBackgroundColor(QColor("darkRed"));
	numText->setTextColor(QColor("white"));
	QFont font;
	font.setBold(true);
	numText->setFont(font);
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 0, numText);

	ui->dataTable->insertRow(ui->dataTable->rowCount());
	QTableWidgetItem *n1Text  = new QTableWidgetItem("Node #1");
	QTableWidgetItem *n1Val = new QTableWidgetItem(QString::number(currElement->n1));
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 0, n1Text);
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 1, n1Val);

	ui->dataTable->insertRow(ui->dataTable->rowCount());
	QTableWidgetItem *n2Text  = new QTableWidgetItem("Node #2");
	QTableWidgetItem *n2Val = new QTableWidgetItem(QString::number(currElement->n2));
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 0, n2Text);
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 1, n2Val);

	ui->dataTable->insertRow(ui->dataTable->rowCount());
	QTableWidgetItem *n3Text  = new QTableWidgetItem("Node #3");
	QTableWidgetItem *n3Val = new QTableWidgetItem(QString::number(currElement->n3));
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 0, n3Text);
	ui->dataTable->setItem(ui->dataTable->rowCount()-1, 1, n3Val);

	if (layerManager && layerManager->getActiveTerrainLayer())
	{
		Node *currNode = layerManager->getActiveTerrainLayer()->getNodePointer(currElement->n1);
		if (currNode)
			DisplayNodeData(currNode);
		currNode = layerManager->getActiveTerrainLayer()->getNodePointer(currElement->n2);
		if (currNode)
			DisplayNodeData(currNode);
		currNode = layerManager->getActiveTerrainLayer()->getNodePointer(currElement->n3);
		if (currNode)
			DisplayNodeData(currNode);
	}
}


void Frame_DataDisplay::setCurrentTimestep(int ts)
{
	ui->tsLineEdit->setText(QString().number(ts));
}


void Frame_DataDisplay::updateSelectedNode(Node *currNode)
{
	ClearDataTable();

	if (!currNode)
		ui->nodeLineEdit->setText(tr(""));
	else
	{
		ui->nodeLineEdit->setText(QString::number(currNode->nodeNumber));
		DisplayNodeData(currNode);
	}
}


void Frame_DataDisplay::updateSelectedElement(Element *currElement)
{
	ClearDataTable();

	if (!currElement)
		ui->elementLineEdit->setText(tr(""));
	else
	{
		ui->elementLineEdit->setText(QString::number(currElement->elementNumber));
		DisplayElementData(currElement);
	}
}

void Frame_DataDisplay::on_nodeLineEdit_returnPressed()
{
	bool check;
	int nodeNum = ui->nodeLineEdit->text().toInt(&check, 10);
	if (check && GLPanel)
	{
		GLPanel->pickNode(nodeNum);
	}
}

void Frame_DataDisplay::on_elementLineEdit_returnPressed()
{
	bool check;
	int elementNum = ui->elementLineEdit->text().toInt(&check, 10);
	if (check && GLPanel)
	{
		GLPanel->pickElement(elementNum);
	}
}

void Frame_DataDisplay::on_tsLineEdit_returnPressed()
{
	bool check;
	int ts = ui->tsLineEdit->text().toInt(&check, 10);
	if (check && GLPanel)
	{
		if (layerManager && layerManager->getActiveWaterLayer())
			layerManager->getActiveWaterLayer()->LoadTimestep(ts);
		else if (layerManager && layerManager->getActiveRefinedWaterLayer())
			layerManager->getActiveRefinedWaterLayer()->LoadTimestep(ts);
		else if (layerManager && layerManager->getActiveCompareWaterLayer())
			layerManager->getActiveCompareWaterLayer()->LoadTimestep(ts);
		GLPanel->updateGL();
	}
}
