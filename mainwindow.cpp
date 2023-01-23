#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include<QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include<QMessageBox>
#include<QFile>

#include<windows.h>
#include<QLineEdit>
#include<fstream>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    #ifdef _WIN32
    for (const auto& file : fs::directory_iterator("./dlls")) {
        if (!fs::is_directory(file)) {
            if (fs::path(file).extension() == ".dll")
            {
                fs::path p = fs::path(file);
                openDll(p);
            }
        }
    }
    for (auto iter = dynLibsList.begin(); iter != dynLibsList.end(); iter++) {

        std::string filename = (*iter).second.filename().string(), substring = ".dll";
        std::size_t ptr = filename.find(substring);

        if (ptr != std::string::npos) {
            filename.erase(ptr, substring.length());
        }
    }
#else

    for (const auto& file : fs::directory_iterator("./dlls")) {
        if (!fs::is_directory(file)) {

            if (fs::path(file).extension() == ".so")
            {
                fs::path p = fs::path(file);
                openDll(p);
            }
        }
    }

    for (auto iter = dynLibsList.begin(); iter != dynLibsList.end(); iter++) {

        std::string filename = (*iter).second.filename().string(), substring = ".so", substring1 = "lib";

        std::size_t ptr = filename.find(substring1);
        if (ptr != std::string::npos) {
            filename.erase(ptr, substring1.length());
        }

        ptr = filename.find(substring);
        if (ptr != std::string::npos) {
            filename.erase(ptr, substring.length());
        }

        ui->ShopsList->addItem(QString::fromStdString(filename));
    }

#endif  
    
    ui->setupUi(this);
    sForm = new Form();
    connect(sForm, &Form::firstForm, this, &MainWindow::show);
    manager = new QNetworkAccessManager();

    connect(this, SIGNAL(sendTitle(json)), sForm, SLOT(getTitle(json)));

    connect(this, SIGNAL(sendPost(std::string, std::string, std::string, int)), sForm, SLOT(createGaleleryPost(std::string, std::string, std::string, int)));

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(ReplyFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    closeDll();
    deletePhotos();

    delete ui;
}

void MainWindow::deletePhotos(){
    for (const auto& file : fs::directory_iterator("photo")) {
        if (!fs::is_directory(file)) {
            if (fs::path(file).extension() == ".jpg")
            {
                fs::path p = fs::path(file);
                remove(p);
            }
        }
    }
}

void MainWindow::openDll(fs::path path) {
    #ifdef _WIN32
    HINSTANCE load;
    load = LoadLibrary(path.string().c_str());
    MainWindow::insertDllIntoList(load, path);
#else
    void* load;
    load = dlopen(path.c_str(), RTLD_LAZY);
    insertDllIntoList(load, path);
#endif
}

void MainWindow::closeDll() {
#ifdef _WIN32
    for (auto iter = dynLibsList.begin(); iter != dynLibsList.end(); iter++) {
        FreeLibrary((*iter).first);
    }

#else
    for (auto iter = dynLibsList.begin(); iter != dynLibsList.end(); iter++) {
        dlclose((*iter).first);
    }
#endif
}

void MainWindow::insertDllIntoList(const HINSTANCE& mem, const fs::path& filepath) {
    std::pair elem = std::make_pair(mem, filepath);
    dynLibsList.push_back(elem);
}

void MainWindow::ReplyFinished(QNetworkReply* reply) {
    response = json::parse(reply->readAll());
    response = response["response"]["items"];
}

void MainWindow::on_DownloadButton_clicked()
{
    for (auto iterator = dynLibsList.begin(); iterator != dynLibsList.end(); iterator++) {
        if ((((*iterator).second.filename().string().find(ui->line_social_net->text().toStdString()) != std::string::npos))) {
            if (!ui->line_social_net->text().toStdString().empty()) {
                if (!ui->line_login->text().toStdString().empty()) {
                    std::ifstream login_file("login.txt");
                    std::string login = ui->line_login->text().toStdString();
                    std::string line;
                    std::string correct_token;
                    while (getline(login_file, line)) {
                        if (line.find(login + "::")!=std::string::npos) {
                            correct_token= line.substr(login.size() + 2, line.size() - 1);
                        }
                    }
                    if (!correct_token.empty()) {

                        typedef std::string(*get_url) (std::string, std::string, std::string);
                        get_url create_url;
                        create_url = (get_url)*GetProcAddress((*iterator).first, "create_url");

                        std::string url = create_url(ui->line_domen->text().toStdString(), ui->line_count->text().toStdString(), correct_token);

                        request.setUrl(QUrl(QString::fromStdString(url)));
                        QNetworkReply* reply = manager->get(request);
                        QEventLoop loop;
                        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
                        loop.exec();

                        typedef void(*get_inv_url) (const json&, json&, std::string);
                        get_inv_url get_investment_url;
                        get_investment_url = (get_inv_url)*GetProcAddress((*iterator).first, "get_investment_url");

                        for (int i = 0; i < response.size(); i++) {
                            get_investment_url(response[i], ans_response, "");
                        }

                        emit sendTitle(ans_response);

                        typedef std::vector<std::string>(*post_inf) (json&);
                        post_inf get_post;
                        get_post = (post_inf)*GetProcAddress((*iterator).first, "get_inf_post");

                        for (int i = 0; i < response.size(); i++) {
                            std::vector<std::string> vec;
                            vec = get_post(response[i]);
                            std::string post_hash = vec[0];
                            std::string post_text = vec[1];
                            std::string repost_text = vec[2];
                            emit sendPost(post_hash, post_text, repost_text, i);
                        }
                        ans_response = {};
                        sForm->show();
                        this->close();
                    }
                    else {
                        QMessageBox::information(this, QString::fromLocal8Bit("Ошибка!"), QString::fromLocal8Bit("Логин не найден!"));
                    }
                }
                else {
                    QMessageBox::information(this, QString::fromLocal8Bit("Ошибка!"), QString::fromLocal8Bit("Введите логин!"));
                }
            }
            else {
                QMessageBox::information(this, QString::fromLocal8Bit("Ошибка!"), QString::fromLocal8Bit("Поле мессанджера не может быть пустым!"));
            }
        }
        else {
            QMessageBox::information(this, QString::fromLocal8Bit("Ошибка!"), QString::fromLocal8Bit("Введите доступный мессенджер!"));
        }
    }
}



