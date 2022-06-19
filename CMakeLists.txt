cmake_minimum_required(VERSION 3.21)
project(YMonotoneTriangulation)

include_directories(/opt/homebrew/Cellar)
link_directories(/opt/homebrew/lib/)
link_directories(/opt/homebrew/Cellar/)
#link_directories(opt/homebrew/include)

find_package(CGAL)
find_package(OpenMP)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6Quick REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Xpreprocessor -fopenmp -std=c++17 -fexceptions -Wall -Wextra -O2 -stdlib=libc++ -Wno-unused-parameter -Wno-macro-redefined")
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.2.2/lib/cmake/")
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.2.2/QtWidgets/")

add_executable(YMonotoneTriangulation main.cpp TriangulateYMonotone.h TriangulateYMonotoneModified.h Test.h MonotonePartition.h MonotonePartitionModified.h)

target_link_libraries(YMonotoneTriangulation libgmp.a)
target_link_libraries(YMonotoneTriangulation libmpfr.a)
target_link_libraries(YMonotoneTriangulation CGAL)
target_link_libraries(YMonotoneTriangulation libomp.a)
target_link_libraries(YMonotoneTriangulation Qt6::Core)
target_link_libraries(YMonotoneTriangulation Qt6::Widgets)
target_link_libraries(YMonotoneTriangulation Qt6::Quick)