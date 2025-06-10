#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <string>
#include <curl/curl.h>

using namespace std;

class ApiClient {
private:
    string baseUrl;
    CURL* curl;
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* response);
    string performRequest(const string& url, const string& method, const string& data = "");
    
public:
    ApiClient(const string& baseUrl);
    ~ApiClient();
    
    bool initialize();
    void cleanup();
    
    //---- functions -----
    string getAllUsers();
    string addUser(const string& name, const string& email);
    string updateUser(int userId, const string& name, const string& email);
    string deleteUser(int userId);
};

#endif