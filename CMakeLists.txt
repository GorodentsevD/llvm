cmake_minimum_required(VERSION 3.13)
project(learn-llvm)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wregister")

find_package(FLEX)
find_package(BISON)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "LLVM Version: ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader)

bison_target(MyParser src/bison.ypp ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp)
flex_target(MyLexer src/flex.lex ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
add_flex_bison_dependency(MyLexer MyParser)

add_executable(${PROJECT_NAME}
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyLexer_OUTPUTS}
        main.cpp
        src/astTree/astTree.cpp src/astTree/astTree.hpp
        src/statement/statement.cpp
        src/statement/statement.hpp
        src/alloc/alloc.cpp
        src/alloc/alloc.hpp
        src/expression/expr.hpp src/expression/literalExpr.cpp src/expression/idExrp.cpp src/expression/unaryExpr.cpp src/expression/error.cpp src/expression/binaryExpr.cpp src/expression/funcCallExpr.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/alloc)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/astTree)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/expression)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/statement)

target_link_libraries(learn-llvm ${llvm_libs})
target_compile_options(learn-llvm PRIVATE -Wno-deprecated-register)