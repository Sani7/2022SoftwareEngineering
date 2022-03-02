set(EXTRA_COMPONENT_DIRS src include lib)
set(srcs "epdif.c" "epdpaint.cpp" "font8.c" "font12.c" "font16.c" "font20.c" "font24.c")

if(CONFIG_EPD_1_54_V2)
    list(APPEND srcs "epd1in54_V2.c")
elseif(CONFIG_EPD_2_13_D)
    list(APPEND srcs "epd2in13d.c")
elseif(CONFIG_EPD_2_7)
    list(APPEND srcs "epd2in7.c")
elseif(CONFIG_EPD_3_7)
    list(APEND srcs "epd3in7.c")
endif()

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_common driver)
