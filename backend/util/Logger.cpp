#include "Logger.h"
using namespace yazi::util;
#include <time.h>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <stdarg.h>
using namespace std;

const char *Logger::s_level[LEVEL_COUNT] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL",
};

Logger *Logger::m_instance = nullptr;

Logger *Logger::instance() {
    if (m_instance == NULL) {
        m_instance = new Logger();
    }
    return m_instance;
}

Logger::Logger() : m_level(DEBUG), m_max(1024 * 1024), m_len(0) {}
Logger::~Logger() { close(); }

void Logger::open(const string &filename) {
    m_mutex.lock();
    m_fout.open(filename, ios::out | ios::trunc);
    m_filename = filename;
    if (m_fout.fail()) {
        throw logic_error("[Logger] open: Could not open log file " + filename);
    }
    m_fout.seekp(0, ios::end);
    m_len = m_fout.tellp();
    m_mutex.unlock();
}
void Logger::close() { m_fout.close(); }

void Logger::log(Level level, const char *file, int line, const char *fmt, ...) {
    if (m_level > level) {
        return;
    }

    if (m_fout.fail()) {
        throw logic_error("[Logger] log: Could not open log file " +m_filename);
    }

    time_t ticks = time(NULL);
    struct tm *ptm = localtime(&ticks);
    // add 8 hours to Beijing time
    ptm->tm_hour+=8;
    mktime(ptm);
    char timestamp[32];
    memset(timestamp, 0, 32);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);


    const char *myfmt = "%s %s %s: %d ";
    int size = snprintf(NULL, 0, myfmt, timestamp, s_level[level], file, line);
    m_mutex.lock();
    if (size > 0) {
        char *buffer = new char[size + 1];
        snprintf(buffer, size + 1, myfmt, timestamp, s_level[level], file, line);
        buffer[size] = '\0';
        m_fout << buffer;
        m_len += size;
        delete[] buffer;
    }

    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    size = vsnprintf(NULL, 0, fmt, arg_ptr);
    va_end(arg_ptr);
    if (size > 0) {
        char *content = new char[size + 1];
        va_start(arg_ptr, fmt);
        size = vsnprintf(content, size + 1, fmt, arg_ptr);
        va_end(arg_ptr);
        m_fout << content;
        m_len += size;
        delete[] content;
    }

    m_fout << endl;
    m_fout.flush();
    m_mutex.unlock();

    if (m_len >= m_max && m_max > 0) {
        rotate();
    }
}

void Logger::rotate() {
    close();

    time_t ticks = time(NULL);
    struct tm *ptm = localtime(&ticks);
    // add 8 hours to Beijing time
    ptm->tm_hour+=8;
    mktime(ptm);
    char timestamp[32];
    memset(timestamp, 0, 32);
    strftime(timestamp, sizeof(timestamp), " %Y-%m-%d %H:%M:%S", ptm);

    string filename = m_filename + timestamp;
    if (rename(m_filename.c_str(), filename.c_str()) != 0) {
        throw std::logic_error("rename log name failed: " +
                               string(strerror(errno)));
    }
    cout << m_filename << endl;
    open(m_filename);
}