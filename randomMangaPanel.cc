#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <format>
#include <string>
#include <iostream>
#include "json.hpp"
#include <windows.h>

using namespace std;
using json = nlohmann::json;

//Find how it sets/stores manga and subsequently manga pages
//How to store/save that image
//FInd out how to interact with other apps on device with c++
//How to open saved image with desired app

// TODO: Create a function to Get Chapters, then a function to Get Pages. 
// TODO: Implement error handling
// TODO: If chapter array is empty/null, run GetChapters again. 

int main(){
    
    string base_url = "https://api.mangadex.dev";
    string url_extension = "/manga/random";

    // Get random manga from mangadex api
    httplib::Client client(base_url);
    auto manga_result = client.Get(url_extension);

    // Error handling for manga request

    // Parse manga to json, then find and store manga 'id'
    json manga = json::parse(manga_result->body);
    string manga_id = manga["data"]["id"];

    // Create chapter endstring to 
    string chapter_url = "/manga/" + manga_id + "/feed";

    // Get list of chapters for manga id
    auto chapters_result = client.Get(chapter_url);
    json chapters = json::parse(chapters_result->body);
    json chapter_array = chapters["data"];

    // Get random array index, and get the id for that index's chapter
    size_t random_index = rand() % chapter_array.size();
    string chapter_id = chapter_array[random_index]["id"];

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
    httplib::Client page_client(base_page_url);

    string image = base_page_url + chapter_hash + "/" + page_id;
    // auto page_image = client.Get(chapter_hash + "/" + page_id);
    
    ShellExecuteA(NULL, "open", image.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);

    
    cout << image << "\n" << endl;
    cout << chapter_hash << endl;
    // cout << page_image << endl;
    

    return 0;
}