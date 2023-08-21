#include "JsonHandler.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <iomanip>

using json = nlohmann::json;

JsonHandler::JsonHandler() {

}

JsonHandler::JsonHandler(std::string filepath){
    m_filepath=filepath;
}

JsonHandler::~JsonHandler() {

}

void JsonHandler::set_filepath(std::string filepath){
    m_filepath=filepath;
}

void JsonHandler::append_to_file(const std::string &received) {
    // 读取文件内容
    std::ifstream inFile(m_filepath);
    json j;
    inFile >> j;
    inFile.close();

    // 解析收到的消息为JSON对象
    json receivedJson = json::parse(received);
    
    // 将接收到的消息追加到barrages数组中
    for (const auto& barrage : receivedJson["barrages"]) {
        j["barrages"].push_back(barrage);
    }

    // 将更新后的JSON对象写回文件
    std::ofstream outFile(m_filepath);
    outFile << j.dump(4);  // 使用缩进的格式将JSON对象写入文件
    outFile.close();

    sort_file();
}

void JsonHandler::sort_file(){
    json j;

    ifstream ifs(m_filepath);
    if (ifs.is_open()) {
        ifs >> j;
        ifs.close();
    }
    if (j.find("barrages") == j.end()) {
        j["barrages"] = nlohmann::json::array();
    }

    sort(j["barrages"].begin(), j["barrages"].end(), [](const nlohmann::json &a, const nlohmann::json &b) {
        return a["barrageTime"].get<std::string>() < b["barrageTime"].get<std::string>();
    });

    ofstream ofs(m_filepath);
    ofs << setw(4) << j << endl;

    ofs.close();
}