# https://github.com/xtensor-stack/xtl

CPMAddPackage(
  NAME xtl
  VERSION 0.7.4
  GIT_TAG 0.7.4
  GITHUB_REPOSITORY xtensor-stack/xtl
  DOWNLOAD_ONLY YES)

if(xtl_ADDED)

  add_library(xtl INTERFACE)

  target_include_directories(xtl
    INTERFACE "${xtl_SOURCE_DIR}/include")

  target_compile_features(xtl
    INTERFACE cxx_std_14)

endif()
