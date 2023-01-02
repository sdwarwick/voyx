# https://github.com/mackron/dr_libs

CPMAddPackage(
  NAME dr
  VERSION 0.13.7
  GIT_TAG 4f6da71ed357ade92dc91f00e6be7301ec9a82a3
  GITHUB_REPOSITORY mackron/dr_libs
  DOWNLOAD_ONLY YES)

if(dr_ADDED)

  add_library(dr INTERFACE)

  target_include_directories(dr
    INTERFACE "${dr_SOURCE_DIR}")

  target_compile_definitions(dr
    INTERFACE -DDR_WAV_IMPLEMENTATION)

endif()
