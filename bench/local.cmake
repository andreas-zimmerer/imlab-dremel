# ---------------------------------------------------------------------------
# IMLAB
# ---------------------------------------------------------------------------

add_executable(tpcc_5w_neworder bench/tpcc_5w_neworder.cc)
target_link_libraries(tpcc_5w_neworder imlab benchmark gtest gmock Threads::Threads)
