# ---------------------------------------------------------------------------
# IMLAB
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Files
# ---------------------------------------------------------------------------

file(GLOB_RECURSE INCLUDE_H "tools/queryc/gen/*.h")
file(GLOB_RECURSE SRC_CC_GEN "tools/queryc/gen/*.cc")

# ---------------------------------------------------------------------------
# Compiler
# ---------------------------------------------------------------------------

add_executable(queryc "${CMAKE_SOURCE_DIR}/tools/queryc/queryc.cc" ${INCLUDE_H})
add_library(imlab_queryc STATIC ${SRC_CC_GEN})
#target_link_libraries(queryc imlab imlab_queryc gflags Threads::Threads)
target_link_libraries(queryc imlab gflags Threads::Threads)

# ---------------------------------------------------------------------------
# Linting
# ---------------------------------------------------------------------------

add_cpplint_target(lint_queryc "${CMAKE_SOURCE_DIR}/tools/queryc/queryc.cc")
list(APPEND lint_targets lint_queryc)

