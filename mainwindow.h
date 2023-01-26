#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<fstream>

////QT INCLUDES////
#include <QMainWindow>
#include <QNetworkAccessManager>
#include<QNetworkRequest>
#include<QUrl>
#include <QNetworkReply>
#include<QMessageBox>
#include<QFile>
#include<QLineEdit>
#include<QComboBox>
#include<QValiDator>

////STD INCLUDES////
#include <filesystem>
#include<windows.h>
#include<fstream>

////NLOHMANN INCLUDES////
#include<nlohmann/json.hpp>

////FORM INCLUDES////
#include<form.h>

namespace fs = std::filesystem;
using json = nlohmann::json;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/// <summary>
/// Основной класс, отображающий графический интерфейс, содержит функционал для работы с динамическими библиотеками,
/// содержит методы для отправки запросов, и загрузки информации постов
/// </summary>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /// <summary>
    /// Json-файл, содержащий массив записей со стены 
    /// </summary>
    json full_response;
    /// <summary>
    /// Json-файл, содержащий в качества ключа хеш поста, а в качестве значений массив ссылок на фотографии,
    /// нужен для загрузки и последующего отображения фотографий
    /// </summary>
    json cut_response;
    /// <summary>
    /// Конструктор класса MainWindow. В аргументы конструктора можно передать указатель на родительский класс,
    /// что передаст управление памятью экземпляру родительского класса. В конструкторе происходит поиск и открытие
    /// доступных динамических библиотек, создаются экземпляры менеджера доступа к сети.
    /// </summary>
    /// <param name="parent">Указатель на родительский класс</param>
    explicit MainWindow(QWidget* parent = nullptr);
    /// <summary>
    /// Метод, загружающий динамические библиотеки
    /// </summary>
    /// <param name="path">Путь к библитеки</param>
    void openDll(fs::path path);
    /// <summary>
    /// Метод, закрывающий динамические библиотеки, нужен дя освобождения памяти 
    /// </summary>
    void closeDll();
    /// <summary>
    /// Метод, удаляющий все фотографии
    /// </summary>
    void deletePhotos();
#ifdef _WIN32
    /// <summary>
    /// Метод, создающий пару из значений дескриптора динамической библиотеки и пути к ней. После создания пары, она помещается
    /// в массив dynLibsList. Объявляется и определяется только при сборке под Windows
    /// </summary>
    /// <param name="mem">Дескриптор библиотеки</param>
    /// <param name="filepath">Путь к библиотеке</param>
    static void insertDllIntoList(const HINSTANCE& mem, const fs::path& filepath);

#else
    /// <summary>
    /// Метод, создающий пару из значений дескриптора динамической библиотеки и пути к ней. После создания пары, она помещается
    /// в массив dynLibsList. Объявляется и определяется только при сборке под Windows
    /// </summary>
    /// <param name="mem">Дескриптор библиотеки</param>
    /// <param name="filepath">Путь к библиотеке</param>
    static void insertDllIntoList(void* mem, fs::path& filepath);
#endif
    /// <summary>
    /// Деструктор класса MainWindow. Удаляет динамически выделенную память, высвобождает динамические библиотеки,
    /// удаляет графическую компоненту
    /// </summary>
    ~MainWindow();
private slots:
    /// <summary>
    /// Слот-обработчик нажатия на кновпку "Загрузить", осуществляет загрузку фотографий и текста, после закрывет форму главного класса и открывает форму класса-галлереи
    /// </summary>
    void on_DownloadButton_clicked();
    /// <summary>
    /// Слот, обрабатывающий ответ менеджера доступа к сети, полученные данные передает в full_responce
    /// </summary>
    /// <param name="reply">Результат работы менеджера доступа к сети</param>
    void ReplyFinished(QNetworkReply* reply);
    /// <summary>
    /// Слот-обработчик нажатия на кнопку "Очистить", осуществляет удаление всех загруженных фотографий
    /// </summary>
    void on_DeletePhotos_clicked();
signals:
    /// <summary>
    /// Сигнал после которого начинается выгрузка фотографий
    /// </summary>
    void sendTitle(json title);
    /// <summary>
    /// Сигнал после которого начинается создание ленты в форме-галлереи
    /// </summary>
    /// <param name="hash_post">Хеш поста, нужен для группировки фотографий с текстов</param>
    /// <param name="text_post">Текст поста</param>
    /// <param name="text_repost">Текст репоста</param>
    void sendPost(std::string hash_post, std::string text_post, std::string text_repost);
private:
    int max_count;
#ifdef _WIN32
    /// <summary>
    /// Массив, содержащий в себе информацию об открытых динамических библиотеках. Хранит дескрипторы и пути
    /// к библиотекам. Объявляется и определяется только при сборке под Windows
    /// </summary>
    inline static std::vector<std::pair<HINSTANCE, fs::path>> dynLibsList;
#else
    /// <summary>
    /// Массив, содержащий в себе информацию об открытых динамических библиотеках. Хранит дескрипторы и пути
    /// к библиотекам. Объявляется и определяется только при сборке не под Windows
    /// </summary>
    inline static std::vector<std::pair<void*, fs::path>> dynLibsList;
#endif
    /// <summary>
    /// Поле, хранящее указатель на экземпляр, отвечающий за графический интерфейс и его элементы
    /// </summary>
    Ui::MainWindow* ui;
    /// <summary>
    /// Поле, хранящее указатель на экземпляр другого класса, отвечает за отображение интерфейса кдасса-формы
    /// </summary>
    Form* sForm;
    /// <summary>
    /// Поле, хранящее указатель на экземпляр менеджера доступа к сети
    /// </summary>
    QNetworkAccessManager* manager;
    /// <summary>
    /// Поле, хранящее в себе сетевой запрос, передаваемый менеджеру доступа к сети
    /// </summary>
    QNetworkRequest request;
    /// <summary>
    /// Поле, хранящее указатель на экземпляр класса выбора элемента, доступного парсера соц-сети
    /// </summary>
    QComboBox* chooseParser;
    /// <summary>
    /// Поле, хранящее указатель на экземпляр класса Qlabel, показвает количество загруженных фотографий
    /// </summary>
    QLabel* count_photos;
    /// <summary>
    /// Переменна-счетчик, хранит количество скачаных фотографий
    /// </summary>
    int I = 0;
    /// <summary>
    /// Метод, считающий количество скачанных фотографий
    /// </summary>
    void setCount();
};
#endif // MAINWINDOW_H
