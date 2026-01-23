#include <iostream>
#include "httplib.h"
#include <nlohmann/json.hpp>

using namespace std;
//Learn more about RESTapi interactions
//Set ManagaDex API
//Find how it sets/stores manga and subsequently manga pages
//How to store/save that image
//FInd out how to interact with other apps on device with c++
//How to open saved image with desired app

int main(){
    
    httplib::Client client("https://api.mangadex.dev/manga/random");



    return 0;
}