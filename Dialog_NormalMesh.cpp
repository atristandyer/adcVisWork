#include "Dialog_NormalMesh.h"
#include "ui_Dialog_NormalMesh.h"

Dialog_NormalMesh::Dialog_NormalMesh(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Dialog_NormalMesh)
{
	ui->setupUi(this);
	setWindowTitle("Open Normal Mesh");

	fort14Location = "";
	fort63Location = "";
}

Dialog_NormalMesh::~Dialog_NormalMesh()
{
	delete ui;
}


void Dialog_NormalMesh::setDefaultName(QString name)
{
	ui->nameLineEdit->setText(name);
}


QString Dialog_NormalMesh::getName()
{
	return ui->nameLineEdit->text();
}


QString Dialog_NormalMesh::getFort14()
{
	return fort14Location;
}


QString Dialog_NormalMesh::getFort63()
{
	return fort63Location;
}

void Dialog_NormalMesh::on_chooseFort14Button_clicked()
{
	fort14Location = QFileDialog::getOpenFileName(this, tr("Choose fort.14 File"));
	ui->fort14LineEdit->setText(fort14Location);
}

void Dialog_NormalMesh::on_chooseFort63Button_clicked()
{
	fort63Location = QFileDialog::getOpenFileName(this, tr("Choose fort.63 File"));
	ui->fort63LineEdit->setText(fort63Location);
}
