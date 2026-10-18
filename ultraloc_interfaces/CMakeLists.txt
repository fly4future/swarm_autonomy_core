cmake_minimum_required(VERSION 3.16)
project(ultraloc_interfaces)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  msg/Bearing.msg
  msg/Range.msg
  msg/RelativePosition.msg
  msg/RelativePositionArray.msg
  DEPENDENCIES std_msgs geometry_msgs
)

# header-only C++ interfaces
add_library(${PROJECT_NAME}_cpp INTERFACE)
target_include_directories(${PROJECT_NAME}_cpp INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(${PROJECT_NAME}_cpp INTERFACE Eigen3::Eigen)

install(DIRECTORY include/ DESTINATION include)
install(TARGETS ${PROJECT_NAME}_cpp EXPORT export_${PROJECT_NAME}_cpp)

ament_export_targets(export_${PROJECT_NAME}_cpp)
ament_export_dependencies(rosidl_default_runtime Eigen3)

ament_package()
