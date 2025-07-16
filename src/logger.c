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

static int fdopen_dup(FILE **file_ptr, int fd) {
    int status;

    fd = dup(fd);
    if(fd == -1) {
        return errno;
    }

    FILE *file = fdopen(fd, "a");
    if(file == NULL) {
        status = errno;
        close(fd);        
        return status;
    }

    errno = 0;
    if(setvbuf(file, NULL, _IOLBF, BUFSIZ) != 0) {
        status = errno != 0 ? errno : EIO;
        close(fd);
        return status;
    }

    *file_ptr = file;
    return 0;
}

static int fdup(FILE **file_ptr, FILE *file) {
    int fd = fileno(file);
    if(fd == -1) {
        return errno;
    }

    return fdopen_dup(file_ptr, fd);
}

static int log_init(struct logger **log_ptr, FILE *file, log_level_t level) {
    int status;

    struct logger *log = malloc(sizeof(struct logger));
    if(log == NULL) {
        return errno;
    }

    status = pthread_mutex_init(&log->lock, NULL);
    if(status != 0) {
        free(log);
        return status;
    }

    log->stream = file;
    log->level = level;

    *log_ptr = log;
    return 0;
}

int log_open(log_t *log_ptr, FILE *file, log_level_t level) {
    int status;

    FILE *nfile;
    status = fdup(&nfile, file);
    if(status != 0) {
        return status;
    }

    status = log_init(log_ptr, nfile, level);
    if(status != 0) {
        fclose(nfile);
        return status;
    }

    return 0;
}

int log_open_fd(log_t *log_ptr, int fd, log_level_t level) {
    int status;

    FILE *nfile;
    status = fdopen_dup(&nfile, fd);
    if(status != 0) {
        return status;
    }

    status = log_init(log_ptr, nfile, level);
    if(status != 0) {
        fclose(nfile);
        return status;
    }

    return 0;
}

int log_open_path(log_t *log_ptr, const char *path, log_level_t level) {
    int status;

    FILE *file = fopen(path, "a");
    if(file == NULL) {
        return errno;
    }

    status = log_init(log_ptr, file, level);
    if(status != 0) {
        fclose(file);
        return status;
    }

    return 0;
}

int log_close(log_t log) {
    int status;

    status = pthread_mutex_destroy(&log->lock);
    if(status != 0) {
        return status;
    }

    if(fclose(log->stream) != 0) {
        return errno;
    }

    free(log);
    return 0;
}
