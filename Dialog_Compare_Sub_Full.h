#ifndef DIALOG_COMPARE_SUB_FULL_H
#define DIALOG_COMPARE_SUB_FULL_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
	class Dialog_Compare_Sub_Full;
}

class Dialog_Compare_Sub_Full : public QDialog
{
		Q_OBJECT
		
	public:
		explicit Dialog_Compare_Sub_Full(QWidget *parent = 0);
		~Dialog_Compare_Sub_Full();

		// Setter functions
		void	setDefaultName(QString name);

		// Getter functions
		QString getName();
		QString getSubFort14();
		QString getFullFort14();
		QString getSubFort63();
		QString getFullFort63();
		QString getPy140();
		
	private slots:
		void on_chooseFort14Full_clicked();
		void on_chooseFort63Full_clicked();
		void on_chooseFort14Sub_clicked();
		void on_chooseFort63Sub_clicked();
		void on_choosePy140_clicked();

	private:
		Ui::Dialog_Compare_Sub_Full *ui;

		QString subFort14Location;
		QString fullFort14Location;
		QString subFort63Location;
		QString fullFort63Location;
		QString py140Location;
};

#endif // DIALOG_COMPARE_SUB_FULL_H
