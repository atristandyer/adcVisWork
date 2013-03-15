#include "Frame_DefaultShader.h"
#include "ui_Frame_DefaultShader.h"

Frame_DefaultShader::Frame_DefaultShader(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::Frame_DefaultShader)
{
	ui->setupUi(this);

	// Set default color
	outlineColor = QColor(153, 153, 153, 128);
	QPalette outlinePalette = ui->outlineColorFrame->palette();
	outlinePalette.setColor(backgroundRole(), outlineColor);
	ui->outlineColorFrame->setPalette(outlinePalette);
	ui->outlineColorFrame->setAutoFillBackground(true);
	ui->outlineR->setText(tr("153"));
	ui->outlineG->setText(tr("153"));
	ui->outlineB->setText(tr("153"));
	ui->outlineA->setText(tr("128"));
}

Frame_DefaultShader::~Frame_DefaultShader()
{
	delete ui;
}


QColor Frame_DefaultShader::getColor()
{
	std::cout << outlineColor.red() << " " << outlineColor.green() << " " << outlineColor.blue() << std::endl;
	return outlineColor;
}


void Frame_DefaultShader::on_outlineSelectButton_clicked()
{
	outlineColor = QColorDialog::getColor(Qt::white, this, "Select Color", QColorDialog::ShowAlphaChannel);
	if (outlineColor.isValid())
	{
		QPalette palette = ui->outlineColorFrame->palette();
		palette.setColor(backgroundRole(), outlineColor);
		ui->outlineColorFrame->setPalette(palette);
		ui->outlineColorFrame->setAutoFillBackground(true);

		ui->outlineR->setText(QString::number(outlineColor.red()));
		ui->outlineG->setText(QString::number(outlineColor.green()));
		ui->outlineB->setText(QString::number(outlineColor.blue()));
		ui->outlineA->setText(QString::number(outlineColor.alpha()));
	}
}


void Frame_DefaultShader::on_outlineR_editingFinished()
{
	bool goodVal;
	int newRed = ui->outlineR->text().toInt(&goodVal, 10);
	if (goodVal && newRed >= 0 && newRed <= 255)
	{
		QPalette palette = ui->outlineColorFrame->palette();
		outlineColor.setRed(newRed);
		palette.setColor(backgroundRole(), outlineColor);
		ui->outlineColorFrame->setPalette(palette);
	} else {
		ui->outlineR->setText(QString::number(ui->outlineColorFrame->palette().color(backgroundRole()).red()));
	}
}

void Frame_DefaultShader::on_outlineG_editingFinished()
{
	bool goodVal;
	int newGreen = ui->outlineG->text().toInt(&goodVal, 10);
	if (goodVal && newGreen >= 0 && newGreen <= 255)
	{
		QPalette palette = ui->outlineColorFrame->palette();
		outlineColor.setGreen(newGreen);
		palette.setColor(backgroundRole(), outlineColor);
		ui->outlineColorFrame->setPalette(palette);
	} else {
		ui->outlineG->setText(QString::number(ui->outlineColorFrame->palette().color(backgroundRole()).green()));
	}
}

void Frame_DefaultShader::on_outlineB_editingFinished()
{
	bool goodVal;
	int newBlue = ui->outlineB->text().toInt(&goodVal, 10);
	if (goodVal && newBlue >= 0 && newBlue <= 255)
	{
		QPalette palette = ui->outlineColorFrame->palette();
		outlineColor.setBlue(newBlue);
		palette.setColor(backgroundRole(), outlineColor);
		ui->outlineColorFrame->setPalette(palette);
	} else {
		ui->outlineB->setText(QString::number(ui->outlineColorFrame->palette().color(backgroundRole()).blue()));
	}
}

void Frame_DefaultShader::on_outlineA_editingFinished()
{
	bool goodVal;
	int newAlpha = ui->outlineA->text().toInt(&goodVal, 10);
	if (goodVal && newAlpha >= 0 && newAlpha <= 255)
	{
		QPalette palette = ui->outlineColorFrame->palette();
		outlineColor.setAlpha(newAlpha);
		palette.setColor(backgroundRole(), outlineColor);
		ui->outlineColorFrame->setPalette(palette);
	} else {
		ui->outlineA->setText(QString::number(ui->outlineColorFrame->palette().color(backgroundRole()).alpha()));
	}
}


void Frame_DefaultShader::printColor()
{
	std::cout << "Outline" << std::endl;
	std::cout << "R: " << outlineColor.red() << " - " << ui->outlineColorFrame->palette().color(backgroundRole()).red() << std::endl;
	std::cout << "G: " << outlineColor.green() << " - " << ui->outlineColorFrame->palette().color(backgroundRole()).green() << std::endl;
	std::cout << "B: " << outlineColor.blue() << " - " << ui->outlineColorFrame->palette().color(backgroundRole()).blue() << std::endl;
	std::cout << "A: " << outlineColor.alpha() << " - " << ui->outlineColorFrame->palette().color(backgroundRole()).alpha() << std::endl;
	std::cout << "Fill" << std::endl;

}
