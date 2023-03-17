add_library(KF6MoreTools)
add_library(KF6::MoreTools ALIAS KF6MoreTools)

target_sources(KF6MoreTools PRIVATE
    kmoretools.cpp
    kmoretoolsconfigdialog_p.cpp
    kmoretoolsmenufactory.cpp
    kmoretoolspresets.cpp
)
ki18n_wrap_ui(KF6MoreTools ui/kmoretoolsconfigwidget.ui)

ecm_qt_declare_logging_category(KF6MoreTools
    HEADER kmoretools_debug.h
    IDENTIFIER KMORETOOLS
    CATEGORY_NAME kf.moretools
    DESCRIPTION "KMoreTools"
    EXPORT KMORETOOLS
)

ecm_generate_export_header(KF6MoreTools
    EXPORT_FILE_NAME kmoretools_export.h
    BASE_NAME KMoreTools
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    DEPRECATED_BASE_VERSION 0
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
    DEPRECATION_VERSIONS
)

ecm_generate_headers(KMoreTools_CamelCase_HEADERS
    HEADER_NAMES
    KMoreTools
    KMoreToolsMenuFactory
    KMoreToolsPresets

    REQUIRED_HEADERS KMoreTools_HEADERS
)
install(
    FILES
        ${KMoreTools_CamelCase_HEADERS}
        ${KMoreTools_HEADERS}
        ${CMAKE_CURRENT_BINARY_DIR}/kmoretools_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KMoreTools
    COMPONENT Devel
)
target_include_directories(KF6MoreTools
    INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KMoreTools>"
)

target_link_libraries(KF6MoreTools
  PUBLIC
    KF6::Service
    Qt6::Widgets
  PRIVATE
    KF6::KIOCore               # For network fetching operations
    KF6::KIOWidgets            # For KUrlRequester
    KF6::I18n                  # For translations
    KF6::ConfigCore            # KAuthorized
    Qt6::Qml
    Qt6::Quick
)

set_target_properties(KF6MoreTools PROPERTIES
   VERSION ${KMORETOOLS_VERSION}
   SOVERSION ${KMORETOOLS_SOVERSION}
   EXPORT_NAME MoreTools
)

install(TARGETS KF6MoreTools EXPORT KF6MoreToolsTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

if(BUILD_QCH)
    ecm_add_qch(
        KF6MoreTools_QCH
        NAME KMoreTools
        BASE_NAME KF6MoreTools
        VERSION ${KF_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${KMoreTools_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            KF6Service_QCH
            Qt6Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
            ${KMoreTools_BINARY_DIR}
        BLANK_MACROS
            KMORETOOLS_EXPORT
            KMORETOOLS_DEPRECATED
            KMORETOOLS_DEPRECATED_EXPORT
            "KMORETOOLS_DEPRECATED_VERSION(x, y, t)"
            "KMORETOOLS_DEPRECATED_VERSION_BELATED(x, y, xt, yt, t)"
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
