MESSAGE( STATUS ">> -------------------------------------------------------------------- <<" )
MESSAGE( STATUS ">> ----------------------- FMath -------------------------------------- <<" )


######### CMake Version ###############################################
cmake_minimum_required( VERSION 3.10 FATAL_ERROR )
### We need at least C++17
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
### Distinction between Clang and AppleClang
cmake_policy( SET CMP0025 NEW )
#######################################################################


#######################################################################
# SET( FMATH_USE_CUDA          OFF  CACHE BOOL "Use CUDA to speed up certain parts of the code." )
SET( FMATH_USE_OPENMP        OFF  CACHE BOOL "Use OpenMP to speed up certain parts of the code." )
SET( FMATH_TEST_COVERAGE     OFF  CACHE BOOL "Build FMath tests with coverage flags." )
#######################################################################


### Set the cmake subdirectory
# list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" )


### Set a default build type in case none is passed
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


# ######### Get git revision ############################################
# include( GetGitRevisionDescription )
# get_git_head_revision( GIT_REFSPEC GIT_SHA1 )
# string( SUBSTRING "${GIT_SHA1}" 0 12 GIT_REV )
# #######################################################################


######### Meta information about the project ##########################
set( META_PROJECT_NAME         "FMath" )
set( META_PROJECT_DESCRIPTION  "Efficient and expressive use of abstract Fields" )
set( META_AUTHOR_ORGANIZATION  "" )
set( META_AUTHOR_DOMAIN        "https://github.com/trick-17/fmath" )
set( META_AUTHOR_MAINTAINER    "" )
set( META_VERSION_MAJOR        "0" )
set( META_VERSION_MINOR        "0" )
set( META_VERSION_PATCH        "0" )
set( META_VERSION              "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}" )
# set( META_VERSION_REVISION     "${GIT_REV}" )
### Propagate version and name_version upwards
set( FMATH_META_VERSION        "${META_VERSION}" PARENT_SCOPE )
set( FMATH_META_NAME_VERSION   "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})" PARENT_SCOPE )
### Project Name
project( ${META_PROJECT_NAME} VERSION ${META_VERSION} )
#######################################################################


# ######### CUDA decisions ##############################################
# if ( FMATH_USE_CUDA )
#     find_package( CUDA REQUIRED )
#     add_definitions( -DUSE_CUDA )
#   # set (CUDA_PROPAGATE_HOST_FLAGS ON)
#   # --std=c++17 flag may be necessary, but it is propagated from global flags...
#   # if it appears twice, for some reason the compilation breaks
#     set( CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -arch=sm_60 --expt-relaxed-constexpr" )
# endif()
# #######################################################################


#######################################################################
if( FMATH_USE_OPENMP )
    include( FindOpenMP )
    if( OPENMP_FOUND )
        set( CMAKE_C_FLAGS          "${CMAKE_C_FLAGS}          ${OpenMP_C_FLAGS}" )
        set( CMAKE_CXX_FLAGS        "${CMAKE_CXX_FLAGS}        ${OpenMP_CXX_FLAGS}" )
        set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}" )
        message( STATUS ">> OpenMP found." )
        message( STATUS ">> OpenMP C Flags:          ${OpenMP_C_FLAGS}" )
        message( STATUS ">> OpenMP CXX Flags:        ${OpenMP_CXX_FLAGS}" )
        message( STATUS ">> OpenMP EXE Linker Flags: ${OpenMP_EXE_LINKER_FLAGS}" )
    else( OPENMP_FOUND )
        message( STATUS ">> WARNING: OpenMP could not be found." )
    endif( OPENMP_FOUND )
endif( )
#######################################################################


######### Extra flags for Eigen #######################################
SET(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE}        -DEIGEN_NO_DEBUG -DEIGEN_FAST_MATH=0")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DEIGEN_FAST_MATH=0")
SET(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG}          -DEIGEN_FAST_MATH=0")
#######################################################################


######## Compiler-specific flags #####################################
### GNU Compiler Collection - gcc
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
    ### Require at least gcc 7.0
    if( CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7 )
        message( FATAL_ERROR "GCC version must be at least 7!" )
    endif( )
    ### Compile and Link Flags (we deactivate -Wint-in-bool-context because of Eigen)
    set( FMATH_COMPILE_FLAGS "-Wall -Wextra -Wpedantic -Wno-int-in-bool-context" )
    set( FMATH_LINK_FLAGS    "" )
    set( FMATH_BENCH_COMPILE_FLAGS "-Wno-unused-variable" )
    ### We deactivate -Wunknown-pragmas in case OpenMP is not used
    if( NOT FMATH_USE_OPENMP )
        set( FMATH_COMPILE_FLAGS "${FMATH_COMPILE_FLAGS} -Wno-unknown-pragmas" )
    endif( )

### Apple Clang
elseif( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
    ### Compile and Link Flags
    set( FMATH_COMPILE_FLAGS "-Wall -Wextra -Wpedantic" )
    set( FMATH_LINK_FLAGS    "" )
    set( FMATH_BENCH_COMPILE_FLAGS "-Wno-unused-variable" )

### Microsoft Visual Compiler - msvc
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
    ### Compile and Link Flags
    set( FMATH_COMPILE_FLAGS "-DNOMINMAX" )
    set( FMATH_LINK_FLAGS    "" )
    set( FMATH_BENCH_COMPILE_FLAGS "" )

### Intel Compiler - icc
elseif( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel" )
    ### Compile and Link Flags
    set( FMATH_COMPILE_FLAGS "-Wall -Wextra -Wpedantic" )
    set( FMATH_LINK_FLAGS    "" )
    set( FMATH_BENCH_COMPILE_FLAGS "-Wno-unused-variable" )
endif( )
message( STATUS "FMath Compile Flags: ${FMATH_COMPILE_FLAGS}" )
message( STATUS "FMath Link Flags:    ${FMATH_LINK_FLAGS}" )
######################################################################


######### Coverage Flags #############################################
if( FMATH_TEST_COVERAGE )
    set( FMATH_FLAGS_COVERAGE
        ${CMAKE_CXX_FLAGS_DEBUG}
        --coverage
        -fno-inline
        -fno-inline-small-functions
        -fno-default-inline )
    string(REPLACE ";" " " FMATH_FLAGS_COVERAGE "${FMATH_FLAGS_COVERAGE}")
    message( STATUS "FMath Coverage Flags: ${FMATH_FLAGS_COVERAGE}" )
endif( )
######################################################################


######### Where to search for library headers #########################
set( FMATH_INCLUDE_DIRS
     ${PROJECT_SOURCE_DIR}/include
     ${PROJECT_SOURCE_DIR}/thirdparty )
include_directories( ${FMATH_INCLUDE_DIRS} )
#######################################################################


######### Declare File groups #########################################
set( HEADER_FMATH_ROOT
     ${CMAKE_CURRENT_SOURCE_DIR}/include/FMath/Core )
set( HEADER_FMATH_DETAIL
     ${CMAKE_CURRENT_SOURCE_DIR}/include/FMath/detail/Convenience.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/include/FMath/detail/Field.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/include/FMath/detail/FieldAssign.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/include/FMath/detail/FieldReductions.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/include/FMath/detail/Operators.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/include/FMath/detail/Expression_Add.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/include/FMath/detail/Expression_Mult.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/include/FMath/detail/Expression_Function.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/include/FMath/detail/Expression_SubSet.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/include/FMath/detail/Expression_Slice.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/include/FMath/detail/Using.hpp )
#######################################################################


######### IDE Folders #################################################
add_library( ${META_PROJECT_NAME} INTERFACE)
target_include_directories( ${META_PROJECT_NAME} INTERFACE ${FMATH_INCLUDE_DIRS} )
target_sources( ${META_PROJECT_NAME} INTERFACE ${HEADER_FMATH_ROOT} ${HEADER_FMATH_DETAIL} )

add_custom_target( ${META_PROJECT_NAME}_Headers SOURCES ${HEADER_FMATH_ROOT} ${HEADER_FMATH_DETAIL} )

### IDE folders
source_group( ""       FILES ${HEADER_FMATH_ROOT}   )
source_group( "detail" FILES ${HEADER_FMATH_DETAIL} )
#######################################################################


######### Test executables ############################################
set( TEST_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )
### Test creation macro
macro( add_framework_test testName testSrc )
    # Executable
    add_executable( test_${testName} test/main.cpp ${testSrc} )
    # Properties
    set_property( TARGET test_${testName} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${TEST_RUNTIME_OUTPUT_DIRECTORY} )
    set_property( TARGET test_${testName} PROPERTY CXX_STANDARD 17 )
    set_property( TARGET test_${testName} PROPERTY CXX_STANDARD_REQUIRED ON )
    set_property( TARGET test_${testName} PROPERTY CXX_EXTENSIONS OFF )
    set_property( TARGET test_${testName} PROPERTY COMPILE_FLAGS "${FMATH_COMPILE_FLAGS} ${FMATH_FLAGS_COVERAGE}" )
    set_property( TARGET test_${testName} PROPERTY LINK_FLAGS    "${FMATH_LINK_FLAGS}    ${FMATH_FLAGS_COVERAGE}" )
    # Include Directories
    target_include_directories( test_${testName} PRIVATE ${PROJECT_SOURCE_DIR}/test )
    # Add the test
    add_test( NAME        test_${testName}
              WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
              COMMAND           test_${testName} --use-colour=yes )
endmacro( add_framework_test testName testSrc )

### Enable CTest testing
enable_testing( )

### Create tests
add_framework_test( scalarfield test/scalarfield.cpp )
add_framework_test( vectorfield test/vectorfield.cpp )
add_framework_test( transform   test/transform.cpp   )
add_framework_test( reductions  test/reductions.cpp  )
add_framework_test( subset      test/indexing.cpp    )
#######################################################################


######### Benchmark executables #######################################
set( BENCH_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )
### Benchmark creation macro
macro( add_framework_bench benchName benchSrc )
    # Executable
    add_executable( bench_${benchName} ${benchSrc} )#WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} )
    # Properties
    set_property( TARGET bench_${benchName} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${BENCH_RUNTIME_OUTPUT_DIRECTORY} )
    set_property( TARGET bench_${benchName} PROPERTY CXX_STANDARD 17 )
    set_property( TARGET bench_${benchName} PROPERTY CXX_STANDARD_REQUIRED ON )
    set_property( TARGET bench_${benchName} PROPERTY CXX_EXTENSIONS OFF )
    set_property( TARGET bench_${benchName} PROPERTY COMPILE_FLAGS "${FMATH_COMPILE_FLAGS} ${FMATH_BENCH_COMPILE_FLAGS}" )
    set_property( TARGET bench_${benchName} PROPERTY LINK_FLAGS    "${FMATH_LINK_FLAGS}" )
    # Include Directories
    target_include_directories( bench_${benchName} PRIVATE ${PROJECT_SOURCE_DIR}/benchmark )
    # Add the bench
    # add_executable( NAME        ${benchName} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} )
endmacro( add_framework_bench benchName benchSrc )

### Create benchmarks
add_framework_bench( simple  benchmark/simple.cpp  )
add_framework_bench( lambdas benchmark/lambdas.cpp )
#######################################################################


######### Conan #######################################################
configure_file(conanfile.py.in ${CMAKE_CURRENT_SOURCE_DIR}/conanfile.py @ONLY)
#######################################################################


MESSAGE( STATUS ">> ----------------------- FMath done --------------------------------- <<" )
MESSAGE( STATUS ">> -------------------------------------------------------------------- <<" )