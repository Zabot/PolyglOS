#ifndef LOG_H
#define LOG_H

#define LEVEL_INFO 1
#define LEVEL_WARN 2
#define LEVEL_ERROR 3

#define INFO(...) log(__FILE__, __LINE__, LEVEL_INFO, __VA_ARGS__)
#define WARN(...) log(__FILE__, __LINE__, LEVEL_WARN, __VA_ARGS__)
#define ERROR(...) log(__FILE__, __LINE__, LEVEL_ERROR, __VA_ARGS__)

void log(const char* file, int lineno, int level, const char* format, ...);

#endif
