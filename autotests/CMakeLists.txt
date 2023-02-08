# SPDX-FileCopyrightText: KDE Contributors
# SPDX-License-Identifier: BSD-2-Clause

include(ECMMarkAsTest)
include(ECMMarkNonGuiExecutable)
include(ECMAddTests)

remove_definitions(-DQT_NO_CAST_FROM_ASCII)

ecm_add_test(kmoretoolstest.cpp ../src/kmoretools_debug.cpp
             LINK_LIBRARIES Qt6::Test Qt6::Widgets KF6::I18n KF6::KIOGui KF6::MoreTools
             TEST_NAME kmoretoolstest)

add_executable(kmoretoolstest2 kmoretoolstest2.cpp ../src/kmoretools_debug.cpp)
ecm_mark_as_test(kmoretoolstest2)
target_link_libraries(kmoretoolstest2 Qt6::Test KF6::MoreTools KF6::I18n KF6::KIOGui)
