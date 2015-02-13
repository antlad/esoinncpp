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
	ui->tableView->setModel(&m_model);
}

MainForm::~MainForm()
{
	delete ui;
}

void MainForm::on_nextStepPushBtn_clicked()
{
//	if (argc != 2)
//	{
//		std::cerr << "Invalid args. Set first args as oci dataset data path\n";
//		return 1;
//	}
//	std::string dataPathString(argv[1]);
//	if (!bf::exists(dataPathString))
//		std::runtime_error((boost::format("Path %s doesn't exists") % dataPathString).str());

//	bf::path dataPath(dataPathString);

//	ESOINN esoinn(8 * 8, 0.001f, 1.0f, 50, 200);
//	bf::path savePath = dataPath / "osi_essoinn.dat";
//	if (bf::exists(savePath))
//	{
//		esoinn.loadFromPath(savePath.string());
//	}

//	bf::path ociTrainPath = dataPath / "optdigits.tra";
//	if (!bf::exists(ociTrainPath))
//		std::runtime_error("oci train does not exist");

//	OCIDataSet ds(ociTrainPath.string());

//	std::vector<float> data;
//	int digit;

//	while(ds.getNextDataNormalized(data, digit))
//	{
//		esoinn.learnNextInput(data, true);
//	}
//	esoinn.classificate();
//	esoinn.saveApexesToFolder(dataPathString, 8, 8);


//	bf::path ociTestPath = dataPath / "optdigits.tes";
//	if (!bf::exists(ociTestPath))
//		std::runtime_error("oci test does not exist");

//	OCIDataSet ds_test(ociTestPath.string());

//	int test_count = 0;
//	int success_count = 0;
//	while(ds_test.getNextDataNormalized(data, digit))
//	{
//		int32_t out_digit = esoinn.calcInput(data, false);
//		if (out_digit == digit)
//			++success_count;
//		++test_count;
//	}
//	std::cout << "success rait " << double(success_count) / double(test_count) * 100 << "\n";

	std::vector<float> data;
	int digit;
	if (m_ds->getNextDataNormalized(data, digit))
	{
		m_esoinn->learnNextInput(data, true);
	}
}

void MainForm::on_loadPushButton_clicked()
{
	try
	{
		m_esoinn = std::make_shared<ESOINN>(8 * 8, 0.001f, 1.0f, 50, 200);
		QString dataPathString = "/home/antlad/111/";


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
