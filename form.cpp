#include "form.h"
#include "ui_form.h"

#include<mainwindow.h>

#include<QMessageBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include<QtNetwork/QtNetwork>
#include<QApplication>
#include<QFile>
#include<QtNetwork>

#include<fstream>

#include<QDebug>
#include<QRect>
#include<QLabel>
#include<QUrl>

#include<nlohmann/json.hpp>


Form::Form(QWidget* parent)
    :QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
    m_manager = new QNetworkAccessManager();
}

Form::~Form()
{
    delete ui;
    delete m_manager;
}

bool fileExists(QString path) {
    QFileInfo check_file(path);
    if (check_file.exists() && check_file.isFile()) {
        return true;
    }
    else {
        return false;
    }
}

void Form::on_back_clicked()
{
    this->close();
    /*if (path_photos.size() > 100) {
        for (int i = path_photos.size() - 1; i > 30; i--) {
            remove(path_photos[i].c_str());
            path_photos.erase(path_photos.end() - 1);
        }
    }*/ 
    while (ui->listWidget->count() > 0)
    {
        ui->listWidget->clear();
    }
    emit firstForm();
}

void Form::getTitle(json title) {
    ans = title;
    QString photo_name;
    for (auto& post : ans.items()) {
        photo_name = QString::fromStdString(post.key().substr(1, post.key().size() - 2));
        for (int j = 0; j < post.value().size(); j++) {
            photo_name += QString::number(j);
            if (fileExists("photo/" + photo_name + ".jpg")) {
                path_photos.push_back("photo/" + photo_name.toStdString() + ".jpg");
                continue;
            }
            else {
                f_request.setUrl(QUrl(QString::fromStdString(post.value()[j])));
                QNetworkReply* f_reply = m_manager->get(f_request);
                QEventLoop loop;
                connect(f_reply, SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();
                QFile qFile("photo/" + photo_name + ".jpg");
                if (qFile.open(QIODevice::WriteOnly)) {
                    qFile.write(f_reply->readAll());
                    qFile.close();
                }
                path_photos.push_back("photo/" + photo_name.toStdString() + ".jpg");
            }
        }
    }
    photo_name.clear();
}

void Form::createGaleleryPost(std::string hash_post, std::string text_post, std::string text_repost,int i) {
    std::string str_text = text_post;
    if (!text_repost.empty()) {
        str_text += "\nЗапись репоста\n" + text_repost;
    }
    QListWidgetItem* icon_text = new QListWidgetItem(QString::fromStdString(str_text), ui->listWidget, 0);
    icon_text->setBackgroundColor(Qt::gray);
    ui->listWidget->addItem(icon_text);
    for (const auto& file : fs::directory_iterator("photo")) {
        if (!fs::is_directory(file)) {
            if (fs::path(file).extension() == ".jpg")
            {
                fs::path p = fs::path(file);
                if (p.relative_path().generic_string().find(hash_post) != std::string::npos){
                    ui->listWidget->setIconSize(QSize(600, 800));              
                    QIcon icon(QPixmap(p.relative_path().generic_string().c_str()));

                    QListWidgetItem* item = new QListWidgetItem(icon,"", ui->listWidget, 0);
                    ui->listWidget->addItem(item);
                }
            }
        }
    }
}