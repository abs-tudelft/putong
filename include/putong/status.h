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

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>

namespace putong {

enum class StatusType : char { OK, Error };

template <typename E>
class Status {
  static_assert(std::is_enum_v<E>);

 public:
  Status() = default;
  Status(E code, std::string message)
      : status_(StatusType::Error), err_(code), msg_(std::move(message)) {}

  static auto OK() -> Status { return Status(); }

  [[nodiscard]] auto ok() const -> bool { return status_ == StatusType::OK; }
  [[nodiscard]] auto msg() const -> std::string { return msg_; }
  [[nodiscard]] auto err() const -> E { return err_; }

 private:
  StatusType status_ = StatusType::OK;
  E err_;
  std::string msg_;
};

}  // namespace putong
