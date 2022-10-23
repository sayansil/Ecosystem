set(SOURCE_LIST
    ecosystem.cpp
    )

FetchContent_Declare(
    pybind11
    GIT_REPOSITORY "https://github.com/pybind/pybind11"
    GIT_TAG f61855b9d8821e2576960109a2a67379a6c2366f
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(pybind11)

set(PYBIND11_CPP_STANDARD -std=c++17)
pybind11_add_module(${ECOSYSTEM_PYTHON_MODULE} ${SOURCE_LIST})
target_include_directories(${ECOSYSTEM_PYTHON_MODULE} PRIVATE 
                           ${Ecosystem_SOURCE_DIR}/include)
target_link_libraries(${ECOSYSTEM_PYTHON_MODULE} PRIVATE ${ECOSYSTEM_LIB} dl)
