#ifndef MAINFORM_H
#define MAINFORM_H

#include <QDialog>
#include <QSortFilterProxyModel>

#include <memory>
#include <esoinn.h>

#include "data/nodesmodel.h"

namespace Ui {
class MainForm;
}

class OCIDataSet;

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
	QSortFilterProxyModel m_proxyModel;


	NodesModel m_model;
	std::shared_ptr<ESOINN> m_esoinn;
	std::shared_ptr<OCIDataSet> m_ds;
};

#endif // MAINFORM_H
