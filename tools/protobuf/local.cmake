# ---------------------------------------------------------------------------
# IMLAB
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Files
# ---------------------------------------------------------------------------

file(GLOB_RECURSE INCLUDE_H "tools/protobuf/gen/*.h")
file(GLOB_RECURSE SRC_CC "tools/protobuf/gen/*.cc")

# ---------------------------------------------------------------------------
# Libraries
# ---------------------------------------------------------------------------

add_library(proto_schema STATIC ${SRC_CC})
target_compile_options(proto_schema PUBLIC "-fPIC")
target_link_libraries(proto_schema ${PROTOBUF_LIBRARY} gflags Threads::Threads)

# ---------------------------------------------------------------------------
# Linting
# ---------------------------------------------------------------------------

add_cpplint_target(lint_proto_schema "${INCLUDE_H}")
list(APPEND lint_targets lint_proto_schema)
