// Copyright 2020 Delft University of Technology
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace putong {

/// @brief An std::chrono-based timer wrapper.
template <typename clock = std::chrono::steady_clock>
struct Timer {
  using ns = std::chrono::nanoseconds;
  using point = std::chrono::time_point<clock, ns>;
  using duration = std::chrono::duration<double>;

  /// @brief Construct a new timer. This also starts the timer if start=true.
  explicit Timer(bool start = false) {
    if (start) Start();
  }

  /**
   * \brief Return whether the internal clock used is steady or not.
   *
   * Also see: https://en.cppreference.com/w/cpp/named_req/Clock
   *
   * \return True if the clock is steady, false otherwise.
   */
  inline static auto steady() -> bool { return clock::is_steady; }

  /// \brief Return the resolution in microseconds.
  inline static auto resolution_us() -> double {
    typedef typename std::ratio_multiply<typename clock::period, std::mega>::type us;
    return static_cast<double>(us::num) / us::den;
  }

  /// @brief Timer start point.
  point start_{};
  /// @brief Timer stop point.
  point stop_{};

  /// @brief Start the timer.
  inline void Start() { start_ = clock::now(); }

  /// @brief Stop the timer.
  inline void Stop() { stop_ = clock::now(); }

  /// @brief Retrieve the interval in seconds.
  [[nodiscard]] inline auto seconds() const -> double {
    duration diff = stop_ - start_;
    return diff.count();
  }

  /// @brief Return the interval in seconds as a formatted string.
  [[nodiscard]] inline auto str(int width = 14) const -> std::string {
    std::stringstream ss;
    ss << std::setprecision(width - 5) << std::setw(width) << std::fixed << seconds();
    return ss.str();
  }

  /// @brief Print the interval on some output stream
  inline void report(std::ostream& os = std::cout, bool last = false,
                     int width = 15) const {
    os << std::setw(width) << ((last ? " " : "") + str() + (last ? "\n" : ","))
       << std::flush;
  }
};

/// @brief An std::chrono-based split timer wrapper with a static number of splits.
template <unsigned int num_splits = 1, typename clock = std::chrono::steady_clock>
struct SplitTimer {
  static_assert(num_splits > 0);

  using ns = std::chrono::nanoseconds;
  using point = std::chrono::time_point<clock, ns>;
  using duration = std::chrono::duration<double>;

  point splits[num_splits + 1];
  std::atomic<size_t> split_idx = 0;

  /// @brief Construct a new timer. This also starts the timer if start=true.
  explicit SplitTimer(bool start = false) {
    if (start) Start();
  }

  /// @brief Copy-constructor.
  SplitTimer(const SplitTimer& s) {
    for (size_t i = 0; i < num_splits + 1; i++) {
      splits[i] = s.splits[i];
    }
    // Can't implicitly copy atomic, so we have to do it manually.
    split_idx = s.split_idx.load();
  }

  /// @brief Copy assignment operator
  SplitTimer& operator=(const SplitTimer& other) {
    for (size_t i = 0; i < num_splits + 1; i++) {
      splits[i] = other.splits[i];
    }
    split_idx = other.split_idx.load();
    return *this;
  }

  /**
   * \brief Return whether the internal clock used is steady or not.
   *
   * Also see: https://en.cppreference.com/w/cpp/named_req/Clock
   *
   * \return True if the clock is steady, false otherwise.
   */
  inline static auto steady() -> bool { return clock::is_steady; }

  /// \brief Return the resolution in microseconds.
  inline static auto resolution_us() -> double {
    typedef typename std::ratio_multiply<typename clock::period, std::mega>::type us;
    return static_cast<double>(us::num) / us::den;
  }

  /// @brief Start the timer.
  inline void Start() {
    splits[0] = clock::now();
    split_idx.store(1);
  }

  /// @brief Record a split time.
  inline void Split() {
#ifndef NDEBUG
    if (split_idx > num_splits) {
      throw std::runtime_error("Putong SplitTimer overflows " +
                               std::to_string(num_splits) + " splits.");
    }
#endif
    auto idx = split_idx.fetch_add(1);
    splits[idx] = clock::now();
  }

  /// @brief Retrieve the split intervals in seconds.
  [[nodiscard]] inline auto seconds() const -> std::vector<double> {
    std::vector<double> result;
    for (size_t i = 1; i < num_splits + 1; i++) {
      duration diff = splits[i] - splits[i - 1];
      result.push_back(diff.count());
    }
    return result;
  }

  /// @brief Push comma separated intervals in seconds onto some stream as strings
  inline void report(std::ostream& os = std::cout, int precision = 15) const {
    auto intervals = seconds();
    for (int i = 0; i < intervals.size(); i++) {
      if (i < intervals.size() - 1) {
        os << std::setprecision(precision) << intervals[i] << ",";
      } else {
        os << std::setprecision(precision) << intervals[i];
      }
    }
    os << std::flush;
  }
};

}  // namespace putong
