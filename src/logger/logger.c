#include "../../include/logger/logger.h"

static char log_format[_GAS_LOG_MAX_LEN];

const char* log_level_strs[] = {
    #define X(name) #name,
    _GAS_LOG_LEVEL_LIST(X)
    #undef X
};

const char* log_format_strs[] = {
    #define X(name, format) #format,
    _GAS_LOG_FORMAT_LIST(X)
    #undef X
};

gas_logger_format gas_logger_parse_format(const char *ptr) {
    for (size_t i = 0; i < sizeof(log_format_strs) / sizeof(log_format_strs[0]); i++) {
        if (strncmp(ptr, log_format_strs[i], strlen(log_format_strs[i])) == 0) {
            return (gas_logger_format)i;
        }
    }

    return GAS_LOG_FORMAT_NONE;
}


char* gas_logger_get_format_str_impl(const gas_logger_format format, const gas_logger_level level, const char* msg, const char* call_file, const char* call_func, int call_line) {
    switch (format) {
        case GAS_LOG_FORMAT_DATETIME: ;
            time_t t;
            struct tm* local;
            
            time(&t);
            
            local = localtime(&t);
            
            char* date = (char*)malloc(_GAS_LOG_TIME_LEN + 1);
            strftime(date, _GAS_LOG_TIME_LEN, "%d/%m/%Y %H:%M", local);
            return date;
        case GAS_LOG_FORMAT_LINE: ;
            char* line = (char*)malloc(_GAS_LOG_LINE_LEN + 1);
            snprintf(line, _GAS_LOG_LINE_LEN, "%d", call_line);
            return line;
        case GAS_LOG_FORMAT_FUNC: ;
            char* func = (char*)malloc(strlen(call_func) + 1);
            strcpy(func, call_func);
            return func;
        case GAS_LOG_FORMAT_FILE: ;
            char* file = (char*)malloc(strlen(call_file) + 1);
            strcpy(file, call_file);
            return file;
        case GAS_LOG_FORMAT_LEVEL: ;
            char* level_str = (char*)malloc(strlen(log_level_strs[level]) + 1);
            strcpy(level_str, log_level_strs[level]);
            return level_str;
        case GAS_LOG_FORMAT_MSG: ;
            char* msg_str = (char*)malloc(strlen(msg) + 1);
            strcpy(msg_str, msg);
            return msg_str;
        default:
            return NULL;
    }   
}

char* gas_logger_parse_format_str_impl(char* format, const gas_logger_level level, const char* msg, const char* call_file, const char* call_func, int call_line) {
    char* buf = (char*)malloc(_GAS_LOG_MAX_LEN + 1);
    size_t buf_size = 0;
    
    char* end = format + strlen(format);
    while (format < end) {
        gas_logger_format type = gas_logger_parse_format(format);
        if (type == GAS_LOG_FORMAT_NONE) {
            if (*format != '\n') {
                strncpy(buf + buf_size, format, 1);
                buf_size++;
            }
            format++;
            continue;
        }
        
        char* str = gas_logger_get_format_str_impl(type, level, msg, call_file, call_func, call_line);
        strcpy(buf + buf_size, str);
        buf_size += strlen(str);
        free(str);
        
        format += strlen(log_format_strs[type]);
    }
    
    return buf;
}

char* gas_logger_get_format() {
    return log_format;
}

void gas_logger_set_format(const char* format) {
    strcpy(log_format, format);
}