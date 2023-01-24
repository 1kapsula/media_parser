#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include<QNetworkRequest>
#include<QUrl>
#include <QNetworkReply>
#include<QMessageBox>
#include<QFile>
#include<QLineEdit>
#include<QComboBox>

#include <filesystem>
#include<windows.h>
#include<fstream>

#include<form.h>

#include<nlohmann/json.hpp>
namespace fs = std::filesystem;
using json = nlohmann::json;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/// <summary>
/// �������� �����, ������������ ����������� ���������, �������� ���������� ��� ������ � ������������� ������������,
/// �������� ������ ��� �������� ��������, � �������� ���������� ������
/// </summary>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /// <summary>
    /// Json-����, ���������� ������ ������� �� ����� 
    /// </summary>
    json full_response;
    /// <summary>
    /// Json-����, ���������� � �������� ����� ��� �����, � � �������� �������� ������ ������ �� ����������,
    /// ����� ��� �������� � ������������ ����������� ����������
    /// </summary>
    json cut_response;
    /// <summary>
    /// ����������� ������ MainWindow. � ��������� ������������ ����� �������� ��������� �� ������������ �����,
    /// ��� �������� ���������� ������� ���������� ������������� ������. � ������������ ���������� ����� � ��������
    /// ��������� ������������ ���������, ��������� ���������� ��������� ������� � ����.
    /// </summary>
    /// <param name="parent">��������� �� ������������ �����</param>
    explicit MainWindow(QWidget* parent = nullptr);
    /// <summary>
    /// �����, ����������� ������������ ����������
    /// </summary>
    /// <param name="path">���� � ���������</param>
    void openDll(fs::path path);
    /// <summary>
    /// �����, ����������� ������������ ����������, ����� �� ������������ ������ 
    /// </summary>
    void closeDll();
    /// <summary>
    /// �����, ��������� ��� ����������
    /// </summary>
    void deletePhotos();
#ifdef _WIN32
    /// <summary>
    /// �����, ��������� ���� �� �������� ����������� ������������ ���������� � ���� � ���. ����� �������� ����, ��� ����������
    /// � ������ dynLibsList. ����������� � ������������ ������ ��� ������ ��� Windows
    /// </summary>
    /// <param name="mem">���������� ����������</param>
    /// <param name="filepath">���� � ����������</param>
    static void insertDllIntoList(const HINSTANCE& mem, const fs::path& filepath);

#else
    /// <summary>
    /// �����, ��������� ���� �� �������� ����������� ������������ ���������� � ���� � ���. ����� �������� ����, ��� ����������
    /// � ������ dynLibsList. ����������� � ������������ ������ ��� ������ ��� Windows
    /// </summary>
    /// <param name="mem">���������� ����������</param>
    /// <param name="filepath">���� � ����������</param>
    static void insertDllIntoList(void* mem, fs::path& filepath);
#endif
    /// <summary>
    /// ���������� ������ MainWindow. ������� ����������� ���������� ������, ������������ ������������ ����������,
    /// ������� ����������� ���������� ����������, ������� ����������� ����������
    /// </summary>
    ~MainWindow();
private slots:
    /// <summary>
    /// ����-���������� ������� �� ������� "���������", ������������ �������� ���������� � ������, ����� �������� ����� �������� ������ � ��������� ����� ������-��������
    /// </summary>
    void on_DownloadButton_clicked();
    /// <summary>
    /// ����, �������������� ����� ��������� ������� � ����, ���������� ������ �������� � full_responce
    /// </summary>
    /// <param name="reply">��������� ������ ��������� ������� � ����</param>
    void ReplyFinished(QNetworkReply* reply);
signals:
    /// <summary>
    /// ������ ����� �������� ���������� �������� ����������
    /// </summary>
    void sendTitle(json title);
    /// <summary>
    /// ������ ����� �������� ���������� �������� ����� � �����-��������
    /// </summary>
    /// <param name="hash_post">��� �����, ����� ��� ����������� ���������� � �������</param>
    /// <param name="text_post">����� �����</param>
    /// <param name="text_repost">����� �������</param>
    void sendPost(std::string hash_post, std::string text_post, std::string text_repost);
private:
#ifdef _WIN32
    /// <summary>
    /// ������, ���������� � ���� ���������� �� �������� ������������ �����������. ������ ����������� � ����
    /// � �����������. ����������� � ������������ ������ ��� ������ ��� Windows
    /// </summary>
    inline static std::vector<std::pair<HINSTANCE, fs::path>> dynLibsList;
#else
    /// <summary>
    /// ������, ���������� � ���� ���������� �� �������� ������������ �����������. ������ ����������� � ����
    /// � �����������. ����������� � ������������ ������ ��� ������ �� ��� Windows
    /// </summary>
    inline static std::vector<std::pair<void*, fs::path>> dynLibsList;
#endif
    /// <summary>
    /// ����, �������� ��������� �� ���������, ���������� �� ����������� ��������� � ��� ��������
    /// </summary>
    Ui::MainWindow* ui;
    /// <summary>
    /// ����, �������� ��������� �� ��������� ������� ������, �������� �� ����������� ���������� ������-�����
    /// </summary>
    Form* sForm;
    /// <summary>
    /// ����, �������� ��������� �� ��������� ��������� ������� � ����
    /// </summary>
    QNetworkAccessManager* manager;
    /// <summary>
    /// ����, �������� � ���� ������� ������, ������������ ��������� ������� � ����
    /// </summary>
    QNetworkRequest request;
    QComboBox* chooseParser;
};
#endif // MAINWINDOW_H