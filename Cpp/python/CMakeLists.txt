set(SOURCE_LIST
    ${Ecosystem_SOURCE_DIR}/python/ecosystem.cpp
    )

add_subdirectory(pybind11)
set(PYBIND11_CPP_STANDARD -std=c++17)
pybind11_add_module(${ECOSYSTEM_PYTHON_MODULE} ${SOURCE_LIST})
target_include_directories(${ECOSYSTEM_PYTHON_MODULE} PRIVATE ${Ecosystem_SOURCE_DIR}/include)
target_link_libraries(${ECOSYSTEM_PYTHON_MODULE} PRIVATE ${ECOSYSTEM_LIB} dl stdc++fs)
