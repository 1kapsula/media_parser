#pragma once
#include<nlohmann/json.hpp>
#include<fstream>
#include<vector>
#include<string>
using json = nlohmann::json;
/*!
    \defgroup vk-parser Парсер мессенджера vk
    \brief Данная библиотека предназначена для парсинга vk на предмет получения информации из постов открытых групп vk
*/

#ifdef _WIN32
extern "C" _declspec(dllexport) std::string create_url(std::string domain, std::string count, std::string token);
#else
/// <summary>
/// Метод, формирующий ссылку в соответсвии с API вк, для получения информации о постах группы  
/// </summary>
/// <param name="domain">Короткий адрес пользователя или сообщества.</param>
/// <param name="count">Количество постов со страницы</param>
/// <param name="token">Ключ доступа пользователя</param>
/// <returns>Возвращает ссылку для выполнение запроса</returns>
extern "C" std::string create_url(std::string domain, std::string count, std::string token);
#endif

#ifdef _WIN32
extern "C" _declspec(dllexport) void get_investment_url(const json & all_object, json & hash_url, std::string hash="");
#else
/// <summary>
/// \brief Метод, парсящий json-файл со всей информацией о записях на стене группы  
/// \details Получает json-объект all_object, парсит его и записывает полученную информацию в json-объект hash_url в виде ключа(hash поста)-массива ссылок на изображения в посте
/// </summary>
/// <param name="all_object">Json-файл, содержущий массив записей со стены сообщества</param>
/// <param name="hash_url">Пустой json-файл, нужный для записи распаршенной информации</param>
/// <param name="hash">Hash поста, нужен для вызова рекурсии</param>
extern "C" void get_investment_url(const json & all_object, json & hash_url, std::string hash = "");
#endif

#ifdef _WIN32
extern "C" _declspec(dllexport) std::vector<std::string> get_text_one_post(json & one_post);
#else
/// <summary>
/// Метод, записывающий в вектор, информацию об одном посте
/// </summary>
/// <param name="one_post">Json-файл содержущий информацию об одном посте</param>
/// <returns> Возвращает вектор, в котором на 0 позиции находится хеш поста, 1 позиции находится текст поста, на 2 позиции - текст поста который был репостнут</returns>
extern "C" std::vector<std::string> get_text_one_post(json & one_post);
#endif
