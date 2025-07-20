/*
 * Copyright 2025 Aleksa Radomirovic
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ULOGC_H_
#define ULOGC_H_


#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


#define ULOGC_EXPORT __attribute__((visibility ("default")))

/**
 * Logger instance.
 */
struct logger;

/**
 * Logger instance handle.
 */
typedef struct logger * log_t;

/**
 * Log level descriptor.
 */
typedef struct log_level {
    int level;
    const char *name;
} log_level_t;

/// Log level for reporting errors.
#define LOG_LEVEL_ERROR     (log_level_t) {  50, "error" }

/// Log level for reporting warnings.
#define LOG_LEVEL_WARNING   (log_level_t) { 150, "warning" }

/// Log level for reporting important information.
#define LOG_LEVEL_INFO      (log_level_t) { 250, "info" }

/// Log level for reporting verbose information.
#define LOG_LEVEL_VERBOSE   (log_level_t) { 275, "verbose" }

/// Log level for reporting debug information.
#define LOG_LEVEL_DEBUG     (log_level_t) { 350, "debug" }

/// Log level for reporting trace information.
#define LOG_LEVEL_TRACE     (log_level_t) { 375, "trace" }

/**
 * Opens a new log at this stream.
 * 
 * @param log a pointer to a log handle to assign
 * @param stream a stream to duplicate and assign to the new log
 * @param level the log's initial default level
 * @return 0 for success, non-zero for failure
 * 
 * @note The stream {@code file} is duplicated when the log is created, meaning that the user still owns {@code file} after calling this function, and must release it by calling {@code fclose(file)} on it themselves.
 *
 * @see log_open_fd()
 */
ULOGC_EXPORT int log_open(log_t *log, FILE *stream, log_level_t level);

/**
 * Opens a new log at this file descriptor.
 * 
 * @param log a pointer to a log handle to assign
 * @param fd a file descriptor to duplicate and assign to the new log
 * @param level the log's initial default level
 * @return 0 for success, non-zero for failure
 * 
 * @note The file descriptor {@code fd} is duplicated when the log is created, meaning that the user still owns {@code fd} after calling this function, and must release it by calling {@code close(fd)} on it themselves.
 * 
 * @see log_open()
 */
ULOGC_EXPORT int log_open_fd(log_t *log, int fd, log_level_t level);

/**
 * Opens a new log at this file path.
 * 
 * @param log a pointer to a log handle to assign
 * @param path a path to a log file
 * @param level the log's initial default level
 * @return 0 for success, non-zero for failure
 *
 * @see log_open()
 */
ULOGC_EXPORT int log_open_path(log_t *log, const char *path, log_level_t level);

/**
 * Closes a log.
 * 
 * @param log a log handle
 * @return 0 for success, non-zero for failure
 */
ULOGC_EXPORT int log_close(log_t log);

/**
 * Logs a message if its level is lesser or equal to the log's level.
 * 
 * @param log a log handle
 * @param level the logging level of the message
 * @param fmt the message's format string
 * @param ... the format string's arguments
 * @return the number of characters written (0 if the logging level is too high), or a negative number on error
 * 
 * @see printf()
 */
ULOGC_EXPORT int lprintf(log_t log, log_level_t level, const char *fmt, ...);

/**
 * Logs a message if its level is lesser or equal to the log's level.
 * 
 * @param log a log handle
 * @param level the logging level of the message
 * @param fmt the message's format string
 * @param args the format string's arguments
 * @return the number of characters written (0 if the logging level is too high), or a negative number on error
 * 
 * @see vprintf()
 */
ULOGC_EXPORT int vlprintf(log_t log, log_level_t level, const char *fmt, va_list args);


#ifdef __cplusplus
}
#endif


#endif
