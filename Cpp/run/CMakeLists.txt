FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui"
    GIT_TAG e7e170c534d27ae32f7b87d17295036ed9c9af81
    GIT_PROGRESS TRUE
)
FetchContent_Populate(imgui)

FetchContent_Declare(
    implot
    GIT_REPOSITORY "https://github.com/epezent/implot"
    GIT_TAG 555ff688a8134bc0c602123149abe9c17d577475
    GIT_PROGRESS TRUE
)
FetchContent_Populate(implot)

set(SOURCE_LIST
    main.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
    )

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${ECOSYSTEM_EXE} ${SOURCE_LIST})
target_include_directories(${ECOSYSTEM_EXE} PRIVATE 
                           ${Ecosystem_SOURCE_DIR}/include 
                           ${imgui_SOURCE_DIR}
                           ${implot_SOURCE_DIR}
                           )

target_link_libraries(${ECOSYSTEM_EXE} PRIVATE ${ECOSYSTEM_LIB} dl glfw OpenGL::GL GLEW::GLEW)
