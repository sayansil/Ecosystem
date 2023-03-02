function(copy_dll TARGET_NAME DLL_NAME)
	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:${DLL_NAME}> $<TARGET_FILE_DIR:${TARGET_NAME}>)
endfunction()

FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui"
    GIT_TAG f87e891f1818d02f55028fe042a323bb27c41d66
    GIT_PROGRESS TRUE
)
FetchContent_Populate(imgui)

FetchContent_Declare(
    implot
    GIT_REPOSITORY "https://github.com/epezent/implot"
    GIT_TAG 626e391670a4018a365787e35ef4cf01cb68ee31
    GIT_PROGRESS TRUE
)
FetchContent_Populate(implot)

set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
FetchContent_Declare(
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG 3.3.8
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(glfw)

if(NOT APPLE)
    find_package(OpenGL REQUIRED)
endif()

set(SOURCE_LIST
    simulation.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
    )

if(APPLE)
    set(BACKEND_LIST
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_metal.mm
    )
    list(APPEND SOURCE_LIST main_metal.mm ${BACKEND_LIST})
elseif(UNIX OR WIN32)
    set(BACKEND_LIST
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        )
    list(APPEND SOURCE_LIST main_opengl.cpp ${BACKEND_LIST})
endif()

add_executable(${ECOSYSTEM_EXE} ${SOURCE_LIST})
target_include_directories(${ECOSYSTEM_EXE} PRIVATE
                           ${Ecosystem_SOURCE_DIR}/include
                           ${imgui_SOURCE_DIR}
                           ${imgui_SOURCE_DIR}/backends
                           ${implot_SOURCE_DIR}
                           )

if(APPLE)
    target_link_libraries(
        ${ECOSYSTEM_EXE} PRIVATE ${ECOSYSTEM_LIB}
        pthread
        glfw
        fmt::fmt
        "-framework Metal"
        "-framework MetalKit"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework QuartzCore"
    )
elseif(UNIX)
    target_link_libraries(${ECOSYSTEM_EXE} PRIVATE ${ECOSYSTEM_LIB} pthread glfw OpenGL::GL fmt::fmt)
elseif(WIN32)
    target_link_libraries(${ECOSYSTEM_EXE} PRIVATE ${ECOSYSTEM_LIB} glfw OpenGL::GL fmt::fmt)
    copy_dll(${ECOSYSTEM_EXE} fmt)
	copy_dll(${ECOSYSTEM_EXE} ${ECOSYSTEM_LIB})
	copy_dll(${ECOSYSTEM_EXE} glfw)
endif()
