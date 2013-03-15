#ifndef DIALOG_NORMALMESH_H
#define DIALOG_NORMALMESH_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
	class Dialog_NormalMesh;
}

class Dialog_NormalMesh : public QDialog
{
		Q_OBJECT
		
	public:
		explicit Dialog_NormalMesh(QWidget *parent = 0);
		~Dialog_NormalMesh();

		// Setter functions
		void	setDefaultName(QString name);

		// Getter functions
		QString getName();
		QString getFort14();
		QString getFort63();
		
	private slots:
		void on_chooseFort14Button_clicked();
		void on_chooseFort63Button_clicked();

	private:
		Ui::Dialog_NormalMesh *ui;

		QString fort14Location;
		QString fort63Location;
};

#endif // DIALOG_NORMALMESH_H
