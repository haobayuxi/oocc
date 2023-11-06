add_executable(retwis_backend backend.cpp)
target_link_libraries(retwis_backend PUBLIC dtx_retwis)

add_executable(retwis_bench bench.cpp)
target_link_libraries(retwis_bench PUBLIC dtx_retwis)
