option(UI "Enable Qt6 based user interface" ON)

find_package(Qt6 COMPONENTS Core QUIET)

if (UI AND NOT Qt6_FOUND)

  message(WARNING "Continuing without user interface!")
  set(UI OFF CACHE BOOL "" FORCE)

endif()

add_library(qt INTERFACE)

if (UI)

  set(CMAKE_AUTOMOC ON)

  find_package(Qt6 REQUIRED COMPONENTS
    Core
    PrintSupport
    Widgets)

  target_link_libraries(qt INTERFACE
    Qt6::Core
    Qt6::PrintSupport
    Qt6::Widgets)

endif()
