file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(gamma-algorithm Model/Graph.cpp main.cpp)
#add_executable(TestGamma Model/Graph.cpp tests/TestGammaAlgorithm.cpp)
#
#target_link_libraries(TestGamma PRIVATE Catch2::Catch2WithMain)
