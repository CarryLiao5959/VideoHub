#pragma once

#include<string>
using namespace std;

class JsonHandler{
    public:
        JsonHandler();
        JsonHandler(std::string filepath);
        ~JsonHandler();

        void set_filepath(std::string filepath);

        void append_to_file(const std::string &received);
        void sort_file();

    private:
        std::string m_filepath;
};