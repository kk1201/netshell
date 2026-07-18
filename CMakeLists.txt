cmake_minimum_required(VERSION 3.19)
project(netshell)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories("${CMAKE_SOURCE_DIR}/include")

set(COMMON_DEFINITIONS
	RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/"
)

if (WIN32)
	list(APPEND PLATFORM_SOURCES
	"src/win32/win32_common.c"
	)
else ()
	list(APPEND PLATFORM_SOURCES
	"src/unix/unix_common.c"
	)
endif()

function(add_target name source)
	add_executable(${name})

	target_sources(${name} PRIVATE ${source} ${PLATFORM_SOURCES})
	target_compile_definitions(${name} PUBLIC ${COMMON_DEFINITIONS})
endfunction()

add_target("${CMAKE_PROJECT_NAME}-client"
	"src/client.c"
)
add_target("${CMAKE_PROJECT_NAME}-server"
	"src/server.c"
)