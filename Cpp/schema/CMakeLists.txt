set(SCHEMAS
    world
    )

set(VIS_SCHEMAS
    species_report
    )

foreach(SCHEMA ${SCHEMAS})
    execute_process(COMMAND ${FLATC} --cpp -o ${CMAKE_CURRENT_SOURCE_DIR}
        --gen-mutable --reflect-names --cpp-std c++17 ${CMAKE_CURRENT_SOURCE_DIR}/${SCHEMA}.fbs)
endforeach()

foreach(SCHEMA ${VIS_SCHEMAS})
    execute_process(COMMAND ${FLATC} --cpp -o ${CMAKE_CURRENT_SOURCE_DIR}
        --gen-mutable --reflect-names --cpp-std c++17 ${CMAKE_CURRENT_SOURCE_DIR}/${SCHEMA}.fbs)
    execute_process(COMMAND ${FLATC} --dart -o ${CMAKE_CURRENT_SOURCE_DIR}
        --gen-object-api ${CMAKE_CURRENT_SOURCE_DIR}/${SCHEMA}.fbs)
endforeach()
