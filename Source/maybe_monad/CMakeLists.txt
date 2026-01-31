# set(lib_name "maybe_monad")
# set(lib_bin_name "${lib_name}_lib")
# set(src_file src/maybe_monad.cpp)
# set(export_header include/maybe_monad/maybe_monad.h)
# add_library("${lib_bin_name}" STATIC ${src_file})
# target_include_directories("${lib_bin_name}" PUBLIC include)
# target_link_libraries(${lib_bin_name} PUBLIC common_lib)
# set_target_properties(${lib_bin_name} PROPERTIES OUTPUT_NAME "${lib_name}")
# set_target_properties(${lib_bin_name} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
# set_target_properties(${lib_bin_name} PROPERTIES PUBLIC_HEADER ${export_header})
# install(
#     TARGETS ${lib_bin_name}  
#     LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
#     ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
#     RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#     PUBLIC_HEADER DESTINATION ${CMAKE_HEADER_OUTPUT_DIRECTORY}/${lib_name}
# )
set(files "src/maybe_monad.cpp")
set(export_headers "include/maybe_monad/maybe_monad.h")
set(public_include "include")
set(libs common_lib)
add_static_lib(maybe_monad_lib 
    FILES ${files}
    LIBS ${libs}
    PUBLIC_INCLUDES ${public_include})