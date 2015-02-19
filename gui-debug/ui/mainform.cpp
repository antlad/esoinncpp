#include "mainform.h"
#include "ui_mainform.h"

#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>

#include "data/ocidataset.h"

#define VAL_C1 "C1"
#define VAL_C2 "C2"
#define VAL_MAXAGE "MAXAGE"
#define VAL_ITERATION "Iteration"
#define VAL_DATAFOLDER "DataFolder"

MainForm::MainForm(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MainForm)
{
	ui->setupUi(this);

	QSettings settings;

	ui->c1LineEdit->setText(settings.value(VAL_C1).toString());
	ui->c2LineEdit->setText(settings.value(VAL_C2).toString());
	ui->maxAgeLineEdit->setText(settings.value(VAL_MAXAGE).toString());
	ui->iterationLineEdit->setText(settings.value(VAL_ITERATION).toString());
	ui->dataFolderLineEdit->setText(settings.value(VAL_DATAFOLDER).toString());

	m_proxyModel.setSourceModel(&m_model);
	ui->tableView->setModel(&m_proxyModel);
}

MainForm::~MainForm()
{
	QSettings settings;
	settings.setValue(VAL_C1, ui->c1LineEdit->text());
	settings.setValue(VAL_C2, ui->c2LineEdit->text());

	settings.setValue(VAL_MAXAGE, ui->maxAgeLineEdit->text());
	settings.setValue(VAL_ITERATION, ui->iterationLineEdit->text());
	settings.setValue(VAL_DATAFOLDER, ui->dataFolderLineEdit->text());
	delete ui;
}

void MainForm::on_nextStepPushBtn_clicked()
{

	std::vector<float> data;
	int digit;
	bool hasNext = true;
	bool wasClassificationStep = false;
	while(hasNext && !wasClassificationStep)
	{
		hasNext = m_ds->getNextDataNormalized(data, digit);
		m_esoinn->learnNextInput(data, digit);
		wasClassificationStep = m_esoinn->wasClassificationStep();
	}
	if (!hasNext)
	{
		m_esoinn->classificate();
		QMessageBox::information(this, "", "finished");
	}

	m_model.updateModel(*m_esoinn.get());
}

void MainForm::on_loadPushButton_clicked()
{
	try
	{
		m_esoinn = std::make_shared<ESOINN>(8 * 8, ui->c1LineEdit->text().toFloat(),
											ui->c2LineEdit->text().toFloat(),
											ui->maxAgeLineEdit->text().toInt(),
											ui->iterationLineEdit->text().toInt());

		QString dataPathString = ui->dataFolderLineEdit->text();

		QDir dataDir(dataPathString);

		if (!dataDir.exists())
			throw std::runtime_error("invalid folder");

		QString trainPath = dataDir.absolutePath() + QDir::separator() + "optdigits.tra";

		if (!QFile::exists(trainPath))
			std::runtime_error("oci train does not exist");

		m_ds = std::make_shared<OCIDataSet>(trainPath.toUtf8().toStdString());

	}
	catch(const std::exception & e)
	{
		QMessageBox::warning(this, "", e.what());
	}
}

void MainForm::on_toEndPushButton_clicked()
{
	std::vector<float> data;
	int digit;
	while(m_ds->getNextDataNormalized(data, digit))
	{
		m_esoinn->learnNextInput(data, digit);
	}
	m_esoinn->classificate();
	QMessageBox::information(this, "", "finished");
	m_model.updateModel(*m_esoinn.get());
}
