#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
        chooseParser = new QComboBox(this);
        chooseParser->addItem(QString::fromStdString(filename));
        chooseParser->setGeometry(275,175,285,30);
        QFont font;
        font.setPointSize(font.pointSize() + 2);
        chooseParser->setFont(font);
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

    connect(this, SIGNAL(sendTitle(json)),
        sForm, SLOT(getTitle(json)));

    connect(this, SIGNAL(sendPost(std::string, std::string, std::string)),
        sForm, SLOT(createGaleleryPost(std::string, std::string, std::string)));

    connect(manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(ReplyFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    closeDll();
    deletePhotos();
    delete chooseParser;
    delete ui;
    
}

void MainWindow::deletePhotos() {
    for (const auto& file : fs::directory_iterator("buffer")) {
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
    full_response = json::parse(reply->readAll());
    full_response = full_response["response"]["items"];
}

void MainWindow::on_DownloadButton_clicked()
{
    for (auto iterator = dynLibsList.begin(); iterator != dynLibsList.end(); iterator++) {
        if ((((*iterator).second.filename().string().find(chooseParser->currentData().toString().toStdString()) != std::string::npos))) {
            if (!ui->line_domen->text().toStdString().empty()) {
                if(ui->line_login->text().toStdString()=="vk-test"){
                    for (const auto& file : fs::directory_iterator("Test")) {
                        if (!fs::is_directory(file)) {
                            if (fs::path(file).extension() == ".json"){
                                if (fs::path(file).relative_path().generic_string().find(ui->line_domen->text().toStdString()) != std::string::npos) {// не видит файл
                                    std::ifstream test_file(fs::path(file).c_str());
                                    full_response = json::parse(test_file);
                                    full_response = full_response["response"]["items"];
                                }
                            }
                        }
                    }
                }
                
                if (!ui->line_login->text().toStdString().empty()) {
                    std::ifstream login_file("buffer/login.txt");
                    std::string login = ui->line_login->text().toStdString();
                    std::string line;
                    std::string correct_token;
                    while (getline(login_file, line)) {
                        if (line.find(login + "::") != std::string::npos) {
                            correct_token = line.substr(login.size() + 2, line.size() - 1);
                        }
                    }
                    login_file.close();
                    if (!correct_token.empty() || ui->line_login->text().toStdString() == "vk-test") {
                        if (full_response.empty()) {
                            typedef std::string(*get_url) (std::string, std::string, std::string);
                            get_url create_url;
                            create_url = (get_url)*GetProcAddress((*iterator).first, "create_url");


                            std::string url = create_url(ui->line_domen->text().toStdString(), ui->line_count->text().toStdString(), correct_token);
                            request.setUrl(QUrl(QString::fromStdString(url)));

                            QNetworkReply* reply = manager->get(request);
                            QEventLoop loop;
                            connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
                            loop.exec();
                        }


                        typedef void(*get_inv_url) (const json&, json&, std::string);
                        get_inv_url get_investment_url;
                        get_investment_url = (get_inv_url)*GetProcAddress((*iterator).first, "get_investment_url");

                        for (int i = 0; i < ui->line_count->text().toInt(); i++) {
                            get_investment_url(full_response[i], cut_response, "");
                        }

                        emit sendTitle(cut_response);

                        typedef std::vector<std::string>(*post_inf) (json&);
                        post_inf get_post;
                        get_post = (post_inf)*GetProcAddress((*iterator).first, "get_inf_post");

                        for (int i = 0; i < ui->line_count->text().toInt(); i++) {
                            std::vector<std::string> vec;
                            vec = get_post(full_response[i]);
                            std::string post_hash = vec[0];
                            std::string post_text = vec[1];
                            std::string repost_text = vec[2];
                            emit sendPost(post_hash, post_text, repost_text);
                        }
                        cut_response = {};
                        sForm->show();
                        this->close();
                        full_response = {};
                    }
                    else {
                        QMessageBox::information(this, QString::fromLocal8Bit("Eror!"), QString::fromLocal8Bit("Логин не найден!"));
                    }
                }
                else {
                    QMessageBox::information(this, QString::fromLocal8Bit("Ошибка!"), QString::fromLocal8Bit("Введите логин!"));
                }
            }
            else {
                QMessageBox::information(this, QString::fromLocal8Bit("Ошибка!"), QString::fromLocal8Bit("Поле короткого имени не может быть пустым!"));
            }
        }
    }
}




