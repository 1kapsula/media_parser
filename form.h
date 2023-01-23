#ifndef FORM_H
#define FORM_H

#include<QPushButton>
#include<QScrollArea>
#include<QHBoxLayout>
#include<QBoxLayout>
#include <QPainter>

#include<QListWidget>

#include <QWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QApplication>
#include<string>
#include<QObject>
#include<QtNetwork/QNetworkRequest>
#include<QByteArray>
#include<QLineEdit>
#include<vector>
#include<QLabel>
#include<QPixmap>

#include <QGraphicsPixmapItem>

#include<QtNetwork>

#include<QMessageBox>

#include<nlohmann/json.hpp>

using json = nlohmann::json;

namespace Ui {
    class Form;
}
class Form : public QWidget
{
    Q_OBJECT
public:
    explicit Form(QWidget* parent = 0);
    ~Form();
    json ans;
signals:
    void firstForm();
public slots:
    void createGaleleryPost(std::string hash_post, std::string text_post, std::string text_repost,int i);
    void getTitle(json title);
private slots:
    void on_back_clicked();
private:
    Ui::Form* ui;
    QNetworkAccessManager* m_manager;
    QNetworkRequest f_request;

    std::vector<std::string> path_photos;
};
#endif // FORM_H