#ifndef _GAS_INCLUDE_LOGGER_LOGGER_H
#define _GAS_INCLUDE_LOGGER_LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/**
 * @defgroup gas_log_len Length of logging strings.
 * @{
*/

#ifndef _GAS_LOG_TIME_LEN
/** Length of datetime strings. */
#define _GAS_LOG_TIME_LEN 17
#endif

#ifndef _GAS_LOG_LINE_LEN
/** Length of line count strings. */
#define _GAS_LOG_LINE_LEN 16
#endif

#ifndef _GAS_LOG_MAX_LEN
/** Length of total log strings. */
#define _GAS_LOG_MAX_LEN 1024
#endif

/**
 * @}
*/

#define _GAS_LOG_LEVEL_LIST(X) \
X(DEBUG) \
X(INFO) \
X(WARN) \
X(ERROR) \
X(FATAL)

/**
 * @enum gas_logger_level
 * @brief Represents a level to log at.
*/
typedef enum gas_logger_level {
    #define X(name) GAS_LOG_LEVEL_##name,
    _GAS_LOG_LEVEL_LIST(X)
    #undef X
} gas_logger_level;

/**
 * @brief The string version of each logging level.
*/
extern const char* log_level_strs[];

#define _GAS_LOG_FORMAT_LIST(X) \
X(NONE, $NONE) \
X(DATETIME, $DATETIME) \
X(LINE, $LINE) \
X(FUNC, $FUNC) \
X(FILE, $FILE) \
X(LEVEL, $LEVEL) \
X(MSG, $MSG)

/**
 * @enum gas_logger_format
 * @brief Represents a single format specifier for logging.
*/
typedef enum gas_logger_format {
    #define X(name, format) GAS_LOG_FORMAT_##name,
    _GAS_LOG_FORMAT_LIST(X)
    #undef X
} gas_logger_format;

/**
 * @brief The string version of each logging format.
*/
extern const char* log_format_strs[];

/**
 * @brief Convertes a string into a singular gas_logger_format.
 *
 * @param[in] A string to read and convert.
 * @return The gas_logger_format value represented in ptr, or GAS_LOG_FORMAT_NONE if none matched.
*/
gas_logger_format gas_logger_parse_format(const char* ptr);

/**
 * @brief Expands a format specifier into its correct value.
 *
 * @warning Should not be used directly, instead use gas_logger_get_format_str.
 * @warning Return value must be destroyed with free().
 *
 * @param[in] format The format specifier to expand.
 * @param[in] level The current logging level.
 * @param[in] msg The message being logged.
 * @param[in] call_file The file this is being called from.
 * @param[in] call_func The function this being called from.
 * @param[in] call_line The line this is being called from.
 * @return The expanded value, must be freed after.
*/
char* gas_logger_get_format_str_impl(const gas_logger_format format, const gas_logger_level level, const char* msg, const char* call_file, const char* call_func, int call_line);

/**
 * @def gas_logger_get_format_str
 * @brief Expands a format specifier into its correct value.
 *
 * @warning Return value must be destroyed with free().
 *
 * @param[in] format The gas_logger_format specifier to expand.
 * @param[in] level The current gas_logger_level.
 * @param[in] msg The char* message being logged.
 * @return The expanded value, must be freed after.
*/
#define gas_logger_get_format_str(format, level, msg) gas_logger_get_format_str_impl(format, level, msg, __FILE__, __func__, __LINE__)

/**
 * @brief Expands a full format string with correct values.
 *
 * @warning Should not be used directly, instead use gas_logger_parse_format_str.
 * @warning Return value must be destroyed with free().
 *
 * @param[in] format Full format string.
 * @param[in] level Current logging level.
 * @param[in] msg The message being logged.
 * @param[in] call_file The file this is being called from.
 * @param[in] call_func The function this being called from.
 * @param[in] call_line The line this is being called from.
 * @return The expanded string, must be freed after.
*/
char* gas_logger_parse_format_str_impl(char* format, const gas_logger_level level, const char* msg, const char* call_file, const char* call_func, int call_line);

/**
 * @def gas_logger_parse_format_str
 * @brief Expands a full format string with correct values.
 *
 * @warning Return value must be destroyed with free().
 *
 * @param[in] format The gas_logger_format specifier to expand.
 * @param[in] level The current gas_logger_level.
 * @param[in] msg The char* message being logged.
 * @return The expanded value, must be freed after.
*/
#define gas_logger_parse_format_str(format, level, msg) gas_logger_parse_format_str_impl(format, level, msg, __FILE__, __func__, __LINE__)

/**
 * @brief Gets the current format string.
 *
 * @return The current unexpanded format string.
*/
char* gas_logger_get_format();

/**
 * @brief Sets the current format string.
 *
 * @param[in] format The desired format string, can be freed after setting.
*/
void gas_logger_set_format(const char* format);

/**
 * @defgroup gas_log Logging macros
 * @{
*/

/**
 * @def GAS_LOGGER_LOG_NOARGS
 * @brief Prints a formatted message with no printf-specifiers.
 *
 * @param[in] level The gas_logger_level to log at.
 * @param[in] msg The char* to log according to the current format.
*/
#define GAS_LOGGER_LOG_NOARGS(level, msg) do { \
    char* __str = gas_logger_parse_format_str(gas_logger_get_format(), level, msg); \
    printf("%s\n", __str); \
    free(__str); \
} while (0)

/**
 * @def GAS_LOGGER_LOG
 * @brief Prints a formatted message with 1 or more printf-specifiers.
 *
 * @param[in] level The gas_logger_level to log at.
 * @param[in] msg The char* to log according to the current format. Can have printf-specifiers.
 * @param[in] ... The values to be passed to printf according to specifiers in msg.
*/
#define GAS_LOGGER_LOG(level, msg, ...) do { \
    char* __str = gas_logger_parse_format_str(gas_logger_get_format(), level, msg); \
    char* __copy = malloc(strlen(__str) + 2); \
    sprintf(__copy, "%s\n", __str); \
    free(__str); \
    printf(__copy, __VA_ARGS__); \
    free(__copy); \
} while (0)

/**
 * @}
*/
#endif