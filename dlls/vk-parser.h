#pragma once
#include<nlohmann/json.hpp>
#include<fstream>

using json = nlohmann::json;

#ifdef _WIN32
extern "C" _declspec(dllexport) std::string create_url(std::string domain, std::string count, std::string token);
#else
extern "C" std::string create_url(std::string domain, std::string count, std::string token);
#endif

#ifdef _WIN32
extern "C" _declspec(dllexport) void get_investment_url(const json & object, json & hash_url, std::string hash="");
#else
extern "C" void get_investment_url(const json & object, json & hash_url, std::string hash = "");
#endif

#ifdef _WIN32
extern "C" _declspec(dllexport) std::vector<std::string> get_text_one_post(json & one_post);
#else
extern "C" std::vector<std::string> get_text_one_post(json & one_post);
#endif
