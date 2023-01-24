#include"vk-parser.h"

#ifdef _WIN32
extern "C" _declspec(dllexport) std::string create_url(std::string domain, std::string count, std::string token) {
	return "https://api.vk.com/method/wall.get?domain=" + domain + "&count=" + count + "&access_token=" + token + "&v=5.131";
}
#else
extern "C++" std::string create_url(std::string domain, std::string count, std::string token) {
	return "https://api.vk.com/method/wall.get?domain=" + domain + "&count=" + count + "&access_token=" + token + "&v=5.131";
}
#endif

#ifdef _WIN32
extern "C" _declspec(dllexport) void get_investment_url(const json & all_object,json & hash_url, std::string hash) {
	if (hash == "") {
		hash = to_string(all_object["hash"]);
	}
	json ob_type = all_object["attachments"];
	for (int j = 0; j < ob_type.size(); j++) {
		std::string type_inf = ob_type[j]["type"];
		if (type_inf == "video") {
			json video_url = ob_type[j][type_inf]["image"];
			for (int g = 0; g < video_url.size(); g++) {
				if (video_url[g]["width"] == 320 || video_url[g]["height"] == 320) {
					hash_url[hash].push_back(video_url[g]["url"]);
				}
			}
		}
		if (type_inf == "photo") {
			json phot_url = ob_type[j][type_inf]["sizes"];
			for (int g = 0; g < phot_url.size(); g++) {
				if (phot_url[g]["type"] == "x") {
					hash_url[hash].push_back(phot_url[g]["url"]);
				}
			}
		}
	}
	if (all_object.contains("copy_history")) {
		get_investment_url(all_object["copy_history"][0],hash_url, hash);
	}
}

#else
extern "C"void get_investment_url(const json & object, json & hash_url, std::string hash) {
	//json hash_url;
	if (hash == "") {
		hash = to_string(object["hash"]);
	}
	json ob_type = object["attachments"];//json объект для i-го attachments-а
	for (int j = 0; j < ob_type.size(); j++) {
		std::string type_inf = ob_type[j]["type"];
		if (type_inf == "video") {
			json video_url = ob_type[j][type_inf]["image"];
			for (int g = 0; g < video_url.size(); g++) {//это если надо будет перебирать все фотки видео
				if (video_url[g]["width"] == 320 || video_url[g]["height"] == 320) {
					hash_url[hash].push_back(video_url[g]["url"]);
				}
			}
		}
		if (type_inf == "photo") {
			json phot_url = ob_type[j][type_inf]["sizes"];
			for (int g = 0; g < phot_url.size(); g++) {
				if (phot_url[g]["type"] == "x") {
					hash_url[hash].push_back(phot_url[g]["url"]);
				}
			}
		}
	}
	if (object.contains("copy_history")) {
		get_investment_url(object["copy_history"][0], hash_url, hash);
	}
}
#endif

#ifdef _WIN32
extern "C" _declspec(dllexport) std::vector<std::string> get_inf_post(json & one_post) { 
	std::vector<std::string> vec(3);
	vec[0] = one_post["hash"];
	vec[1] = one_post["text"];
	if (one_post.contains("copy_history")) {
		vec[2] = one_post["copy_history"][0]["text"];
	}
	return vec;
}
#else
extern "C" std::map<std::string, std::vector<std::string>> get_inf_post(json & one_post) {
	std::vector<std::string> vec(3);
	vec[0] = one_post["hash"];
	vec[1] = one_post["text"];
	if (one_post.contains("copy_history")) {
		vec[2] = one_post["copy_history"][0]["text"];
	}
	return vec;
}
#endif
