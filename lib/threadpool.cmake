# https://github.com/bshoshany/thread-pool

CPMAddPackage(
  NAME threadpool
  VERSION 3.3.0
  GITHUB_REPOSITORY bshoshany/thread-pool
  DOWNLOAD_ONLY YES)

if(threadpool_ADDED)

  add_library(threadpool INTERFACE)

  target_include_directories(threadpool
    INTERFACE "${threadpool_SOURCE_DIR}")

  if (UNIX)
    target_link_libraries(threadpool
      INTERFACE pthread)
  endif()

endif()
