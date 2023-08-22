#include "JsonHandler.h"
#include "Logger.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <iomanip>
using namespace yazi::util;
using json = nlohmann::json;

JsonHandler::JsonHandler() {

}

JsonHandler::JsonHandler(std::string directory){
    m_directory=directory;
}

JsonHandler::~JsonHandler() {

}

void JsonHandler::set_filepath(std::string filepath){
    m_filepath=filepath;
}

void JsonHandler::append_to_file(const std::string &received) {
    debug("append_to_file");
    // 解析收到的消息为JSON对象
    debug("received: %s",received.c_str());
    json receivedJson = json::parse(received);
    // 获取对应视频的弹幕文件
    string vid=receivedJson["vid"].dump();
    vid.erase(std::remove_if(vid.begin(), vid.end(), [](char ch) {return ch == '\"'; }), vid.end());
    m_filepath = m_directory + vid + "/barrages.json";
    debug("m_filepath: %s",m_filepath.c_str());
    // 读取文件内容
    std::ifstream inFile(m_filepath);
    debug("open ifstream success");
    json j;
    inFile >> j;
    debug("write json success");
    inFile.close();
    // 将接收到的消息追加到barrages数组中
    j["barrages"].push_back(receivedJson);

    // 将更新后的JSON对象写回文件
    std::ofstream outFile(m_filepath);
    outFile << j.dump(4);  // 使用缩进的格式将JSON对象写入文件
    debug("rewrite ofstream success");
    outFile.close();
    try{
        sort_file();
    }catch(const std::exception &e){
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}

void JsonHandler::sort_file(){
    debug("sort_file");
    json j;
    ifstream ifs(m_filepath);
    if (ifs.is_open()) {
        ifs >> j;
        ifs.close();
    }
    debug("open ifstream success");
    if (j.find("barrages") == j.end()) {
        j["barrages"] = nlohmann::json::array();
    }

    sort(j["barrages"].begin(), j["barrages"].end(), [](const nlohmann::json &a, const nlohmann::json &b) {
        // return a["barrageTime"].get<std::string>() < b["barrageTime"].get<std::string>();
        return a["barrageTime"].get<int>() < b["barrageTime"].get<int>();
    });
    debug("sort json success");
    ofstream ofs(m_filepath);
    ofs << setw(4) << j << endl;

    ofs.close();
}