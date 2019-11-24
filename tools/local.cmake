# ---------------------------------------------------------------------------
# IMLAB
# ---------------------------------------------------------------------------

include("${CMAKE_SOURCE_DIR}/tools/schemac/local.cmake")
include("${CMAKE_SOURCE_DIR}/tools/queryc/local.cmake")

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------

set(TOOLS_SRC
    "tools/imlabdb.cc"
)

# ---------------------------------------------------------------------------
# Executables
# ---------------------------------------------------------------------------

add_executable(imlabdb tools/imlabdb.cc)
target_link_libraries(imlabdb imlab imlab_queryc gflags Threads::Threads)

# ---------------------------------------------------------------------------
# Linting
# ---------------------------------------------------------------------------

add_cpplint_target(lint_tools "${TOOLS_SRC}")
list(APPEND lint_targets lint_tools)

