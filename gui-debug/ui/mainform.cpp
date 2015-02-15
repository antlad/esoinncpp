#include "mainform.h"
#include "ui_mainform.h"

#include <QDir>
#include <QMessageBox>

#include "data/ocidataset.h"

MainForm::MainForm(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MainForm)
{
	ui->setupUi(this);

	m_proxyModel.setSourceModel(&m_model);

	ui->tableView->setModel(&m_proxyModel);
}

MainForm::~MainForm()
{
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


		//	if (!bf::exists(dataPathString))
		//		std::runtime_error((boost::format("Path %s doesn't exists") % dataPathString).str());

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
