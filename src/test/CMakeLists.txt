if (WIN32)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif (WIN32)
include(FetchContent)
message(STATUS "--- fetching gtest")
FetchContent_Declare(
    googletest
    URL      file://${CMAKE_CURRENT_SOURCE_DIR}/googletest.v1.13.0.tar.gz
    URL_HASH SHA256=b7d3f5172c64c6a19ae1b9eca89a54f75f2214d5136024f666d0460191fad3da
)
FetchContent_MakeAvailable(googletest)

file(GLOB SOURCES_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
add_executable(${LIB_NAME}-tests ${SOURCES_TESTS})
target_link_libraries(${LIB_NAME}-tests PUBLIC
${LIB_NAME}
gtest
gtest_main
)
target_compile_definitions(${LIB_NAME}-tests PUBLIC
-D DATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/data"
)
