# SPDX-FileCopyrightText: KDE Contributors
# SPDX-License-Identifier: BSD-2-Clause

include(ECMMarkAsTest)
include(ECMMarkNonGuiExecutable)
include(ECMAddTests)

find_package(Qt6Test ${REQUIRED_QT_VERSION} NO_MODULE REQUIRED)
set_package_properties(Qt6Test PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required for unit tests")

ecm_add_test(kmoretools/kmoretoolstest.cpp ../src/kmoretools_debug.cpp
             LINK_LIBRARIES Qt6::Test Qt6::Widgets KF6::I18n KF6::KIOGui KF6::MoreTools
             TEST_NAME kmoretoolstest)

add_executable(kmoretoolstest2 kmoretoolstest2.cpp ../src/kmoretools_debug.cpp)
ecm_mark_as_test(kmoretoolstest2)
target_link_libraries(kmoretoolstest2 Qt6::Test KF6::NewStuff KF6::I18n KF6::KIOGui)
