#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "FileSystemPathUtils.h"

#include <iostream>
#include <string>
#include <fstream>

#include "QMimeData"
#include "QMessageBox"
#include <QUrl>
#include <QFileDialog>
#include <QProcess>
#include <QShortcut>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QMetalRoughMaterial>

#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qcameralens.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qpointlight.h>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

using namespace std;

string currentFilePath;
QProcess *process;
bool expertMode = false;

#ifdef Q_OS_LINUX
QString commandToRun = "scripts/run.sh";
#elif defined(Q_OS_WINDOWS)
QString commandToRun = "wsl \"scripts/run.sh\"";
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    QWidget *container = QWidget::createWindowContainer(view);

    ui->viewHorizontalLayout->addWidget(container, 1);

    this->setAcceptDrops(true);

    this->setAcceptDrops(true);
    setlocale(LC_NUMERIC, "C");

    ui->stopButton->setEnabled(false);
    ui->actionTools->setChecked(true);
    ui->actionOutput->setChecked(true);

    this->on_cellTypeCombo_currentIndexChanged(ui->cellTypeCombo->currentIndex());
    this->on_cellSizeCombo_currentIndexChanged(ui->cellSizeCombo->currentIndex());
    this->on_featureCombo_currentIndexChanged(ui->featureCombo->currentIndex());
    this->on_featureInputCombo_currentIndexChanged(ui->featureInputCombo->currentIndex());
    this->on_mesherCombo_currentIndexChanged(ui->mesherCombo->currentIndex());
    this->on_outputCheckBox_stateChanged(ui->outputCheckBox->isChecked());

    ui->dockTools->setTitleBarWidget(new QWidget()); // remove the titlebar

    #ifdef Q_OS_WINDOWS
    ui->textOutput->setFontFamily("Consolas");
    checkWSL();
    #endif

    new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_E), this, SLOT(toggleExpertMode()));
    ui->outputSideCombo->setVisible(false);
    ui->outputSideLabel->setVisible(false);
    ui->outputTypeGroup->setVisible(false);
    ui->filterRadiusLabel->setVisible(false);
    ui->filterRadiusSpinBox->setVisible(false);
    ui->correctionFactorLabel->setVisible(false);
    ui->correctionFactorSpinBox->setVisible(false);

    ui->tabWidget->setTabVisible(3, false);
    ui->dockTools->setMinimumSize(200, 200); // force Qt to recalculate the minimum size constraint

    displayMessage("Welcome to QASLI, the graphical interface of ASLI!", false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent * event)
{}

void MainWindow::dragEnterEvent(QDragEnterEvent* event_)
{
    if (event_->mimeData()->hasFormat("text/uri-list"))
    {
        event_->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* event_)
{
    QList<QUrl> urls = event_->mimeData()->urls();
    if (urls.isEmpty())
        return;

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty())
        return;
    if (fileName.endsWith("stl", Qt::CaseInsensitive))
    {
        std::cout<<"Dropped surface file "<<fileName.toStdString()<<" on mainwindow..."<<std::endl;
        renderFile(fileName);
    }
}

void MainWindow::toggleFullScreen(bool value)
{
    if (value)
    {
        this->showFullScreen();
    }
    else
    {
        this->showNormal();
    }
}

void MainWindow::on_actionLoad_Surface_triggered()
{
    QFileDialog* qfd = new QFileDialog(this, "Select STL File", "", "*.stl");
    QString fileName = qfd->getOpenFileName(this, "Select STL File", "", "*.stl");
    delete qfd;

    if (fileName.toStdString().empty())
        return;

    renderFile(fileName);
}

void MainWindow::on_actionTools_toggled(bool value)
{
    if (value)
        ui->dockTools->show();
    else
        ui->dockTools->hide();
}

void MainWindow::on_actionOutput_toggled(bool value)
{
    if (value)
        ui->dockOutput->show();
    else
        ui->dockOutput->hide();
}

void MainWindow::on_actionAbout_ASLI_triggered()
{
    About *about = new About(this);
    about->setWindowFlags(about->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    about->show();
}

void MainWindow::on_dockTools_visibilityChanged(bool visible)
{
    ui->actionTools->setChecked(visible);
}

void MainWindow::on_dockOutput_visibilityChanged(bool visible)
{
    ui->actionOutput->setChecked(visible);
}

void MainWindow::on_cellTypeCombo_currentIndexChanged(int index)
{
    bool enabled = false;
    if (index == 8)
        enabled = true;
    ui->unitCellLineEdit->setEnabled(enabled);
    ui->browseUnitCellButton->setEnabled(enabled);
    ui->label_2->setEnabled(enabled);

    if (!expertMode)
        enabled = false;
    ui->filterRadiusLabel->setVisible(enabled);
    ui->filterRadiusSpinBox->setVisible(enabled);
    ui->correctionFactorLabel->setVisible(enabled);
    ui->correctionFactorSpinBox->setVisible(enabled);
}

void MainWindow::on_browseUnitCellButton_clicked()
{
    QFileDialog* qfd = new QFileDialog(this, "Select TAP File", "", "*.tap");
    QString s = qfd->getOpenFileName(this, "Select TAP File", "", "*.tap");
    delete qfd;

    if (s.toStdString().empty())
        return;

    ui->unitCellLineEdit->setText(s);
}

void MainWindow::on_cellSizeCombo_currentIndexChanged(int index)
{
    bool enabled = false;
    if (index == 1)
        enabled = true;
    ui->cellSizeSpinBox->setEnabled(!enabled);
    ui->cellSizeLineEdit->setEnabled(enabled);
    ui->browseCellSizeButton->setEnabled(enabled);
    ui->label_3_1->setEnabled(!enabled);
    ui->label_4->setEnabled(enabled);
}

void MainWindow::on_mesherCombo_currentIndexChanged(int index)
{
    if (index == 1)
    {
        ui->groupBoxCGAL->setEnabled(true);
        ui->groupBoxMMG->setEnabled(false);
    }
    else
    {
        ui->groupBoxCGAL->setEnabled(false);
        ui->groupBoxMMG->setEnabled(true);
    }
}

void MainWindow::on_browseCellSizeButton_clicked()
{
    QFileDialog* qfd = new QFileDialog(this, "Select SAP File", "", "*.sap");
    QString s = qfd->getOpenFileName(this, "Select SAP File", "", "*.sap");
    delete qfd;

    if (s.toStdString().empty())
        return;

    ui->cellSizeLineEdit->setText(s);
}

void MainWindow::on_featureCombo_currentIndexChanged(int index)
{
    bool showMode;
    if (index == 0 || index == 1)
    {
        ui->featureSizeLabel->setText("Feature ([0, 1])");
        ui->featureSizeSpinBox->setMaximum(1.0);
        showMode = false;
    }
    else
    {
        ui->featureSizeLabel->setText("Feature ([0, 1000])");
        ui->featureSizeSpinBox->setMaximum(1000.0);
        showMode = true;
    }
    ui->featureModeCombo->setVisible(showMode);
    ui->featureModeLabel->setVisible(showMode);
}

void MainWindow::on_featureInputCombo_currentIndexChanged(int index)
{
    bool enabled = false;
    if (index == 1)
        enabled = true;
    ui->featureSizeSpinBox->setEnabled(!enabled);
    ui->featureSizeLineEdit->setEnabled(enabled);
    ui->browseFeatureButton->setEnabled(enabled);
    ui->label_7->setEnabled(enabled);
    ui->featureSizeLabel->setEnabled(!enabled);
}

void MainWindow::on_browseFeatureButton_clicked()
{
    QFileDialog* qfd = new QFileDialog(this, "Select FAP File", "", "*.fap");
    QString s = qfd->getOpenFileName(this, "Select FAP File", "", "*.fap");
    delete qfd;

    if (s.toStdString().empty())
        return;

    ui->featureSizeLineEdit->setText(s);
}

void MainWindow::on_outputCheckBox_stateChanged(int arg1)
{
    bool enabled = true;
    if (arg1)
        enabled = false;
    ui->label_8->setEnabled(enabled);
    ui->outputLineEdit->setEnabled(enabled);
    ui->outputbrowseButton->setEnabled(enabled);
}

void MainWindow::on_outputbrowseButton_clicked()
{
    QFileDialog* qfd = new QFileDialog(this, "Select Output Directory", "", "");
    qfd->setFileMode(QFileDialog::Directory);
    qfd->setOption(QFileDialog::ShowDirsOnly, true);
    QString s = qfd->getExistingDirectory(this, "Select Output Directory", "");
    delete qfd;

    if (s.toStdString().empty())
        return;

    ui->outputLineEdit->setText(s);
}

void MainWindow::on_runButton_clicked()
{
    if (currentFilePath == "")
    {
        QMessageBox qmb;
        qmb.setText("You should first load a STL file!");
        qmb.exec();
        return;
    }
    writeConfigFile();
    process = new QProcess(this);
    QString program = commandToRun;
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readError()));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
    process->start(program);
    ui->runButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->runButton->setText("Running...");
}

void MainWindow::on_stopButton_clicked()
{
    if (process->state() == QProcess::ProcessState::Running)
        process->kill();
}

void MainWindow::renderFile(QString fileName)
{
    displayMessage("Loading STL...", false);
    currentFilePath = fileName.toUtf8().constData();
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity;
    Qt3DCore::QEntity *viewer = new Qt3DCore::QEntity(rootEntity);
    view->setRootEntity(rootEntity);

    //Qt3DExtras::QMetalRoughMaterial *material = new Qt3DExtras::QMetalRoughMaterial();
    //material->setMetalness(0.361);
    //material->setRoughness(0.3);
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(254, 254, 254));

    QUrl data = QUrl::fromLocalFile(fileName);

    Qt3DRender::QMesh *viewerMesh = new Qt3DRender::QMesh;
    viewerMesh->setMeshName("Mesh");
    viewerMesh->setSource(data);
    viewer->addComponent(viewerMesh);
    viewer->addComponent(material);

    Qt3DRender::QCamera *camera = view->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(45.0f, 45.0f, 45.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(camera);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(0.5f);
    lightEntity->addComponent(light);

    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(60, 0, 40.0f));
    lightEntity->addComponent(lightTransform);

    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(camera);

    displayMessage("STL file loaded!", false);

    qApp->processEvents();
    view->camera()->viewAll();
}

void MainWindow::writeConfigFile()
{
    // Input & output files
    string inputFile = convertPath(currentFilePath);
    string tapFile = convertPath(ui->unitCellLineEdit->text().toUtf8().constData());
    string sapFile = convertPath(ui->cellSizeLineEdit->text().toUtf8().constData());
    string fapFile = convertPath(ui->featureSizeLineEdit->text().toUtf8().constData());
    string outputFile = "outputs/";
    if (!ui->outputCheckBox->isChecked())
        outputFile = convertPath(ui->outputLineEdit->text().toUtf8().constData());

    // Lattice settings
    string latticeType;
    switch (ui->cellTypeCombo->currentIndex())
    {
        case 0:
            latticeType = "gyroid";
            break;
        case 1:
            latticeType = "sheet_gyroid";
            break;
        case 2:
            latticeType = "diamond";
            break;
        case 3:
            latticeType = "sheet_diamond";
            break;
        case 4:
            latticeType = "primitive";
            break;
        case 5:
            latticeType = "sheet_primitive";
            break;
        case 6:
            latticeType = "IWP";
            break;
        case 7:
            latticeType = "sheet_IWP";
            break;
        default:
            latticeType = "hybrid";
            break;
    }
    float latticeRadius = ui->filterRadiusSpinBox->value();
    float latticeCorrectionFactor = ui->correctionFactorSpinBox->value();
    float latticeSize;
    if (ui->cellSizeCombo->currentIndex() == 0)
        latticeSize = ui->cellSizeSpinBox->value();
    else
        latticeSize = 0;

    string latticeFeature;
    switch(ui->featureCombo->currentIndex())
    {
        case 0:
            latticeFeature = "isovalue";
            break;
        case 1:
            latticeFeature = "volumeFraction";
            break;
        case 2:
            latticeFeature = "wallSize";
            break;
        default:
            latticeFeature = "poreSize";
            break;
    }
    float latticeFeatureValue;
    if (ui->featureInputCombo->currentIndex() == 0)
        latticeFeatureValue = ui->featureSizeSpinBox->value();
    else
        latticeFeatureValue = 0;

    string latticeFeatureMode;
    if (ui->featureModeCombo->currentIndex() == 0)
        latticeFeatureMode = "relative";
    else
        latticeFeatureMode = "absolute";

    // Material properties
    string mpModel = "gibson_ashby";
    float mpSolid = 0;
    float mpC = 0;
    float mpn = 0;
    float mpC1 = 0;
    float mpn1 = 0;

    // Mesh settings
    string mesher = ui->mesherCombo->currentText().toUtf8().toUpper().constData();
    string outputType = "scaffold";
    if (ui->outputSideCombo->currentIndex() == 1)
        outputType = "scaffold";
    else if (ui->outputSideCombo->currentIndex() == 2)
        outputType = "void";
    string volumeMesh = "FALSE";
    if (ui->outputVolumeRadio->isChecked())
        volumeMesh = "TRUE";
    int nThreads = 1;

    // Mesh settings (MMG)
    float meshHvol = 0;
    float meshHinitial = ui->initialSizeSpinBox->value();
    float meshHmin = 0;
    float meshHmax = 0;
    float meshHausd = ui->boundaryAprxSpinBox->value();
    float meshHgrad = ui->gradationSpinBox->value();

    // Mesh settings (CGAL)
    float meshFacetAngle = ui->facetAngleSpinBox->value();
    float meshFacetSize = 0;
    float meshFacetDistance = ui->facetDistanceSpinBox->value();
    float meshCellRadiusEdgeRatio = 0;
    float meshCellSize = 0;
    string meshPreserveEdges = "TRUE";
    float meshPoissonOffset = 0;

    // Output settings
    ofstream file;
    file.open("config.yml");

    file << "files:" << endl;
    file << "  stl: " << inputFile << endl;
    file << "  tap: " << tapFile << endl;
    file << "  sap: " << sapFile << endl;
    file << "  fap: " << fapFile << endl;
    file << "  output: " << outputFile << endl;
    file << "lt_type: " << latticeType << endl;
    file << "lt_type_filterRadius: " << latticeRadius << endl;
    file << "lt_type_correctionFactor: " << latticeCorrectionFactor << endl;
    file << "lt_size: " << latticeSize << endl;
    file << "lt_feature: " << latticeFeature << endl;
    file << "lt_feature_val: " << latticeFeatureValue << endl;
    file << "lt_feature_mode: " << latticeFeatureMode << endl;
    file << "mp_materialModel: " << mpModel << endl;
    file << "mp_solid: " << mpSolid << endl;
    file << "mp_C: " << mpC << endl;
    file << "mp_n: " << mpn << endl;
    file << "mp_C1: " << mpC1 << endl;
    file << "mp_n1: " << mpn1 << endl;
    file << "me_mesher: " << mesher << endl;
    file << "me_side: " << outputType << endl;
    file << "me_volumeMesh: " << volumeMesh << endl;
    file << "me_nThreads: " << nThreads << endl;
    file << "me_hvol: " << meshHvol << endl;
    file << "me_hinitial: " << meshHinitial << endl;
    file << "me_hmin: " << meshHmin << endl;
    file << "me_hmax: " << meshHmax << endl;
    file << "me_hausd: " << meshHausd << endl;
    file << "me_hgrad: " << meshHgrad << endl;
    file << "me_facetAngle: " << meshFacetAngle << endl;
    file << "me_facetSize: " << meshFacetSize << endl;
    file << "me_facetDistance: " << meshFacetDistance << endl;
    file << "me_cellRadiusEdgeRatio: " << meshCellRadiusEdgeRatio << endl;
    file << "me_cellSize: " << meshCellSize << endl;
    file << "me_preserveEdges: " << meshPreserveEdges << endl;
    file << "me_poissonOffset: " << meshPoissonOffset << endl;

    file.close();
}

void MainWindow::readOutput()
{
    displayMessage(process->readAllStandardOutput(), false);
}

void MainWindow::readError()
{
    displayMessage(process->readAllStandardError(), true);
}

void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QMessageBox qmb;
//    qmb.setText("Process finished with exit code " + QString::number(exitCode));
    if (exitStatus == QProcess::ExitStatus::NormalExit)
        if (exitCode == 0)
            qmb.setText("Process finished successfully!");
        else
            qmb.setText("Process finished with error!");
    else
        qmb.setText("Process interrupted!");
    qmb.exec();
    ui->runButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->runButton->setText("Run");
}

void MainWindow::checkWSL()
{
    bool wslInstalled = false;
    QProcess* wslProcess = new QProcess(this);
    wslProcess->start("bash --version");
    if (!wslProcess->waitForFinished(-1))
        qWarning() << "Error:" << wslProcess->errorString();
    else if (wslProcess->exitCode() == 0)
        wslInstalled = true;

    if (!wslInstalled)
    {
        QMessageBox qmb;
        qmb.setText("This program needs WSL (Windows Subsystem for Linux) to be installed!\nYou can still explore around, but you cannot run the meshing process.\nPlease refer to the documentation for further information.");
        qmb.setIcon(QMessageBox::Critical);
        qmb.exec();
    }
}

void MainWindow::toggleExpertMode()
{
    expertMode = !expertMode;

    ui->outputSideCombo->setVisible(expertMode);
    ui->outputSideLabel->setVisible(expertMode);
    ui->outputTypeGroup->setVisible(expertMode);

    on_cellTypeCombo_currentIndexChanged(ui->cellTypeCombo->currentIndex());

    QMessageBox qmb;
    if (expertMode)
        qmb.setText("Expert Mode enabled.");
    else
        qmb.setText("Expert Mode disabled.");
    qmb.setIcon(QMessageBox::Information);
    qmb.exec();
}

void MainWindow::displayMessage(QString msg, bool isError)
{
    if (isError)
        ui->textOutput->setTextColor(QColor("red"));
    else
        ui->textOutput->setTextColor(QColor("black"));
    ui->textOutput->append(msg);
}
