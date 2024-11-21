set(SOURCES
    src/fart.c
)

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS "inc"
                    REQUIRES esp_wifi
                    )