add_executable(voyx)

file(GLOB_RECURSE
  HDR "${CMAKE_CURRENT_LIST_DIR}/*.h"
  CPP "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

source_group(
  TREE "${CMAKE_CURRENT_LIST_DIR}"
  FILES ${HDR} ${CPP}
  PREFIX voyx)

target_sources(voyx
  PRIVATE ${HDR} ${CPP})

target_include_directories(voyx
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}/..")

target_link_libraries(voyx
  PRIVATE cxxopts
          dr
          easyloggingpp
          fmt
          mlinterp
          pocketfft
          qcustomplot
          qt
          readerwriterqueue
          rtaudio
          rtmidi
          sdft
          xtensor
          xtl)

target_compile_features(voyx
  PRIVATE cxx_std_20)

if (MSVC)

  target_compile_options(voyx
    PRIVATE /fp:fast)

else()

  target_compile_options(voyx
    PRIVATE -ffast-math)

endif()

if (UNIX)

  target_link_libraries(voyx
    PRIVATE pthread)

endif()

if (UI)

  target_compile_definitions(voyx
    PRIVATE VOYXUI)

endif()
