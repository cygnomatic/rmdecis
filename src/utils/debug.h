//
// Created by catslashbin on 23-3-30.
//

#ifndef CYGNOIDES_DECISION_DEBUG_H
#define CYGNOIDES_DECISION_DEBUG_H

#include <chrono>
#include <iostream>
#include <spdlog/spdlog.h>

#define TIMEIT(tag, func)                                                      \
  auto start_##tag = std::chrono::high_resolution_clock::now();                \
  { func; }                                                                    \
  auto end_##tag = std::chrono::high_resolution_clock::now();                  \
  std::chrono::duration<double, std::milli> duration_##tag =                   \
      end_##tag - start_##tag;                                                 \
  if (duration_##tag.count() > 100) {                                          \
    spdlog::info("TIMEIT - {}: Time taken: {} ms.", #tag,                      \
                 duration_##tag.count());                                      \
  };

#endif // CYGNOIDES_DECISION_DEBUG_H
