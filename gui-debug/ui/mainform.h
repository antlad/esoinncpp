#ifndef MAINFORM_H
#define MAINFORM_H

#include <QDialog>

#include <memory>
#include <esoinn.h>

namespace Ui {
class MainForm;
}

class MainForm : public QDialog
{
	Q_OBJECT

public:
	explicit MainForm(QWidget *parent = 0);
	~MainForm();

private slots:
	void on_nextStepPushBtn_clicked();

	void on_loadPushButton_clicked();

private:
	Ui::MainForm *ui;

	std::shared_ptr<ESOINN> m_esoinn;
};

#endif // MAINFORM_H
