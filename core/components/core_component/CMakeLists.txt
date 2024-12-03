set(SOURCES
    src/fart.c
    src/EspIDFComms.cpp
)

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS "inc"
                    REQUIRES esp_wifi)