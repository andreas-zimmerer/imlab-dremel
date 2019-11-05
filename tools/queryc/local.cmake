# ---------------------------------------------------------------------------
# IMLAB
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Compiler
# ---------------------------------------------------------------------------

add_executable(queryc "${CMAKE_SOURCE_DIR}/tools/queryc/queryc.cc" ${INCLUDE_H})
target_link_libraries(queryc imlab gflags Threads::Threads)

# ---------------------------------------------------------------------------
# Linting
# ---------------------------------------------------------------------------

add_cpplint_target(lint_queryc "${CMAKE_SOURCE_DIR}/tools/queryc/queryc.cc")
list(APPEND lint_targets lint_queryc)

