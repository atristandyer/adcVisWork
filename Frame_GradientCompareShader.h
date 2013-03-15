#ifndef FRAME_GRADIENTCOMPARESHADER_H
#define FRAME_GRADIENTCOMPARESHADER_H

#include <QFrame>
#include <QLineEdit>
#include <QColorDialog>

#define RED 0
#define GREEN 1
#define BLUE 2
#define ALPHA 3

namespace Ui {
	class Frame_GradientCompareShader;
}

class Frame_GradientCompareShader : public QFrame
{
		Q_OBJECT
		
	public:
		explicit Frame_GradientCompareShader(QWidget *parent = 0);
		~Frame_GradientCompareShader();

		// Getter Methods
		QColor	getHighColor();
		QColor	getLowColor();
		QColor	getWDColor();
		bool	getCustomRangeEnabled();
		float	getHighValue();
		float	getLowValue();

	private slots:

		// Text Edit Slots
		void on_highR_editingFinished();
		void on_highG_editingFinished();
		void on_highB_editingFinished();
		void on_highA_editingFinished();
		void on_lowR_editingFinished();
		void on_lowG_editingFinished();
		void on_lowB_editingFinished();
		void on_lowA_editingFinished();
		void on_wdR_editingFinished();
		void on_wdG_editingFinished();
		void on_wdB_editingFinished();
		void on_wdA_editingFinished();

		// Button Click Slots
		void on_highSelectButton_clicked();
		void on_lowSelectButton_clicked();
		void on_wdSelectButton_clicked();

		// Toggle Slot
		void on_useCustomRadio_toggled(bool checked);

	private:
		Ui::Frame_GradientCompareShader *ui;

		// Colors
		QColor	highColor;
		QColor	lowColor;
		QColor	wdColor;

		// Private Functions
		void	UpdateGradient();
		void	SetColorFromText(QFrame *frame, QLineEdit *line, QColor *color, int colorID);
		void	UpdateTextValues();
};

#endif // FRAME_GRADIENTCOMPARESHADER_H
