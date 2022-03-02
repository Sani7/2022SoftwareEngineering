set(srcs "Epaper3i7.cpp"
    )

set(EXTRA_COMPONENT_DIRS "components/")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ".")
