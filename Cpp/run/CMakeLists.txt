set(SOURCE_LIST
    main.cpp
    ../extern/imgui/imgui.cpp
    ../extern/imgui/imgui_demo.cpp
    ../extern/imgui/imgui_draw.cpp
    ../extern/imgui/imgui_tables.cpp
    ../extern/imgui/imgui_widgets.cpp
    ../extern/imgui/backends/imgui_impl_glfw.cpp
    ../extern/imgui/backends/imgui_impl_opengl3.cpp
    )

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${ECOSYSTEM_EXE} ${SOURCE_LIST})
target_include_directories(${ECOSYSTEM_EXE} PRIVATE 
                           ${Ecosystem_SOURCE_DIR}/include 
                           ${Ecosystem_SOURCE_DIR}/extern/nlohmann/include
                           ${Ecosystem_SOURCE_DIR}/extern/imgui
                           )

target_link_libraries(${ECOSYSTEM_EXE} PRIVATE ${ECOSYSTEM_LIB} dl glfw OpenGL::GL GLEW::GLEW)
