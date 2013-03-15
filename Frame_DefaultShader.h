#ifndef FRAME_DEFAULTSHADER_H
#define FRAME_DEFAULTSHADER_H

#include <QFrame>
#include <QColorDialog>
#include <iostream>

namespace Ui {
	class Frame_DefaultShader;
}

class Frame_DefaultShader : public QFrame
{
		Q_OBJECT
		
	public:
		explicit Frame_DefaultShader(QWidget *parent = 0);
		~Frame_DefaultShader();

		// Getter functions
		QColor getColor();

		
	private slots:
		void on_outlineSelectButton_clicked();
		void on_outlineR_editingFinished();
		void on_outlineG_editingFinished();
		void on_outlineB_editingFinished();
		void on_outlineA_editingFinished();

	private:
		Ui::Frame_DefaultShader *ui;

		QColor outlineColor;

		void printColor();
};

#endif // FRAME_DEFAULTSHADER_H
