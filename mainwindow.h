#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <filesystem>
#include<form.h>
#include<nlohmann/json.hpp>
#include <QNetworkAccessManager>
#include<QtNetwork/QNetworkRequest>
#include <QPixmap>
#include<QComboBox>

namespace fs = std::filesystem;
using json = nlohmann::json;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    json response;
    json ans_response;
    explicit MainWindow(QWidget* parent = 0);
    void openDll(fs::path path);
    void closeDll();
    void deletePhotos();

    static void insertDllIntoList(const HINSTANCE& mem, const fs::path& filepath);
    ~MainWindow();
private slots:
    void on_DownloadButton_clicked();
    void ReplyFinished(QNetworkReply* reply);
signals:
    void sendTitle(json title);
    void sendPost(std::string hash_post, std::string text_post, std::string text_repost,int i);
private:
    Ui::MainWindow* ui;
    Form* sForm;
    QComboBox* aaaaa;
    QNetworkAccessManager* manager;
    QNetworkRequest request;
    inline static std::vector<std::pair<HINSTANCE, fs::path>> dynLibsList;
};
#endif // MAINWINDOW_H