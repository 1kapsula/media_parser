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
/// Класс-формы, служит для обработки сигналов, отображения полученной, распаршенной информации
/// </summary>
class Form : public QWidget
{
    Q_OBJECT
public:
    /// <summary>
    /// Конструктор класса Form. В аргументы конструктора можно передать указатель на родительский класс,
    /// что передаст управление памятью экземпляру родительского класса. В нем происходит создание экземпляра менеджера доступа к сети
    /// </summary>
    /// <param name="parent">Указатель на родительский класс</param>
    explicit Form(QWidget* parent = nullptr);
    /// <summary>
    /// Деструктор класса Form. Удаляет экземпляр менеджера сетевого доступа, удаляет графическую компоненту
    /// </summary>
    ~Form();
    /// <summary>
    /// Получает json-объект из ключа-массива фтографий к посту
    /// </summary>
    json ans;
signals:
    /// <summary>
    /// Сигнал после происходит закрытие графическоого интерфейса класса Form, и открывается интерфейс класса MainWindow, с помощью этого сигнала реализована кнопка назад
    /// </summary>
    void firstForm();
public slots:
    /// <summary>
    /// Слот-обработчик, который принимает информацию о посте.
    /// </summary>
    void createGaleleryPost(std::string hash_post, std::string text_post, std::string text_repost);
    /// <summary>
    /// Слот-обработчик загрузки фотографий, получает json-объект, на основе его данных строит уникальное имя файла и если такого файла еще нет, то создает его
    /// </summary>
    /// <param name="title">Json-объект с информацией о посте</param>
    void getTitle(json title);
private slots:
    /// <summary>
    /// Слот-обработчик сигнала нажатия "назад", после начинает удаления графических иконок, а затем закрытие окна класса Form, с последующем вызовом сигнала firstForm()
    /// </summary>
    void on_back_clicked();
private:
    /// <summary>
    /// Поле, хранящее указатель на экземпляр, отвечающий за графический интерфейс и его элементы
    /// </summary>
    Ui::Form* ui;
    /// <summary>
    /// Поле, хранящее указатель на экземпляр менеджера доступа к сети
    /// </summary>
    QNetworkAccessManager* m_manager;
    /// <summary>
    /// Поле, хранящее в себе сетевой запрос, передаваемый менеджеру доступа к сети
    /// </summary>
    QNetworkRequest f_request;
};
#endif // FORM_H