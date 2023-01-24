#ifndef FORM_H
#define FORM_H

////QT INCLUDES////
#include<QListWidget>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include<QtNetwork>
#include<QPixmap>
#include<QMessageBox>
#include<QFile>
#include<QUrl>

////STD INCLUDES////
#include<string>
#include<vector>
#include<fstream>

////NLOHMANN INCLUDES////
#include<nlohmann/json.hpp>


using json = nlohmann::json;

namespace Ui {
    class Form;
}
/// <summary>
/// �����-�����, ������ ��� ��������� ��������, ����������� ����������, ������������ ����������
/// </summary>
class Form : public QWidget
{
    Q_OBJECT
public:
    /// <summary>
    /// ����������� ������ Form. � ��������� ������������ ����� �������� ��������� �� ������������ �����,
    /// ��� �������� ���������� ������� ���������� ������������� ������. � ��� ���������� �������� ���������� ��������� ������� � ����
    /// </summary>
    /// <param name="parent">��������� �� ������������ �����</param>
    explicit Form(QWidget* parent = nullptr);
    /// <summary>
    /// ���������� ������ Form. ������� ��������� ��������� �������� �������, ������� ����������� ����������
    /// </summary>
    ~Form();
    /// <summary>
    /// �������� json-������ �� �����-������� ��������� � �����
    /// </summary>
    json ans;
signals:
    /// <summary>
    /// ������ ����� ���������� �������� ������������� ���������� ������ Form, � ����������� ��������� ������ MainWindow, � ������� ����� ������� ����������� ������ �����
    /// </summary>
    void firstForm();
public slots:
    /// <summary>
    /// ����-����������, ������� ��������� ���������� � �����.
    /// </summary>
    void createGaleleryPost(std::string hash_post, std::string text_post, std::string text_repost);
    /// <summary>
    /// ����-���������� �������� ����������, �������� json-������, �� ������ ��� ������ ������ ���������� ��� ����� � ���� ������ ����� ��� ���, �� ������� ���
    /// </summary>
    /// <param name="title">Json-������ � ����������� � �����</param>
    void getTitle(json title);
private slots:
    /// <summary>
    /// ����-���������� ������� ������� "�����", ����� �������� �������� ����������� ������, � ����� �������� ���� ������ Form, � ����������� ������� ������� firstForm()
    /// </summary>
    void on_back_clicked();
private:
    /// <summary>
    /// ����, �������� ��������� �� ���������, ���������� �� ����������� ��������� � ��� ��������
    /// </summary>
    Ui::Form* ui;
    /// <summary>
    /// ����, �������� ��������� �� ��������� ��������� ������� � ����
    /// </summary>
    QNetworkAccessManager* m_manager;
    /// <summary>
    /// ����, �������� � ���� ������� ������, ������������ ��������� ������� � ����
    /// </summary>
    QNetworkRequest f_request;
};
#endif // FORM_H