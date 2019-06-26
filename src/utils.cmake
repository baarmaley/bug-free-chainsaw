# Добавить POST_BUILD команды для копирования библиотек qt в папку с .exe
function(add_windeploy_to_target qt_target)
    if(WIN32)
        # Run winddeployqt if it can be found
        find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS ${QTDIR} ENV QTDIR PATH_SUFFIXES bin)
        get_target_property(CURRENT_TARGET_SOURCE_DIR ${qt_target} SOURCE_DIR)
        add_custom_command(TARGET ${qt_target} POST_BUILD COMMAND ${WINDEPLOYQT_EXECUTABLE} --qmldir ${CURRENT_TARGET_SOURCE_DIR} $<TARGET_FILE:${qt_target}>)
    endif()
endfunction(add_windeploy_to_target)