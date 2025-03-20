add_executable(graph Graph/Graph.cpp main.cpp)
add_executable(TestGraph Graph/Graph.cpp Graph/test.cpp)

target_link_libraries(TestGraph PRIVATE Catch2::Catch2WithMain)