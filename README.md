# Assignment REST API

## Setup and Run
1. Install dependencies: `npm install`
2. Start server: `npm start`
3. Server will run on http://localhost:3000

## API Endpoints
- GET /users - Get all users
- POST /users - Add a new user (requires name and email in JSON body)
- PUT /users - Updates a user by ID.
- DELETE /users/:id - Delete a user by ID

# REST API C++ Client

## Prerequisites
- C++11 or later compiler
- libcurl development libraries
- CMake (version 3.10 or later)

## Installing libcurl
Windows (vcpkg):
- vcpkg install curl

## Build and Run

1. cd client
2. mkdir build && cd build
3. cmake .. 
   OR
3. cmake .. -DCMAKE_TOOLCHAIN_FILE="C:\vcpkg\scripts\buildsystems\vcpkg.cmake" -DCMAKE_PREFIX_PATH="C:\vcpkg\installed\x64-windows" -DVCPKG_TARGET_TRIPLET=x64-windows
4. cmake --build .
5. ./RestApiClient
Make sure the Node.js server is running on localhost:3000 before running the client.