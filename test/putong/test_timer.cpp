// Copyright 2018-2019 Delft University of Technology
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

#include <gmock/gmock.h>
#include <chrono>
#include <thread>

#include "putong/timer.h"

namespace putong {

TEST(Timer, Split) {
  using namespace std::chrono_literals;
  SplitTimer<3> t;

  t.Start();
  std::this_thread::sleep_for(50ms);
  t.Split();
  std::this_thread::sleep_for(50ms);
  t.Split();
  std::this_thread::sleep_for(50ms);
  t.Split();

  for (const auto &split : t.seconds()) {
    ASSERT_TRUE(split > 0.04 && split < 0.06);
  }

  t.report(std::cout);
}

}
