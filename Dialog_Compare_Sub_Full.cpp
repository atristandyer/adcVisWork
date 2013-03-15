#include "Dialog_Compare_Sub_Full.h"
#include "ui_Dialog_Compare_Sub_Full.h"

Dialog_Compare_Sub_Full::Dialog_Compare_Sub_Full(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog_Compare_Sub_Full)
{
	ui->setupUi(this);

	setWindowTitle("Compare Sub/Full Domain");

	subFort14Location = "";
	fullFort14Location = "";
	subFort63Location = "";
	fullFort63Location = "";
}

Dialog_Compare_Sub_Full::~Dialog_Compare_Sub_Full()
{
	delete ui;
}

void Dialog_Compare_Sub_Full::setDefaultName(QString name)
{
	ui->nameLineEdit_2->setText(name);
}

QString Dialog_Compare_Sub_Full::getName()
{
	return ui->nameLineEdit_2->text();
}

QString Dialog_Compare_Sub_Full::getSubFort14()
{
	return ui->fort14Sub->text();
}

QString Dialog_Compare_Sub_Full::getFullFort14()
{
	return ui->fort14Full->text();
}

QString Dialog_Compare_Sub_Full::getSubFort63()
{
	return ui->fort63Sub->text();
}

QString Dialog_Compare_Sub_Full::getFullFort63()
{
	return ui->fort63Full->text();
}

QString Dialog_Compare_Sub_Full::getPy140()
{
	return ui->py140->text();
}

void Dialog_Compare_Sub_Full::on_chooseFort14Full_clicked()
{
	fullFort14Location = QFileDialog::getOpenFileName(this, tr("Choose Full Domain fort.14 File"));
	ui->fort14Full->setText(fullFort14Location);
}

void Dialog_Compare_Sub_Full::on_chooseFort63Full_clicked()
{
	fullFort63Location = QFileDialog::getOpenFileName(this, tr("Choose Full Domain fort.63 File"));
	ui->fort63Full->setText(fullFort63Location);
}

void Dialog_Compare_Sub_Full::on_chooseFort14Sub_clicked()
{
	subFort14Location = QFileDialog::getOpenFileName(this, tr("Choose Sub Domain fort.14 File"));
	ui->fort14Sub->setText(subFort14Location);
}

void Dialog_Compare_Sub_Full::on_chooseFort63Sub_clicked()
{
	subFort63Location = QFileDialog::getOpenFileName(this, tr("Choose Sub Domain fort.63 File"));
	ui->fort63Sub->setText(subFort63Location);
}

void Dialog_Compare_Sub_Full::on_choosePy140_clicked()
{
	py140Location = QFileDialog::getOpenFileName(this, tr("Choose py.140 File"));
	ui->py140->setText(py140Location);
}
