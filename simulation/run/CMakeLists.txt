FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui"
    GIT_TAG c881667c00655c98dba41deb942587e0041d0ed0
    GIT_PROGRESS TRUE
)
FetchContent_Populate(imgui)

FetchContent_Declare(
    implot
    GIT_REPOSITORY "https://github.com/epezent/implot"
    GIT_TAG 2dc2a4cfd5e3f0419b1f864cdaedba94ec8ef3a5
    GIT_PROGRESS TRUE
)
FetchContent_Populate(implot)

FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml
    GIT_TAG a91ba69f7bd21d0a9aeb8c87493af58271988dca
    GIT_PROGRESS TRUE
)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)
FetchContent_MakeAvailable(imgui-sfml)

set(SOURCE_LIST
    main.cpp
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
    )

find_package(GLEW REQUIRED)

add_executable(${ECOSYSTEM_EXE} ${SOURCE_LIST})
target_include_directories(${ECOSYSTEM_EXE} PRIVATE 
                           ${Ecosystem_SOURCE_DIR}/include 
                           ${imgui_SOURCE_DIR}
                           ${implot_SOURCE_DIR}
                           )

target_link_libraries(${ECOSYSTEM_EXE} PRIVATE ${ECOSYSTEM_LIB} pthread ImGui-SFML::ImGui-SFML)
