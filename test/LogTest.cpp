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

#include "LogTest.hpp"

TEST(LogTest, TestInit) {
    log_t log;

    ASSERT_EQ(log_open(&log, stderr, LOG_LEVEL_INFO), 0);
    ASSERT_EQ(log_close(log), 0);
}

TEST(LogTest, TestInitFD) {
    log_t log;

    ASSERT_EQ(log_open_fd(&log, STDERR_FILENO, LOG_LEVEL_INFO), 0);
    ASSERT_EQ(log_close(log), 0);
}

TEST(LogTest, TestInitPath) {
    log_t log;

    ASSERT_EQ(log_open_path(&log, "/dev/null", LOG_LEVEL_INFO), 0);
    ASSERT_EQ(log_close(log), 0);
}

TEST_F(LogPrintTest, Basic) {
    ASSERT_GT(lprintf(log, LOG_LEVEL_INFO, "Hello, World!"), 0);
}

TEST_F(LogPrintTest, BasicTooHigh) {
    ASSERT_EQ(lprintf(log, LOG_LEVEL_TRACE, "This should not print."), 0);
}
