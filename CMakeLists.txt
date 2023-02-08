# SPDX-FileCopyrightText: KDE Contributors
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.16)

set(KF_VERSION "5.240.0") # handled by release scripts
set(KF_DEP_VERSION "5.240.0") # handled by release scripts
project(KNewStuff VERSION ${KF_VERSION})

set(CMAKE_CXX_EXTENSIONS OFF)

include(FeatureSummary)
find_package(ECM 5.240.0 NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMQmlModule)
include(ECMQtDeclareLoggingCategory)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

set(REQUIRED_QT_VERSION 6.4.0)
find_package(Qt6 ${REQUIRED_QT_VERSION} NO_MODULE REQUIRED COMPONENTS Gui Widgets Xml)
find_package(Qt6 ${REQUIRED_QT_VERSION} NO_MODULE COMPONENTS Qml Quick)
find_package(KF6Archive ${KF_DEP_VERSION} REQUIRED)
find_package(KF6Completion ${KF_DEP_VERSION} REQUIRED)
find_package(KF6Config ${KF_DEP_VERSION} REQUIRED)
find_package(KF6CoreAddons ${KF_DEP_VERSION} REQUIRED)
find_package(KF6I18n ${KF_DEP_VERSION} REQUIRED)
find_package(KF6IconThemes ${KF_DEP_VERSION} REQUIRED)
find_package(KF6KIO ${KF_DEP_VERSION} REQUIRED)
find_package(KF6ItemViews ${KF_DEP_VERSION} REQUIRED)
find_package(KF6Package ${KF_DEP_VERSION} REQUIRED)
find_package(KF6Service ${KF_DEP_VERSION} REQUIRED)
find_package(KF6TextWidgets ${KF_DEP_VERSION} REQUIRED)
find_package(KF6WidgetsAddons ${KF_DEP_VERSION} REQUIRED)
find_package(KF6XmlGui ${KF_DEP_VERSION} REQUIRED)

include(ECMGenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMAddQch)
include(ECMDeprecationSettings)
include(KDEClangFormat)
include(KDEGitCommitHooks)
include(CMakeDependentOption)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

set(EXCLUDE_DEPRECATED_BEFORE_AND_AT 0 CACHE STRING "Control the range of deprecated API excluded from the build [default=0].")

ecm_setup_version(PROJECT VARIABLE_PREFIX KMORETOOLS
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kmoretools_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF6MoreToolsConfigVersion.cmake"
                        SOVERSION 6)

add_definitions(-DTRANSLATION_DOMAIN=\"kmoretools\")
ecm_set_disabled_deprecation_versions(
    QT 6.4.0
    KF 5.102
)

ki18n_install(po)
add_subdirectory(data)
add_subdirectory(src)
if (BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF6MoreTools")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF6MoreTools_QCH
        FILE KF6MoreToolsQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF6MoreToolsConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF6MoreToolsConfig.cmake"
  INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF6MoreToolsConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF6MoreToolsConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KF6MoreToolsTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF6MoreToolsTargets.cmake NAMESPACE KF6:: )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kmoretools_version.h DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF} COMPONENT Devel)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
