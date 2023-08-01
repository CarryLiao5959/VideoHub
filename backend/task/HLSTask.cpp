#include "HLSTask.h"
#include "Logger.h"
#include "Singleton.h"
using namespace yazi::util;
using namespace yazi::task;
using namespace yazi::socket;
#include <sstream>
using std::ostringstream;
#include <fstream>
#include <dirent.h>
#include <vector>
#include <string>
#include <iostream>

HLSTask::HLSTask(Socket *socket) : Task(socket) {
    debug("new HLSTask sockfd: %d", socket->getfd());
}

HLSTask::~HLSTask() {}

void HLSTask::run() {
    debug("HLSTask running");
    SocketHandler *handler = Singleton<SocketHandler>::instance();
    Socket *socket = static_cast<Socket *>(m_data);

    string directory = "file/mp4/";
    std::vector<std::string> filenames;

    DIR *dirp = opendir(directory.c_str());
    struct dirent *dp;
    while ((dp = readdir(dirp)) != NULL) {
        std::string filename(dp->d_name);
        if (filename == "." || filename == "..")
            continue;
        int pos = filename.rfind(".");
        int len = filename.size()-pos;
        std::string tmp = filename.substr(pos + 1, len);
        if (tmp== "ts" || tmp == "m3u8"){
            filenames.push_back(directory + filename);
            debug("get %s", filename.c_str())
        }
    }
    closedir(dirp);

    char buf[hls_buf_size];
    memset(buf, 0, hls_buf_size);
    std::string file_num = to_string(filenames.size());
    memcpy(buf, file_num.c_str(), file_num.size());
    int ret = socket->send(buf, file_num.size());
    if(ret < 0) {
        error("send error: %s", strerror(errno));
    }
    debug("send file_num: %d", file_num);

    // for (const auto &filename : filenames) {
    //     std::ifstream file(filename, std::ios::binary);
    //     if (!file) {
    //         error("could not open %s", filename.c_str());
    //         handler->remove(socket);
    //         return;
    //     }
    //     debug("open %s", filename.c_str());

    //     file.seekg(0, std::ios::end);
    //     std::streamsize size = file.tellg();
    //     file.seekg(0, std::ios::beg);

    //     char buf[hls_buf_size];
    //     memset(buf, 0, hls_buf_size);

    //     int filename_length = filename.size();
    //     socket->send(&filename_length, sizeof(int));
    //     usleep(100);
    //     socket->send(filename.c_str(), filename_length);
    //     usleep(100);

    //     int cnt = 1;
    //     while (!file.eof()) {
    //         file.read(buf, hls_buf_size);
    //         std::streamsize count = file.gcount();
    //         socket->send(buf, count);
    //         usleep(100);
    //         cnt++;
    //         memset(buf, 0, hls_buf_size);
    //     }
    //     debug("send package %d", cnt);
    //     debug("%s sent success", filename.c_str());

    //     file.close();
    // }

    handler->attach(socket);
}

void HLSTask::destroy() {
    debug("work job destory");
    delete this;
}