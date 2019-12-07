# ---------------------------------------------------------------------------
# IMLAB
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# TBB: Thread Building Blocks
# ---------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "../FindTBB.cmake")

find_package(TBB)

# ---------------------------------------------------------------------------
# Benchmarks
# ---------------------------------------------------------------------------

add_executable(dremel_benchmark bench/dremel_benchmark.cc)

target_link_libraries(dremel_benchmark imlab tbb benchmark gtest gmock Threads::Threads)
