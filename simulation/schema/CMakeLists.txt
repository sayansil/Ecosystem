set(SCHEMAS
    world
    )

set(VIS_SCHEMAS
    species_report
    )

foreach(SCHEMA ${SCHEMAS})
    execute_process(COMMAND ${FLATC} --cpp -o ${CMAKE_CURRENT_SOURCE_DIR}/cpp
        --gen-mutable --reflect-names --gen-object-api --cpp-std c++17 ${CMAKE_CURRENT_SOURCE_DIR}/${SCHEMA}.fbs)
    execute_process(COMMAND ${FLATC} --python -o ${CMAKE_CURRENT_SOURCE_DIR}/python
        --gen-object-api ${CMAKE_CURRENT_SOURCE_DIR}/${SCHEMA}.fbs)
endforeach()

foreach(SCHEMA ${VIS_SCHEMAS})
    execute_process(COMMAND ${FLATC} --python -o ${CMAKE_CURRENT_SOURCE_DIR}/python
        --gen-object-api ${CMAKE_CURRENT_SOURCE_DIR}/${SCHEMA}.fbs)
endforeach()
