#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <format>
#include <string>
#include <iostream>
#include "json.hpp"
#include <windows.h>

using namespace std;
using json = nlohmann::json;

// Note to self: the '&' symbol is a reference to the client that we initialize in main since httplib::Client is not something we can directly store in a variable. 
string GetMangaId(httplib::Client& client);
string GetChapterId(httplib::Client& client, const string& manga_id);
string GetPage(httplib::Client& client, const string& chapter_id);

int main(){
    string base_url = "https://api.mangadex.dev";
    httplib::Client client(base_url);
    string image = "https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fwallpapers.com%2Fimages%2Fhd%2Frick-astley-th6vqytajjixfuqj.jpg&f=1&nofb=1&ipt=b7eaf9290aa2652e22b596442f7cf4648ecf5d4facc5df411b5940942b1bec0b";

    do {
        // Get Random Manga
        string manga_id = GetMangaId(client); 
        // Get Manga's ChapterID
        string chapter_id = GetChapterId(client, manga_id);
        if (chapter_id == ""){
            // Reminder to self: continue will restart the loop from the top if hit. I.e. if continue is hit here, image = GetPage wont run and the code jumps back to GetManga.
            continue;
        }
        image = GetPage(client, chapter_id);
        break;
    }
    while(true);
    
    ShellExecuteA(NULL, "open", image.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);

    return 0;
}

string GetMangaId(httplib::Client& client){
    string url_extension = "/manga/random";

    // Get random manga from mangadex api
    auto manga_result = client.Get(url_extension);

    // Parse manga to json, then find and store manga 'id'
    json manga = json::parse(manga_result->body);

    return manga["data"]["id"];
}

string GetChapterId(httplib::Client& client, const string& manga_id){
    string chapter_url = "/manga/" + manga_id + "/feed";

    auto chapters_result = client.Get(chapter_url);
    json chapters = json::parse(chapters_result->body);
    json chapter_array = chapters["data"];
    // BREAK POINT: IF ARRAY IS EMPTY PROGRAM WONT CONTINUE, THUS NEW MANGA NEEDS TO BE SEARCHED FOR.
    if (chapter_array.empty()){
        return "";
    }
    
    size_t random_index = rand() % chapter_array.size();

    return chapter_array[random_index]["id"];
}

string GetPage(httplib::Client& client, const string& chapter_id){
    // Get json return for all pages from a chapter id
    auto page_result = client.Get("/at-home/server/" + chapter_id);
    json pages = json::parse(page_result->body);
    // Store chapter hash value for later to get image
    string chapter_hash = pages["chapter"]["hash"];
    // Store only the array of page ids
    json page_array = pages["chapter"]["data"];

    // Get random array index and store the value of that index
    size_t random_ind = rand() % page_array.size(); //rename later when put in own function
    string page_id = page_array[random_ind]; 
    
    string base_page_url = "https://uploads.mangadex.org/data/";
    // httplib::Client page_client(base_page_url);

    return base_page_url + chapter_hash + "/" + page_id;
}