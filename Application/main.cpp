/************************************************** Description
 * *******************************************************/
/*
    File : main.cpp
    Programmer : Mohammad Lotfi
    Used : main
    Design Pattern : none
    Types of memory : Heap & Stack
    Total Tread : Nothing
    Site : https://www.mahsen.ir
    Tel : +989124662703
    Email : info@mahsen.ir
    Last Update : 2025/2/4
*/
/************************************************** Warnings
 * **********************************************************/
/*
    Only for learning

    curl http://localhost:11434/api/chat -d '{
    "model": "phi3:mini",
    "stream": false,
    "prompt": "Hi"
    }'
*/
/************************************************** Wizards
 * ***********************************************************/
/*
    Nothing
*/
/************************************************** Includes
 * **********************************************************/
#include "Module/Print.hpp"
#include <fstream>
#include <curl/curl.h>
#include "nlohmann/json.hpp"
/************************************************** Defineds
 * **********************************************************/
/*
    Nothing
*/
/************************************************** Names
 * *************************************************************/
/*
    Nothing
*/
/************************************************** Variables
 * *********************************************************/
/*
    Nothing
*/
/************************************************** Opjects
 * ***********************************************************/
using json = nlohmann::json;
/************************************************** Functions
 * *********************************************************/
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* The main function start of program in cpp language */
int main() {
    Println((char *)"Application: Start");

    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    // Default Ollama API endpoint
    std::string url = "http://localhost:11434/api/generate";
    json requestData;
    json respondData;
    requestData["model"] = "phi3:mini";
    requestData["stream"] = false;
    requestData["prompt"] = "Hi";

    curl = curl_easy_init();
    if (curl) {
        // Set Ollama API URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // JSON data for the request
        readBuffer = requestData.dump();
        const char* json_data = readBuffer.c_str();

        // Set HTTP headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set POST request
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        // Response handling
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            Println((char*)curl_easy_strerror(res));
            //std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Find the last '{' and corresponding '}'
            size_t lastOpeningBrace = readBuffer.rfind('{');
            size_t lastClosingBrace = readBuffer.find('}', lastOpeningBrace);
            // Extract the second JSON string
            respondData = json::parse(readBuffer.substr(lastOpeningBrace, lastClosingBrace - lastOpeningBrace + 1));
            Println((char*)respondData["response"].get<std::string>().c_str());
            //Println((char*)readBuffer.c_str());
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }


    return 0;
}
/************************************************** Tasks
 * *************************************************************/
/*
    Nothing
*/
/************************************************** Vectors
 * ***********************************************************/
/*
    Nothing
*/
/**********************************************************************************************************************/
