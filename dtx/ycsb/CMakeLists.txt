add_executable(ycsb_backend backend.cpp)
target_link_libraries(ycsb_backend PUBLIC dtx_tatp)

add_executable(ycsb_bench bench.cpp)
target_link_libraries(ycsb_bench PUBLIC dtx_tatp)
