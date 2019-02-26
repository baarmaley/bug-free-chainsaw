# Добавить POST_BUILD команды для копирования библиотек qt в папку с .exe
function(add_windeploy_to_target qt_target)
    if(WIN32)
        # Run winddeployqt if it can be found
        find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS ${QTDIR} ENV QTDIR PATH_SUFFIXES bin)
        add_custom_command(TARGET ${qt_target} POST_BUILD COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${qt_target}>)
    endif()
endfunction(add_windeploy_to_target)