set(lib_name "common")
set(lib_bin_name "${lib_name}_lib")
set(src_file src/common.cpp )
set(export_header "include/common/template_util.h" "include/common/check_template.h" "include/common/concept_check.h")
add_library(${lib_bin_name} STATIC ${src_file})
target_include_directories(${lib_bin_name} PUBLIC include)
set_target_properties(${lib_bin_name} PROPERTIES OUTPUT_NAME "${lib_name}")
set_target_properties(${lib_bin_name} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#set_target_properties(${lib_bin_name} PROPERTIES PUBLIC_HEADER ${export_header})
install(FILES ${export_header} DESTINATION ${CMAKE_HEADER_OUTPUT_DIRECTORY}/${lib_name})
install(
    TARGETS ${lib_bin_name}  
    LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
