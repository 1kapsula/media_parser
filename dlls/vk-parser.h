#pragma once
#include<nlohmann/json.hpp>
#include<fstream>
#include<vector>
#include<string>
using json = nlohmann::json;
/*!
    \defgroup vk-parser ������ ����������� vk
    \brief ������ ���������� ������������� ��� �������� vk �� ������� ��������� ���������� �� ������ �������� ����� vk
*/

#ifdef _WIN32
extern "C" _declspec(dllexport) std::string create_url(std::string domain, std::string count, std::string token);
#else
/// <summary>
/// �����, ����������� ������ � ����������� � API ��, ��� ��������� ���������� � ������ ������  
/// </summary>
/// <param name="domain">�������� ����� ������������ ��� ����������.</param>
/// <param name="count">���������� ������ �� ��������</param>
/// <param name="token">���� ������� ������������</param>
/// <returns>���������� ������ ��� ���������� �������</returns>
extern "C" std::string create_url(std::string domain, std::string count, std::string token);
#endif

#ifdef _WIN32
extern "C" _declspec(dllexport) void get_investment_url(const json & all_object, json & hash_url, std::string hash="");
#else
/// <summary>
/// \brief �����, �������� json-���� �� ���� ����������� � ������� �� ����� ������  
/// \details �������� json-������ all_object, ������ ��� � ���������� ���������� ���������� � json-������ hash_url � ���� �����(hash �����)-������� ������ �� ����������� � �����
/// </summary>
/// <param name="all_object">Json-����, ���������� ������ ������� �� ����� ����������</param>
/// <param name="hash_url">������ json-����, ������ ��� ������ ������������ ����������</param>
/// <param name="hash">Hash �����, ����� ��� ������ ��������</param>
extern "C" void get_investment_url(const json & all_object, json & hash_url, std::string hash = "");
#endif

#ifdef _WIN32
extern "C" _declspec(dllexport) std::vector<std::string> get_text_one_post(json & one_post);
#else
/// <summary>
/// �����, ������������ � ������, ���������� �� ����� �����
/// </summary>
/// <param name="one_post">Json-���� ���������� ���������� �� ����� �����</param>
/// <returns> ���������� ������, � ������� �� 0 ������� ��������� ��� �����, 1 ������� ��������� ����� �����, �� 2 ������� - ����� ����� ������� ��� ���������</returns>
extern "C" std::vector<std::string> get_text_one_post(json & one_post);
#endif
