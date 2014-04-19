find_package(Qt4 4.8 REQUIRED QtCore QtGui QtDBus)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

QT4_ADD_RESOURCES(thermald_dock_RCCS thermald_dock.qrc)

set(thermald_dock_HDRS thermald-dock.hpp)
qt4_wrap_cpp(thermald_dock_MOC ${thermald_dock_HDRS})

set(thermald_dock_SRCS thermald-dock.cpp ${thermald_dock_RCCS})

add_executable(thermald-dock ${thermald_dock_MOC} ${thermald_dock_SRCS})
target_link_libraries(thermald-dock ${QT_LIBRARIES})