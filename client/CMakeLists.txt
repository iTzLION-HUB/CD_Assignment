cmake_minimum_required(VERSION 3.10)
project(RestApiClient)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Find libcurl - Windows/vcpkg compatible approach
if(WIN32)
    # For Windows with vcpkg
    find_package(CURL REQUIRED)
    set(CURL_LIBRARIES CURL::libcurl)
else()
    # For Linux/macOS
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CURL REQUIRED libcurl)
endif()

# Create executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/ApiClient.cpp
)

# Link libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${CURL_INCLUDE_DIRS})
    target_compile_options(${PROJECT_NAME} PRIVATE ${CURL_CFLAGS_OTHER})
endif()

# Windows specific settings
if(WIN32)
    # Add Windows socket libraries
    target_link_libraries(${PROJECT_NAME} ws2_32 wldap32 crypt32 normaliz)
endif()