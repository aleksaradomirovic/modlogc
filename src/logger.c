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

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "ulogc.h"

#include "logger.h"

static int log_init(struct logger **log_ptr, FILE *stream, log_level_t level) {
    int status;

    errno = 0;
    if(setvbuf(stream, NULL, _IOLBF, BUFSIZ)) {
        status = errno;
        if(!status) {
            status = EIO;
        }

        return status;
    }

    struct logger *log = malloc(sizeof(struct logger));
    if(!log) {
        return errno;
    }

    status = pthread_mutex_init(&log->lock, NULL);
    if(status) {
        free(log);
        return status;
    }

    log->stream = stream;
    log->level = level;

    *log_ptr = log;
    return 0;
}

int log_open_fd(log_t *log, int fd, log_level_t level) {
    int status;

    fd = dup(fd);
    if(fd < 0) {
        return errno;
    }

    FILE *file = fdopen(fd, "a");
    if(!file) {
        status = errno;
        close(fd);
        return status;
    }

    status = log_init(log, file, level);
    if(status) {
        fclose(file);
        return status;
    }

    return 0;
}

int log_open(log_t *log, FILE *stream, log_level_t level) {
    int fd = fileno(stream);
    if(fd < 0) {
        return errno;
    }

    return log_open_fd(log, fd, level);
}

int log_open_path(log_t *log, const char *path, log_level_t level) {
    int status;

    FILE *file = fopen(path, "a");
    if(!file) {
        return errno;
    }

    status = log_init(log, file, level);
    if(status) {
        fclose(file);
        return status;
    }

    return 0;
}

int log_close(log_t log) {
    int status;

    status = pthread_mutex_destroy(&log->lock);
    if(status) {
        return status;
    }

    if(log->stream) {
        if(fclose(log->stream)) {
            return errno;
        }
    }

    free(log);
    return 0;
}
