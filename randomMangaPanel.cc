#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <format>
#include <string>
#include <iostream>
#include "json.hpp"

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
    
    cout << chapters_result->body << endl;
    cout << chapter_id << endl;
    

    return 0;
}