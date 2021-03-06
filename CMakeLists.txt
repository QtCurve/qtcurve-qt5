project(QtCurve-Qt5)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}"
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

set(QTCURVE_VERSION_MAJOR "1")
set(QTCURVE_VERSION_MINOR "8")
set(QTCURVE_VERSION_PATCH "15")
set(QTCURVE_VERSION "${QTCURVE_VERSION_MAJOR}.${QTCURVE_VERSION_MINOR}")
set(QTCURVE_VERSION_FULL "${QTCURVE_VERSION}.${QTCURVE_VERSION_PATCH}")

option(ENABLE_QT5 "Building Qt5 theme." On)
# option(QTC_QT5_ENABLE_KDE "Building Qt5 style with KDE5 support." Off)
option(QTC_ENABLE_X11 "Enable X11" On)
option(QTC_QT5_OLD_NVIDIA_ARROW_FIX "Qt5 old nvidia arrow fix." Off)
option(QTC_QT5_ENABLE_PARENTLESS_DIALOG_FIX_SUPPORT
  "Qt5 support for the 'fix parentless dialogs'." Off)

# Only try to figure out what kde prefix is if not specified at command line.
if(NOT DEFINED QTC_KDE4_PREFIX)
  find_package(KDE4)
  find_program(KDE4_KDECONFIG_EXECUTABLE NAMES kde4-config
    PATHS "${CMAKE_INSTALL_PREFIX}/bin" ${_KDEDIRS} /opt/kde4/bin
    NO_DEFAULT_PATH)
  if(NOT KDE4_KDECONFIG_EXECUTABLE)
    find_program(KDE4_KDECONFIG_EXECUTABLE NAMES kde4-config)
  endif()
  if(KDE4_KDECONFIG_EXECUTABLE)
    execute_process(COMMAND "${KDE4_KDECONFIG_EXECUTABLE}" --prefix
      OUTPUT_VARIABLE QTC_KDE4_PREFIX)
    string(STRIP "${QTC_KDE4_PREFIX}" QTC_KDE4_PREFIX)
  else()
    set(QTC_KDE4_PREFIX "${CMAKE_INSTALL_PREFIX}")
  endif()
endif()

configure_file(config.h.in config.h @ONLY)

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}")

add_subdirectory(qt5)

if(QTC_QT5_ENABLE_PARENTLESS_DIALOG_FIX_SUPPORT)
  message(WARNING
    "You have enabled support for the 'fix parentless dialogs' option for Qt5. "
    "This is known to BREAK some applications. Please "
    "DO NOT report errors to application authors when you have "
    "this enabled. Please reconsider DISABLING this option.")
endif()
