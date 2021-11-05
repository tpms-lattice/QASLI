#include "global.h"
#include "about.h"
#include "ui_about.h"
#include <QProcess>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    QString buildDate = __DATE__;
    QString buildTime = __TIME__;
    ui->buildTimeLabel->setText(QString("Built on %1 %2").arg(buildDate).arg(buildTime));

    ui->qtVersionLabel->setText(QString("Based on Qt %1").arg(QT_VERSION_STR));

    ui->asliVersionLabel->setText("Error reading ASLI version");
    QProcess process;
    QString dir = WORKING_DIR;
    QString program = COMMAND_TO_RUN;
    program = dir + program;
    process.setProgram(program);
    process.setArguments(QStringList() << "-version");
    process.setWorkingDirectory(dir);
    process.start();
    process.waitForFinished();
    QString output(process.readAllStandardOutput());
    for (QString line : output.split("\n"))
        if (!line.isEmpty())
            if (line.toLower().startsWith("asli v"))
                ui->asliVersionLabel->setText(line);


//    this->setFixedSize(this->size());
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

About::~About()
{
    delete ui;
}
