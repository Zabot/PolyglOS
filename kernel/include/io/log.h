#ifndef LOG_H
#define LOG_H

#define LEVEL_INFO 1
#define LEVEL_WARN 2
#define LEVEL_ERROR 3

#define INFO(system,...) log(__FILE__, __LINE__, LEVEL_INFO, system, __VA_ARGS__)
#define WARN(system,...) log(__FILE__, __LINE__, LEVEL_WARN, system, __VA_ARGS__)
#define ERROR(system,...) log(__FILE__, __LINE__, LEVEL_ERROR, system, __VA_ARGS__)

void log(const char* file, int lineno, int level, const char* system, const char* format, ...);

#endif
