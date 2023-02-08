# SPDX-FileCopyrightText: Alexander Lohnau <alexander.lohnau@gmx.de>
# SPDX-License-Identifier: BSD-2-Clause

add_executable(kmoretoolstest_interactive kmoretoolstest_interactive.cpp)
ecm_mark_as_test(kmoretoolstest_interactive)
target_link_libraries(kmoretoolstest_interactive Qt6::Test KF6::MoreTools KF6::I18n Qt6::Widgets KF6::KIOGui)
