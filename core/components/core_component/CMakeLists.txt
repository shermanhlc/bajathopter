set(SOURCES
    src/fart.c
    src/EspIDFComms.cpp
)

idf_component_register(SRCS "src/fart.c"
                    INCLUDE_DIRS "inc"
                    REQUIRES esp_wifi)