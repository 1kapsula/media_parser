#include "form.h"
#include "ui_form.h"
#include<mainwindow.h>


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
    while (ui->listWidget->count() > 0)
    {
        ui->listWidget->clear();
    }
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
            if (fileExists("buffer/" + photo_name + ".jpg")) {
                continue;
            }
            else {
                f_request.setUrl(QUrl(QString::fromStdString(post.value()[j])));
                QNetworkReply* f_reply = m_manager->get(f_request);
                QEventLoop loop;
                connect(f_reply, SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();
                QFile qFile("buffer/" + photo_name + ".jpg");
                if (qFile.open(QIODevice::WriteOnly)) {
                    qFile.write(f_reply->readAll());
                    qFile.close();
                }
            }
        }
    }
    photo_name.clear();
}

void Form::createGaleleryPost(std::string hash_post, std::string text_post, std::string text_repost) {
    QFont font_separ;
    font_separ.setPointSize(14);
    
    std::string start_record = "Начало публикации";

    QListWidgetItem* text_separ = new QListWidgetItem(QString::fromStdString(start_record), ui->listWidget, 0);
    
    text_separ->setFont(font_separ);

    text_separ->setBackground(Qt::green);

    ui->listWidget->addItem(text_separ);

    std::string str_text = text_post;
    if (text_post.empty()) {
        str_text += "Данный пост не содержит собственного текста!";
    }

    if (!text_repost.empty()) {
        str_text += "\nДанный пост содержит текст репоста:\n" + text_repost;
    }
    QListWidgetItem* icon_text = new QListWidgetItem(QString::fromStdString(str_text), ui->listWidget, 0);

    ui->listWidget->addItem(icon_text);
    for (const auto& file : fs::directory_iterator("buffer")) {
        if (!fs::is_directory(file)) {
            if (fs::path(file).extension() == ".jpg")
            {
                fs::path p = fs::path(file);
                if (p.relative_path().generic_string().find(hash_post) != std::string::npos) {
                    ui->listWidget->setIconSize(QSize(600, 800));
                    QIcon icon(QPixmap(p.relative_path().generic_string().c_str()));

                    QListWidgetItem* item = new QListWidgetItem(icon, "", ui->listWidget, 100);

                    ui->listWidget->addItem(item);

                }
            }
        }
    }
    std::string end_record = "Конец публикации";
    QListWidgetItem* end_separ = new QListWidgetItem(QString::fromStdString(end_record), ui->listWidget, 0);
    
    end_separ->setFont(font_separ);

    end_separ->setBackground(Qt::red);

    ui->listWidget->addItem(end_separ);

}