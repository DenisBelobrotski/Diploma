cmake_minimum_required (VERSION 3.8)

project (Diploma)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /arch:AVX /GF /GL /Gw /Gy /Qpar /Qpar-report:1 /Qvec-report:1")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

set(LIBS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs)

set(GNUPLOT_WRAPPER_ROOT ${LIBS_ROOT}/GNUPlotWrapper)
set(MAGNETIC_FLUID_FORM_ALGORITHM_ROOT ${LIBS_ROOT}/MagneticFluidFormAlgorithm)

include_directories(
		${GNUPLOT_WRAPPER_ROOT}/include
		${MAGNETIC_FLUID_FORM_ALGORITHM_ROOT}/include)

add_subdirectory(${GNUPLOT_WRAPPER_ROOT})
add_subdirectory(${MAGNETIC_FLUID_FORM_ALGORITHM_ROOT})

file(GLOB_RECURSE proj_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable (Diploma ${proj_sources})

target_link_libraries(Diploma GNUPlotWrapper MagneticFluidFormAlgorithm)
