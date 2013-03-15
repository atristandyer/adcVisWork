#include "Dialog_ShaderEditor.h"
#include "ui_Dialog_ShaderEditor.h"

Dialog_ShaderEditor::Dialog_ShaderEditor(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog_ShaderEditor)
{
	ui->setupUi(this);

	ui->tabWidget->setTabText(0, tr("Terrain"));
	ui->tabWidget->setTabText(1, tr("Water"));

	defaultShaderFrameTerrain = new Frame_DefaultShader();
	defaultShaderFrameWater = new Frame_DefaultShader();
	gradientShaderFrameTerrain = new Frame_GradientShader();
	gradientShaderFrameWater = new Frame_GradientShader();
	gradientCompareShaderFrame = new Frame_GradientCompareShader();

	defaultShaderIndexTerrain = ui->stackedWidget->addWidget(defaultShaderFrameTerrain);
	defaultShaderIndexWater = ui->stackedWidgetWater->addWidget(defaultShaderFrameWater);
	gradientShaderIndexTerrain = ui->stackedWidget->addWidget(gradientShaderFrameTerrain);
	gradientShaderIndexWater = ui->stackedWidgetWater->addWidget(gradientShaderFrameWater);
	gradientCompareShaderIndex = ui->stackedWidgetWater->addWidget(gradientCompareShaderFrame);

	connect(ui->shaderList, SIGNAL(currentRowChanged(int)), this, SLOT(shaderPicked(int)));
	connect(ui->shaderListWater, SIGNAL(currentRowChanged(int)), this, SLOT(shaderPicked(int)));
}


Dialog_ShaderEditor::~Dialog_ShaderEditor()
{
	delete ui;
}


void Dialog_ShaderEditor::initialTab(int index)
{
	ui->tabWidget->setCurrentIndex(index);
}


void Dialog_ShaderEditor::setDefaultShaderName(QString name)
{
	ui->shaderName->setText(name);
}


//GLShader *Dialog_ShaderEditor::getNewShader()
//{
//	// Making a terrain shader
//	if (ui->tabWidget->currentIndex() == 0)
//	{
//		// Making a solid shader
//		if (ui->stackedWidget->currentIndex() == defaultShaderIndexTerrain && defaultShaderFrameTerrain)
//		{
//			DefaultShader *newShader = new DefaultShader();
//			QColor color = defaultShaderFrameTerrain->getColor();
//			newShader->SetColor(color.red()/255.0, color.green()/255.0, color.blue()/255.0, color.alpha()/255.0);
//			return newShader;
//		}
//		// Making a gradient shader
//		else if (ui->stackedWidget->currentIndex() == gradientShaderIndexTerrain && gradientShaderFrameTerrain)
//		{
//			GradientShader *newShader = new GradientShader();
//			QColor highColor = gradientShaderFrameTerrain->getHighColor();
//			QColor lowColor = gradientShaderFrameTerrain->getLowColor();
//			bool customRange = gradientShaderFrameTerrain->getCustomRangeEnabled();
//			newShader->SetTopColor(highColor.red()/255.0, highColor.green()/255.0, highColor.blue()/255.0, highColor.alpha()/255.0);
//			newShader->SetBottomColor(lowColor.red()/255.0, lowColor.green()/255.0, lowColor.blue()/255.0, lowColor.alpha()/255.0);
//			if (customRange)
//			{
//				newShader->SetHeightRange(gradientShaderFrameTerrain->getLowValue(), gradientShaderFrameTerrain->getHighValue());
//			} else {
//				newShader->SetHeightRange(-10.0, 10.0);
//			}
//			return newShader;
//		}
//	}
//	// Making a water shader
//	else if (ui->tabWidget->currentIndex() == 1)
//	{
//		// Making a solid shader
//		if (ui->stackedWidgetWater->currentIndex() == defaultShaderIndexWater && defaultShaderFrameWater)
//		{
//			DefaultShader *newShader = new DefaultShader();
//			QColor color = defaultShaderFrameWater->getColor();
//			newShader->SetColor(color.red()/255.0, color.green()/255.0, color.blue()/255.0, color.alpha()/255.0);
//			return newShader;
//		}
//		// Making a gradient shader
//		else if (ui->stackedWidgetWater->currentIndex() == gradientShaderIndexWater && gradientShaderFrameWater)
//		{
//			GradientShader *newShader = new GradientShader();
//			QColor highColor = gradientShaderFrameWater->getHighColor();
//			QColor lowColor = gradientShaderFrameWater->getLowColor();
//			bool customRange = gradientShaderFrameWater->getCustomRangeEnabled();
//			newShader->SetTopColor(highColor.red()/255.0, highColor.green()/255.0, highColor.blue()/255.0, highColor.alpha()/255.0);
//			newShader->SetBottomColor(lowColor.red()/255.0, lowColor.green()/255.0, lowColor.blue()/255.0, lowColor.alpha()/255.0);
//			if (customRange)
//			{
//				newShader->SetHeightRange(gradientShaderFrameWater->getLowValue(), gradientShaderFrameWater->getHighValue());
//			} else {
//				newShader->SetHeightRange(-10.0, 10.0);
//			}
//			return newShader;
//		}
//	}
//	return 0;
//}


DefaultShader *Dialog_ShaderEditor::getNewDefaultShader()
{
	// Making a terrain shader
	if (ui->tabWidget->currentIndex() == 0)
	{
		// Making a solid shader
		if (ui->stackedWidget->currentIndex() == defaultShaderIndexTerrain && defaultShaderFrameTerrain)
		{
			DefaultShader *newShader = new DefaultShader();
			QColor color = defaultShaderFrameTerrain->getColor();
			newShader->SetColor(color.red()/255.0, color.green()/255.0, color.blue()/255.0, color.alpha()/255.0);
			return newShader;
		}
	}
	// Making a water shader
	else if (ui->tabWidget->currentIndex() == 1)
	{
		// Making a solid shader
		if (ui->stackedWidgetWater->currentIndex() == defaultShaderIndexWater && defaultShaderFrameWater)
		{
			DefaultShader *newShader = new DefaultShader();
			QColor color = defaultShaderFrameWater->getColor();
			newShader->SetColor(color.red()/255.0, color.green()/255.0, color.blue()/255.0, color.alpha()/255.0);
			return newShader;
		}
	}
	return 0;
}


GradientShader *Dialog_ShaderEditor::getNewGradientShader()
{
	// Making a terrain shader
	if (ui->tabWidget->currentIndex() == 0)
	{
		if (ui->stackedWidget->currentIndex() == gradientShaderIndexTerrain && gradientShaderFrameTerrain)
		{
			GradientShader *newShader = new GradientShader();
			QColor highColor = gradientShaderFrameTerrain->getHighColor();
			QColor lowColor = gradientShaderFrameTerrain->getLowColor();
			bool customRange = gradientShaderFrameTerrain->getCustomRangeEnabled();
			newShader->SetTopColor(highColor.red()/255.0, highColor.green()/255.0, highColor.blue()/255.0, highColor.alpha()/255.0);
			newShader->SetBottomColor(lowColor.red()/255.0, lowColor.green()/255.0, lowColor.blue()/255.0, lowColor.alpha()/255.0);
			if (customRange)
			{
				newShader->UseCustomHeightRange = true;
				newShader->SetHeightRange(gradientShaderFrameTerrain->getLowValue(), gradientShaderFrameTerrain->getHighValue());
			} else {
				newShader->UseCustomHeightRange = false;
			}
			return newShader;
		}
	}
	// Making a water shader
	else if (ui->tabWidget->currentIndex() == 1)
	{
		// Making a gradient shader
		if (ui->stackedWidgetWater->currentIndex() == gradientShaderIndexWater && gradientShaderFrameWater)
		{
			GradientShader *newShader = new GradientShader();
			QColor highColor = gradientShaderFrameWater->getHighColor();
			QColor lowColor = gradientShaderFrameWater->getLowColor();
			bool customRange = gradientShaderFrameWater->getCustomRangeEnabled();
			newShader->SetTopColor(highColor.red()/255.0, highColor.green()/255.0, highColor.blue()/255.0, highColor.alpha()/255.0);
			newShader->SetBottomColor(lowColor.red()/255.0, lowColor.green()/255.0, lowColor.blue()/255.0, lowColor.alpha()/255.0);
			if (customRange)
			{
				newShader->UseCustomHeightRange = true;
				newShader->SetHeightRange(gradientShaderFrameWater->getLowValue(), gradientShaderFrameWater->getHighValue());
			} else {
				newShader->UseCustomHeightRange = false;
			}
			return newShader;
		}
	}
	return 0;
}


GradientCompareShader *Dialog_ShaderEditor::getNewGradientCompareShader()
{
	// Must be a water layer
	if (ui->tabWidget->currentIndex() == 1)
	{
		if (ui->stackedWidgetWater->currentIndex() == gradientCompareShaderIndex && gradientCompareShaderFrame)
		{
			GradientCompareShader *newShader = new GradientCompareShader();
			QColor highColor = gradientCompareShaderFrame->getHighColor();
			QColor lowColor = gradientCompareShaderFrame->getLowColor();
			QColor wdColor = gradientCompareShaderFrame->getWDColor();
			bool customRange = gradientCompareShaderFrame->getCustomRangeEnabled();
			newShader->SetTopColor(highColor.red()/255.0, highColor.green()/255.0, highColor.blue()/255.0, highColor.alpha()/255.0);
			newShader->SetBottomColor(lowColor.red()/255.0, lowColor.green()/255.0, lowColor.blue()/255.0, lowColor.alpha()/255.0);
			newShader->SetWDColor(wdColor.red()/255.0, wdColor.green()/255.0, wdColor.blue()/255.0, wdColor.alpha()/255.0);
			if (customRange)
			{
				newShader->UseCustomHeightRange = true;
				newShader->SetHeightRange(gradientCompareShaderFrame->getLowValue(), gradientCompareShaderFrame->getHighValue());
			} else {
				newShader->UseCustomHeightRange = false;
			}

			return newShader;
		}
	}

	return 0;
}


QString Dialog_ShaderEditor::getShaderName()
{
	return ui->shaderName->text();
}


int Dialog_ShaderEditor::getShaderType()
{
	if (ui->tabWidget->currentIndex() == 0)
		return ui->shaderList->currentRow();
	else if (ui->tabWidget->currentIndex() == 1)
		return ui->shaderListWater->currentRow();
	else
		return -1;
}


int Dialog_ShaderEditor::getTerrainOrWater()
{
	return ui->tabWidget->currentIndex();
}


void Dialog_ShaderEditor::shaderPicked(int index)
{
	if (ui->tabWidget->currentIndex() == 0)
	{
		if (index == 0)
		{
			ui->stackedWidget->setCurrentIndex(defaultShaderIndexTerrain);
		} else if (index == 1)
		{
			ui->stackedWidget->setCurrentIndex(gradientShaderIndexTerrain);
		}
	}
	else if (ui->tabWidget->currentIndex() == 1)
	{
		if (index == 0)
		{
			ui->stackedWidgetWater->setCurrentIndex(defaultShaderIndexWater);
		} else if (index == 1)
		{
			ui->stackedWidgetWater->setCurrentIndex(gradientShaderIndexWater);
		} else if (index == 2)
		{
			ui->stackedWidgetWater->setCurrentIndex(gradientCompareShaderIndex);
		}
	}
}

