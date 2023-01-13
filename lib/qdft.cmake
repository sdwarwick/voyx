# https://github.com/jurihock/qdft

CPMAddPackage(
  NAME qdft
  VERSION 0.2
  GIT_TAG e257d4a93edfd488b3fa7307df9e408d6946be09
  GITHUB_REPOSITORY jurihock/qdft
  DOWNLOAD_ONLY YES)

if(qdft_ADDED)

  add_library(qdft INTERFACE)

  target_include_directories(qdft
    INTERFACE "${qdft_SOURCE_DIR}/src/cpp")

endif()
