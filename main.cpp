#include "mainwindow.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include<QtNetwork/QtNetwork>
#include <QApplication>
#include<QLabel>
#include<QUrl>

int main(int argc, char *argv[])
{
    auto* app = new QApplication(argc, argv);
    MainWindow m;
    m.show();

    return (*app).exec();
}
