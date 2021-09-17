#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    QString buildDate = __DATE__;
    QString buildTime = __TIME__;
    ui->buildTimeLabel->setText(QString("Built on %1 %2").arg(buildDate).arg(buildTime));

    ui->qtVersionLabel->setText(QString("Based on Qt %1").arg(QT_VERSION_STR));

    this->setFixedSize(this->size());
}

About::~About()
{
    delete ui;
}
