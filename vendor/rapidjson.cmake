# ---------------------------------------------------------------------------
# IMLAB
# ---------------------------------------------------------------------------

include(ExternalProject)
find_package(Git REQUIRED)

# Get rapidjson
ExternalProject_Add(
    rapidjson
    PREFIX "vendor/rapidjson"
    GIT_REPOSITORY "https://github.com/Tencent/rapidjson.git"
    GIT_TAG f54b0e47a08782a6131cc3d60f94d038fa6e0a51
    TIMEOUT 10
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

# Prepare rapidjson (rapidjson is a header-only library)
ExternalProject_Get_Property(rapidjson install_dir)
set(RAPIDJSON_INCLUDE_DIR ${install_dir}/include)
