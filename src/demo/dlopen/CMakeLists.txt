# bridge shared lib
add_library(bridge SHARED
    bridge.cpp
)

# shared lib
add_library(sample_app SHARED
    sample_app.cpp
)
target_link_libraries(sample_app PRIVATE bridge)

# executable demo
add_executable(demo_dlopen
    main.cpp
)
target_link_libraries(demo_dlopen PRIVATE bridge sample_app)