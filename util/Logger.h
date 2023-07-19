#pragma once

#include <fstream>
#include <string>

using namespace std;

namespace yazi {
namespace util {

class Logger {
  public:
    enum Level {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        FATAL,
        LEVEL_COUNT,
    };
    void log(Level level, const char *file, int line, const char *fmt, ...);
    void open(const string &filename);
    void close();
    void level(Level level) { m_level = level; }
    void max(int bytes) { m_max = bytes; }

    static Logger *instance();

  private:
    Logger();
    ~Logger();

    void rotate();

  private:
    string m_filename;
    ofstream m_fout;
    Level m_level;
    int m_max;
    int m_len;
    static const char *s_level[LEVEL_COUNT];
    static Logger *m_instance;
};

#define debug(format, ...)                                                     \
    Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, format,         \
                            ##__VA_ARGS__);
#define info(format, ...)                                                      \
    Logger::instance()->log(Logger::INFO, __FILE__, __LINE__, format,          \
                            ##__VA_ARGS__);
#define warn(format, ...)                                                      \
    Logger::instance()->log(Logger::WARN, __FILE__, __LINE__, format,          \
                            ##__VA_ARGS__);
#define error(format, ...)                                                     \
    Logger::instance()->log(Logger::ERROR, __FILE__, __LINE__, format,         \
                            ##__VA_ARGS__);
#define fatal(format, ...)                                                     \
    Logger::instance()->log(Logger::FATAL, __FILE__, __LINE__, format,         \
                            ##__VA_ARGS__);

} // namespace util
} // namespace yazi