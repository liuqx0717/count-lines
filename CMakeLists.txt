cmake_minimum_required(VERSION 3.15)

project(count-lines
    VERSION 1.0.0
)

include(CTest)

set(CMAKE_CXX_STANDARD 14) 



set(TCLAP_INCLUDE_DIR ""
    CACHE PATH "Include directory of TCLAP (absolute path)")
set(ENABLE_ASAN $<OR:$<NOT:$<BOOL:${MSVC}>>,$<VERSION_GREATER:${MSVC_VERSION},1928>>
    CACHE BOOL "Enable address sanitizer for tests")
set(STATIC_LINK ${MSVC}
    CACHE BOOL "Statically link all the dependencies")



if (${MSVC})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /wd4100")
    if (${STATIC_LINK})
        # Use statically-linked runtime library
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
    endif()
    set(ASAN_COMPILE_FLAGS "/fsanitize=address")
    set(ASAN_LINK_FLAGS "")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-maybe-uninitialized")
    if (${STATIC_LINK})
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    else()
        # Cannot specify `-static` with `-fsanitize=address`
        set(ASAN_COMPILE_FLAGS "-fsanitize=address")
        set(ASAN_LINK_FLAGS "-fsanitize=address")
    endif()
    set(ASAN_COMPILE_FLAGS "${ASAN_COMPILE_FLAGS};-fsanitize=undefined;-fno-omit-frame-pointer")
    set(ASAN_LINK_FLAGS "${ASAN_LINK_FLAGS};-fsanitize=undefined")
endif()



add_library(tclap INTERFACE)
target_include_directories(tclap BEFORE
    INTERFACE ${TCLAP_INCLUDE_DIR}
)

add_library(asan INTERFACE)
target_compile_options(asan 
    INTERFACE ${ASAN_COMPILE_FLAGS}
)
target_link_options(asan
    INTERFACE ${ASAN_LINK_FLAGS}
)



set(COMMON_SOURCE
    src/common.cpp
    src/count-lines.cpp
)

add_executable(count-lines
    ${COMMON_SOURCE}
    src/main.cpp
)
target_compile_definitions(count-lines
    PRIVATE VERSION="${PROJECT_VERSION}"
)
target_link_libraries(count-lines
    PRIVATE tclap
)

add_executable(unit-test
    ${COMMON_SOURCE}
    src/unit-test.cpp
) 
target_link_libraries(unit-test
    PRIVATE $<${ENABLE_ASAN}:asan>
)



add_test(
    NAME unit-test
    COMMAND unit-test
)

