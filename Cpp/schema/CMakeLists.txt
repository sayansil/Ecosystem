set(SCHEMAS
    world
    )

foreach(SCHEMA ${SCHEMAS})
    execute_process(COMMAND ${FLATC} --cpp -o ${Ecosystem_SOURCE_DIR}/schema
        --gen-mutable --reflect-names --cpp-std c++17 ${Ecosystem_SOURCE_DIR}/schema/${SCHEMA}.fbs)
endforeach()
