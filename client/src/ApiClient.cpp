#include "ApiClient.h"
#include <iostream>
#include <sstream>

using namespace std;

ApiClient::ApiClient(const string& baseUrl) : baseUrl(baseUrl), curl(nullptr) {
}

ApiClient::~ApiClient() {
    cleanup();
}

bool ApiClient::initialize() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    return curl != nullptr;
}

// Cleanup
void ApiClient::cleanup() {
    if (curl) {
        curl_easy_cleanup(curl);
        curl = nullptr;
    }
    curl_global_cleanup();
}

//callback function for writing response data
size_t ApiClient::WriteCallback(void* contents, size_t size, size_t nmemb, string* response) {
    size_t totalSize = size * nmemb;
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Perform HTTP requests
string ApiClient::performRequest(const string& url, const string& method, const string& data) {
    if (!curl) {
        return "Error: CURL not initialized";
    }
    
    string response;
    
    // Reset curl handle for new request
    curl_easy_reset(curl);
    
    // Set URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // Set callback function and data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    // Disable SSL verification for local testing
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    
    // Set timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    
    struct curl_slist* headers = nullptr;
    
    if (method == "POST" || method == "PUT") {
        curl_easy_setopt(curl, method == "POST" ? CURLOPT_POST : CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(data.length()));
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    } else if (method == "DELETE") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    } else {
        // GET request
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    }
    
    // Perform the request
    CURLcode res = curl_easy_perform(curl);
    
    // Clean up headers
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    if (res != CURLE_OK) {
        return "Error: " + string(curl_easy_strerror(res));
    }
    
    return response;
}

// Get all users
string ApiClient::getAllUsers() {
    return performRequest(baseUrl + "/users", "GET");
}

// Add a new user
string ApiClient::addUser(const string& name, const string& email) {
    stringstream jsonData;
    jsonData << "{\"name\":\"" << name << "\",\"email\":\"" << email << "\"}";
    return performRequest(baseUrl + "/users", "POST", jsonData.str());
}

// Update a user
string ApiClient::updateUser(int userId, const string& name, const string& email) {
    stringstream jsonData;
    jsonData << "{\"name\":\"" << name << "\",\"email\":\"" << email << "\"}";
    return performRequest(baseUrl + "/users/" + to_string(userId), "PUT", jsonData.str());
}

// Delete a user by ID
string ApiClient::deleteUser(int userId) {
    return performRequest(baseUrl + "/users/" + to_string(userId), "DELETE");
}