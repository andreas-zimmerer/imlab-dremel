# ---------------------------------------------------------------------------
# IMLAB
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Benchmarks
# ---------------------------------------------------------------------------

add_executable(dremel_benchmark bench/dremel_benchmark.cc)

target_link_libraries(dremel_benchmark imlab tbb benchmark gtest gmock Threads::Threads)
