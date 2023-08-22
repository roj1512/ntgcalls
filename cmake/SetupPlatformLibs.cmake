set(cmake_dir ${CMAKE_SOURCE_DIR}/cmake)
function(setup_platform_libs target_name)
    if (WIN32)
        include(${cmake_dir}/Windows.cmake)
    elseif(LINUX)
        include(${cmake_dir}/Linux.cmake)
    elseif(APPLE)
        include(${cmake_dir}/macOS.cmake)
    else()
        message(FATAL_ERROR "Your OS is not supported yet")
    endif()
endfunction()