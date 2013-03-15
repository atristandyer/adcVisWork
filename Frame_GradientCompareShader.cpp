#include "Frame_GradientCompareShader.h"
#include "ui_Frame_GradientCompareShader.h"

Frame_GradientCompareShader::Frame_GradientCompareShader(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::Frame_GradientCompareShader)
{
	ui->setupUi(this);

	highColor = QColor(23, 255, 127, 255);
	lowColor = QColor(20, 90, 255, 255);
	wdColor = QColor(0, 0, 0, 255);

	UpdateTextValues();

	QPalette highPalette = ui->highColorFrame->palette();
	highPalette.setColor(backgroundRole(), highColor);
	ui->highColorFrame->setPalette(highPalette);
	ui->highColorFrame->setAutoFillBackground(true);

	QPalette lowPalette = ui->lowColorFrame->palette();
	lowPalette.setColor(backgroundRole(), lowColor);
	ui->lowColorFrame->setPalette(lowPalette);
	ui->lowColorFrame->setAutoFillBackground(true);

	QLinearGradient gradient (0, 0, 0, ui->previewFrame->height());
	gradient.setColorAt(0, highColor);
	gradient.setColorAt(1, lowColor);
	QPalette previewPalette = ui->previewFrame->palette();
	previewPalette.setBrush(backgroundRole(), gradient);
	ui->previewFrame->setPalette(previewPalette);
	ui->previewFrame->setAutoFillBackground(true);
}

Frame_GradientCompareShader::~Frame_GradientCompareShader()
{
	delete ui;
}

QColor Frame_GradientCompareShader::getHighColor()
{
	return highColor;
}

QColor Frame_GradientCompareShader::getLowColor()
{
	return lowColor;
}

QColor Frame_GradientCompareShader::getWDColor()
{
	return wdColor;
}

bool Frame_GradientCompareShader::getCustomRangeEnabled()
{
	return ui->useCustomRadio->isChecked();
}

float Frame_GradientCompareShader::getHighValue()
{
	if (ui->useCustomRadio->isChecked())
	{
		bool goodVal;
		float highVal = ui->highElevation->text().toFloat(&goodVal);
		if (goodVal)
		{
			ui->lowElevation->text().toFloat(&goodVal);
			if (goodVal)
				return highVal;
			else
				return 0.0;
		}
		else
			return 0.0;
	} else {
		return 0.0;
	}
}

float Frame_GradientCompareShader::getLowValue()
{
	if (ui->useCustomRadio->isChecked())
	{
		bool goodVal;
		float lowVal = ui->lowElevation->text().toFloat(&goodVal);
		if (goodVal)
		{
			ui->highElevation->text().toFloat(&goodVal);
			if (goodVal)
				return lowVal;
			else return 0.0;
		}
		else
			return 0.0;
	} else {
		return 0.0;
	}
}

void Frame_GradientCompareShader::on_highR_editingFinished()
{
	SetColorFromText(ui->highColorFrame, ui->highR, &highColor, RED);
}

void Frame_GradientCompareShader::on_highG_editingFinished()
{
	SetColorFromText(ui->highColorFrame, ui->highG, &highColor, GREEN);
}

void Frame_GradientCompareShader::on_highB_editingFinished()
{
	SetColorFromText(ui->highColorFrame, ui->highB, &highColor, BLUE);
}

void Frame_GradientCompareShader::on_highA_editingFinished()
{
	SetColorFromText(ui->highColorFrame, ui->highA, &highColor, ALPHA);
}

void Frame_GradientCompareShader::on_lowR_editingFinished()
{
	SetColorFromText(ui->lowColorFrame, ui->lowR, &lowColor, RED);
}

void Frame_GradientCompareShader::on_lowG_editingFinished()
{
	SetColorFromText(ui->lowColorFrame, ui->lowG, &lowColor, GREEN);
}

void Frame_GradientCompareShader::on_lowB_editingFinished()
{
	SetColorFromText(ui->lowColorFrame, ui->lowB, &lowColor, BLUE);
}

void Frame_GradientCompareShader::on_lowA_editingFinished()
{
	SetColorFromText(ui->lowColorFrame, ui->lowA, &lowColor, ALPHA);
}

void Frame_GradientCompareShader::on_wdR_editingFinished()
{
	SetColorFromText(ui->wdColorFrame, ui->wdR, &wdColor, RED);
}

void Frame_GradientCompareShader::on_wdG_editingFinished()
{
	SetColorFromText(ui->wdColorFrame, ui->wdG, &wdColor, GREEN);
}

void Frame_GradientCompareShader::on_wdB_editingFinished()
{
	SetColorFromText(ui->wdColorFrame, ui->wdB, &wdColor, BLUE);
}

void Frame_GradientCompareShader::on_wdA_editingFinished()
{
	SetColorFromText(ui->wdColorFrame, ui->wdA, &wdColor, ALPHA);
}

void Frame_GradientCompareShader::on_highSelectButton_clicked()
{
	highColor = QColorDialog::getColor(Qt::white, this, "Select High Color", QColorDialog::ShowAlphaChannel);
	if (highColor.isValid())
	{
		QPalette palette = ui->highColorFrame->palette();
		palette.setColor(backgroundRole(), highColor);
		ui->highColorFrame->setPalette(palette);
		ui->highColorFrame->setAutoFillBackground(true);
	}
	UpdateTextValues();
	UpdateGradient();
}

void Frame_GradientCompareShader::on_lowSelectButton_clicked()
{
	lowColor = QColorDialog::getColor(Qt::white, this, "Select High Color", QColorDialog::ShowAlphaChannel);
	if (lowColor.isValid())
	{
		QPalette palette = ui->lowColorFrame->palette();
		palette.setColor(backgroundRole(), lowColor);
		ui->lowColorFrame->setPalette(palette);
		ui->lowColorFrame->setAutoFillBackground(true);
	}
	UpdateTextValues();
	UpdateGradient();
}

void Frame_GradientCompareShader::on_wdSelectButton_clicked()
{
	wdColor = QColorDialog::getColor(Qt::white, this, "Select W/D Difference Color", QColorDialog::ShowAlphaChannel);
	if (wdColor.isValid())
	{
		QPalette palette = ui->wdColorFrame->palette();
		palette.setColor(backgroundRole(), wdColor);
		ui->wdColorFrame->setPalette(palette);
		ui->wdColorFrame->setAutoFillBackground(true);
	}
	UpdateTextValues();
}

void Frame_GradientCompareShader::on_useCustomRadio_toggled(bool checked)
{
	if (checked)
	{
		ui->highElevation->setEnabled(true);
		ui->lowElevation->setEnabled(true);
	} else {
		ui->highElevation->setEnabled(false);
		ui->lowElevation->setEnabled(false);
	}
}

void Frame_GradientCompareShader::UpdateGradient()
{
	QPalette highPalette = ui->highColorFrame->palette();
	highPalette.setColor(backgroundRole(), highColor);
	ui->highColorFrame->setPalette(highPalette);
	ui->highColorFrame->setAutoFillBackground(true);

	QPalette lowPalette = ui->lowColorFrame->palette();
	lowPalette.setColor(backgroundRole(), lowColor);
	ui->lowColorFrame->setPalette(lowPalette);
	ui->lowColorFrame->setAutoFillBackground(true);

	QLinearGradient gradient (0, 0, 0, ui->previewFrame->height());
	gradient.setColorAt(0, highColor);
	gradient.setColorAt(1, lowColor);
	QPalette previewPalette = ui->previewFrame->palette();
	previewPalette.setBrush(backgroundRole(), gradient);
	ui->previewFrame->setPalette(previewPalette);
	ui->previewFrame->setAutoFillBackground(true);
}

void Frame_GradientCompareShader::SetColorFromText(QFrame *frame, QLineEdit *line, QColor *color, int colorID)
{
	bool goodVal;
	int newColor = line->text().toInt(&goodVal, 10);
	if (goodVal && newColor >= 0 && newColor <= 255)
	{
		QPalette palette = frame->palette();
		if (colorID == RED)
			color->setRed(newColor);
		else if (colorID == GREEN)
			color->setGreen(newColor);
		else if (colorID == BLUE)
			color->setBlue(newColor);
		else if (colorID == ALPHA)
			color->setAlpha(newColor);
		palette.setColor(backgroundRole(), *color);
		frame->setPalette(palette);
		UpdateGradient();
		return;
	}

	if (colorID == RED)
		line->setText(QString::number(frame->palette().color(backgroundRole()).red()));
	else if (colorID == GREEN)
		line->setText(QString::number(frame->palette().color(backgroundRole()).green()));
	else if (colorID == BLUE)
		line->setText(QString::number(frame->palette().color(backgroundRole()).blue()));
	else if (colorID == ALPHA)
		line->setText(QString::number(frame->palette().color(backgroundRole()).alpha()));
}

void Frame_GradientCompareShader::UpdateTextValues()
{
	ui->highR->setText(QString::number(highColor.red()));
	ui->highG->setText(QString::number(highColor.green()));
	ui->highB->setText(QString::number(highColor.blue()));
	ui->highA->setText(QString::number(highColor.alpha()));
	ui->lowR->setText(QString::number(lowColor.red()));
	ui->lowG->setText(QString::number(lowColor.green()));
	ui->lowB->setText(QString::number(lowColor.blue()));
	ui->lowA->setText(QString::number(lowColor.alpha()));
	ui->wdR->setText(QString::number(wdColor.red()));
	ui->wdG->setText(QString::number(wdColor.green()));
	ui->wdB->setText(QString::number(wdColor.blue()));
	ui->wdA->setText(QString::number(wdColor.alpha()));
}
