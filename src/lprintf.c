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
#include <limits.h>
#include <stdckdint.h>

#include "ulogc.h"

#include "logger.h"

static int log_print_locked(log_t log, log_level_t level, const char *fmt, va_list args) {
    int status;
    int count = 0;

    if(level.level > log->level.level) {
        return 0;
    }

    status = vfprintf(log->stream, fmt, args);
    if(status < 0) {
        return status;
    }
    if(ckd_add(&count, count, status)) {
        count = INT_MAX;
    }

    status = fprintf(log->stream, "\n");
    if(status < 0) {
        return status;
    }
    if(ckd_add(&count, count, status)) {
        count = INT_MAX;
    }

    return count;
}

int vlprintf(log_t log, log_level_t level, const char *fmt, va_list args) {
    int status;

    status = pthread_mutex_lock(&log->lock);
    if(status) {
        return status;
    }

    int count = log_print_locked(log, level, fmt, args);
    
    status = pthread_mutex_unlock(&log->lock);
    if(status) {
        return status;
    }

    return count;
}

int lprintf(log_t log, log_level_t level, const char *fmt, ...) {
    int status;

    va_list args;
    va_start(args, fmt);
    status = vlprintf(log, level, fmt, args);
    va_end(args);

    return status;
}
