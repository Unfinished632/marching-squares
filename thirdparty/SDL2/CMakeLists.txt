message("-- Linking SDL")
add_library(SDL2 SDL2.dll)
set_target_properties(SDL2 PROPERTIES LINKER_LANGUAGE C)