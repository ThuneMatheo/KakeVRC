#include <iostream>
#include <nlohmann/json.hpp>
#include "Console.hpp"
#include <Obf.hpp>
#include "Globals.hpp"
#include "FewTags.h"
#include <curl/curl.h>
#include <thread>
#include <chrono>

using json = nlohmann::json;

struct MemoryStruct {
    char* memory;
    size_t size;
};

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        Console::i().logError(obf("[FewTags][Curl] not enough memory (realloc returned NULL)"));
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void addTagIfNotExists(const std::string& userID, const std::string& tag) {
    if (FewTagsTagMap[userID].find(tag) == FewTagsTagMap[userID].end()) {
        FewTagsTagMap[userID].insert(tag);
    }
}

void FewTags::RefreshFewTags() {
    FewTagsTagMap.clear();
    auto addJsonIfNotExists = [](const std::string& userID, const json& record) {
        if (UIDJsonMap.find(userID) == UIDJsonMap.end()) {
            UIDJsonMap[userID] = record;
        }
    };

    CURL* curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = (char*)malloc(1);
    chunk.size = 0;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        Console::i().logError(obf("[FewTags][Curl] Failed to initialize curl."));
    }
    // https://raw.githubusercontent.com/Fewdys/FewTags/main/FewTags.json
    curl_easy_setopt(curl, CURLOPT_URL, obf("https://raw.githubusercontent.com/Fewdys/FewTags/main/FewTags.json").c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::stringstream buffer;
        buffer << obf("[FewTags][Curl] Failed to fetch data: ") << curl_easy_strerror(res);
        Console::i().logError(buffer.str());
        curl_easy_cleanup(curl);
        return;
    }

    curl_easy_cleanup(curl);

    try {
        json jsonData = json::parse(chunk.memory);
        for (const auto& record : jsonData["records"]) {
            std::string userID = record["UserID"];

            addJsonIfNotExists(userID, record);

            if (std::find(FewTagsUIDS.begin(), FewTagsUIDS.end(), userID) == FewTagsUIDS.end())
                FewTagsUIDS.push_back(userID);

            if (!record["Tag"].empty()) {
                for (const auto& tag : record["Tag"]) {
                    addTagIfNotExists(userID, tag);
                }
            }

        }
    }
    catch (const std::exception& e) {
        std::stringstream buffer;
        buffer << obf("[FewTags][JSON] Error parsing JSON: ") << e.what();
        Console::i().logError(buffer.str());
        return;
    }

    curl_global_cleanup();

    free(chunk.memory);

    return;
}

void FewTags::Update() {
    // auto ThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get()); // we arent touching anything game related so we shouldnt need to do this
    while (true)
    {
        std::stringstream buffer;
        buffer << obf("[FewTags] Refreshing");
        Console::i().log(buffer.str());
        FewTags::RefreshFewTags();
        std::this_thread::sleep_for(std::chrono::seconds(150));
    }
}