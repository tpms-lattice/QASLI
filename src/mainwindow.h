#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <Qt3DExtras/qt3dwindow.h>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void toggleFullScreen(bool value);
    void displayMessage(QString msg, bool isError);

private:
    Ui::MainWindow *ui;
    Qt3DExtras::Qt3DWindow *view;
    void renderFile(QString fileName);
    void writeConfigFile();

protected:
    // override main window close event to close all child dialogs
    void closeEvent(QCloseEvent * event);
    // drag and drop of mesh files on the main window
    void dragEnterEvent(QDragEnterEvent* event_);
    void dropEvent(QDropEvent* event_);

private slots:
    void on_cellTypeCombo_currentIndexChanged(int index);
    void on_browseUnitCellButton_clicked();
    void on_cellSizeCombo_currentIndexChanged(int index);
    void on_browseCellSizeButton_clicked();
    void on_actionLoad_Surface_triggered();
    void on_runButton_clicked();
    void on_featureInputCombo_currentIndexChanged(int index);
    void on_mesherCombo_currentIndexChanged(int index);
    void on_outputCheckBox_stateChanged(int arg1);
    void on_browseFeatureButton_clicked();
    void on_outputbrowseButton_clicked();
    void readOutput();
    void readError();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void on_stopButton_clicked();
    void on_featureCombo_currentIndexChanged(int index);
    void on_actionTools_toggled(bool value);
    void on_actionOutput_toggled(bool value);
    void on_dockOutput_visibilityChanged(bool visible);
    void on_dockTools_visibilityChanged(bool visible);
    void checkWSL();
    void toggleExpertMode();
    void on_actionAbout_ASLI_triggered();
};

#endif // MAINWINDOW_H
