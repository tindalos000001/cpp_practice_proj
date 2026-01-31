# 添加静态库
function(add_static_lib TARGET_NAME)
    set(options)
    set(one_value_args)
    set(multi_value_args FILES LIBS PUBLIC_INCLUDES PRIVATE_INCLUDES EXPORT_HEADERS EXPORT_DIRECTORY BIN_DIR)
    # 使用cmake的cmake_pasrse_arguments方法解析参数
    cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
    if (ARG_UNPARSED_ARGUMENTS)
        message(
                SEND_ERROR
                "Error: unrecognized arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif ()
    # 定义库Target，声明类型为静态库
    add_library(${TARGET_NAME} STATIC "")
    set(TARGET_PRIVATE_INCLUDES "")
    set(TARGET_PUBLIC_INCLUDES "")
    # 定义库的include路径
    if (DEFINED ARG_PRIVATE_INCLUDES)
        list(APPEND TARGET_PRIVATE_INCLUDES ${ARG_PRIVATE_INCLUDES})
    endif()
    if (DEFINED ARG_PUBLIC_INCLUDES)
        list(APPEND TARGET_PUBLIC_INCLUDES ${ARG_PUBLIC_INCLUDES})
    endif()
    target_include_directories(
        ${TARGET_NAME}
        PUBLIC ${TARGET_PUBLIC_INCLUDES}
        PRIVATE ${TARGET_PRIVATE_INCLUDES}
        ${PROJECT_SOURCE_DIR}/Source
        ${CMAKE_CURRENT_SOURCE_DIR}/
    )
    # 如果有要导出的头文件则设置PUBLIC_HEADER属性
    if (DEFINED ARG_EXPORT_HEADERS)
        set_target_properties(
            ${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${ARG_EXPORT_HEADERS}"
        )
    endif()
    # 定义输出的二进制文件目录
    if (DEFINED ARG_BIN_DIR)
        set_target_properties(
            ${TARGET_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${ARG_BIN_DIR})
        set_target_properties(
            ${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${ARG_BIN_DIR})
        set_target_properties(
            ${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${ARG_BIN_DIR})
    endif()
    # 链接库和源文件
    target_sources(${TARGET_NAME} PRIVATE ${ARG_FILES})
    target_link_libraries(${TARGET_NAME} PUBLIC ${ARG_LIBS})
    # 定义install时的导出行为,默认使用TARGET_NAME作为导出目录
    if (DEFINED ARG_EXPORT_DIRECTORY)
        install(
            TARGETS ${TARGET_NAME}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${ARG_EXPORT_DIRECTORY})
    else()
         install(
            TARGETS ${TARGET_NAME}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${TARGET_NAME})
    endif()
    # 如果开启了clang-tidy检查则给编译目标设置CLANG_TIDY检查属性
    if (WITH_CLANG_TIDY)
        set_target_properties(${TARGET_NAME} PROPERTIES CXX_CLANG_TIDY
                "clang-tidy;-checks=*")
    endif ()

endfunction()




