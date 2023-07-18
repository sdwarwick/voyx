# https://github.com/sdwarwick/pocketfft

# GIT_TAG 8645d53873bf8258840feb00ad5bd8174b9daf97
# VERSION 2023.7.16

CPMAddPackage(
  NAME pocketfft
  VERSION V1.0
  GIT_REPOSITORY https://github.com/sdwarwick/pocketfft
  DOWNLOAD_ONLY YES)

if(pocketfft_ADDED)

  add_library(pocketfft INTERFACE)

  target_include_directories(pocketfft
    INTERFACE "${pocketfft_SOURCE_DIR}")

  target_compile_definitions(pocketfft
    INTERFACE -DPOCKETFFT_NO_MULTITHREADING)

  target_compile_definitions(pocketfft
    INTERFACE -DPOCKETFFT_CACHE_SIZE=10)

  if (UNIX)
    target_link_libraries(pocketfft
      INTERFACE pthread)
  endif()

endif()
