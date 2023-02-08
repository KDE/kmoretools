/*
    SPDX-FileCopyrightText: 2014-2015 Gregor Mi <codestruct@posteo.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "kmoretools.h"
#include "kmoretools_p.h"
#include "kmoretoolsmenufactory.h"
#include "kmoretoolspresets.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>

/**
 * Each test case starts a test GUI.
 * Run kmoretoolstest_interactive with the test case name as first parameter
 * (e.g. testDialogForGroupingNames) to run only this test GUI.
 */
class KMoreToolsTestInteractive : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();

    void testConfigDialogAllInstalled();
    void testConfigDialogSomeNotInstalled();
    void testConfigDialogNotInstalled1Service2Items();

    void test_buildMenu_WithQActions_interative1();

    void testDialogForGroupingNames();

    void testLazyMenu();

private:
    void testConfigDialogImpl(bool withNotInstalled, bool withMultipleItemsPerNotInstalledService, const QString &description);
};

void KMoreToolsTestInteractive::init()
{
}

void KMoreToolsTestInteractive::cleanup()
{
}

bool menuAtLeastOneActionWithText(const QMenu *menu, const QString &text)
{
    const auto lstActions = menu->actions();
    for (auto a : lstActions) {
        if (a->text() == text) {
            return true;
        }
    }

    return false;
}

void KMoreToolsTestInteractive::test_buildMenu_WithQActions_interative1()
{
    KMoreTools kmt("unittest-kmoretools/qactions"); // todo: disable copy-ctor!?

    const auto menuBuilder = kmt.menuBuilder();
    menuBuilder->clear();
    QMenu menu;
    menuBuilder->addMenuItem(new QAction("Hallo 1", &menu), "id1");
    menuBuilder->addMenuItem(new QAction("Hallo 2", &menu), "id2");
    menuBuilder->addMenuItem(new QAction("Hallo 3", &menu), "id3");

    menuBuilder->buildByAppendingToMenu(&menu);
    menuBuilder->showConfigDialog("test_buildMenu_WithQActions 1");
}

void KMoreToolsTestInteractive::testConfigDialogImpl(bool withNotInstalled, bool withMultipleItemsPerNotInstalledService, const QString &description)
{
    KMoreTools kmt("unittest-kmoretools/2");
    const auto kateApp = kmt.registerServiceByDesktopEntryName("org.kde.kate");
    const auto gitgApp = kmt.registerServiceByDesktopEntryName("gitg");
    const auto notinstApp = kmt.registerServiceByDesktopEntryName("mynotinstalledapp");
    const auto notinstApp2 = kmt.registerServiceByDesktopEntryName("mynotinstapp2");
    notinstApp2->setHomepageUrl(QUrl("https://www.kde.org"));
    const auto menuBuilder = kmt.menuBuilder();
    menuBuilder->addMenuItem(kateApp);
    menuBuilder->addMenuItem(gitgApp);
    if (withNotInstalled) {
        auto item1 = menuBuilder->addMenuItem(notinstApp);
        item1->setInitialItemText(notinstApp->formatString("$Name - item 1"));

        menuBuilder->addMenuItem(notinstApp2);

        if (withMultipleItemsPerNotInstalledService) {
            auto item3 = menuBuilder->addMenuItem(notinstApp);
            item3->setInitialItemText(notinstApp->formatString("$Name - second item"));
        }
    }
    auto i1 = menuBuilder->addMenuItem(kateApp, KMoreTools::MenuSection_More);
    i1->setId("kate1");
    i1->setInitialItemText("Kate more");
    auto i2 = menuBuilder->addMenuItem(gitgApp, KMoreTools::MenuSection_More);
    i2->setId("gitg1");
    i2->setInitialItemText("gitg more");
    menuBuilder->showConfigDialog(description);

    // show resulting menu
    auto dlg = new QDialog();
    auto button = new QPushButton("Test the menu", dlg);
    auto menu = new QMenu(dlg);
    menuBuilder->buildByAppendingToMenu(menu);
    button->setMenu(menu); // TODO: connect to the button click signal to always rebuild the menu
    auto label =
        new QLabel("Test the menu and hit Esc to exit if you are done. Note that changes made via the Configure dialog will have no immediate effect.", dlg);
    label->setWordWrap(true);
    auto layout = new QHBoxLayout();
    layout->addWidget(button);
    layout->addWidget(label);
    dlg->setLayout(layout);
    QObject::connect(dlg, &QDialog::finished, dlg, [dlg]() {
        qDebug() << "delete dlg;";
        delete dlg;
    });
    dlg->exec();
}

void KMoreToolsTestInteractive::testConfigDialogAllInstalled()
{
    testConfigDialogImpl(false, false, "TEST all installed");
}

void KMoreToolsTestInteractive::testConfigDialogSomeNotInstalled()
{
    testConfigDialogImpl(true, false, "TEST some not installed");
}

void KMoreToolsTestInteractive::testConfigDialogNotInstalled1Service2Items()
{
    testConfigDialogImpl(true, true, "TEST more than one item for one not installed service");
}

void KMoreToolsTestInteractive::testDialogForGroupingNames()
{
    // show resulting menu
    auto dlg = new QDialog();
    auto labelInfo = new QLabel(
        "First, select a URL (leave the URL box empty to give no URL; don't forget to add file:// or https://). Then, select a grouping name. => A menu will "
        "be created that you can try out. KDE4/KF5: If an application does not start even there is the launch indicator, try: $ eval `dbus-launch`",
        dlg);
    labelInfo->setWordWrap(true);
    auto selectButton = new QPushButton("Select grouping name...", dlg);
    auto labelLineEdit = new QLabel("URL 1 (file://..., https://...", dlg);
    auto urlLineEdit = new QLineEdit(dlg);
    urlLineEdit->setText("file:///etc/bash.bashrc");
    auto menuButton = new QPushButton("<wait for selection>", dlg);

    const auto groupingNamesList = {
        "disk-usage",
        "disk-partitions",
        "files-find",
        "font-tools",
        "git-clients-for-folder",
        "git-clients-and-actions",
        "icon-browser",
        "language-dictionary",
        "mouse-tools",
        "screenrecorder",
        "screenshot-take",
        "system-monitor-processes",
        "system-monitor-logs",
        "time-countdown",
    };

    KMoreToolsMenuFactory menuFactory("unittest-kmoretools/3");

    auto groupingNamesMenu = new QMenu(dlg);
    QMenu *moreToolsMenu = nullptr;
    for (auto groupingName : groupingNamesList) {
        auto action = new QAction(groupingName, groupingNamesMenu);
        action->setData(groupingName);
        groupingNamesMenu->addAction(action);

        QObject::connect(action,
                         &QAction::triggered,
                         action,
                         [action, &menuFactory, &moreToolsMenu, urlLineEdit, menuButton]() { // clazy:exclude=lambda-in-connect
                             auto groupingName = action->data().toString();
                             QUrl url;
                             if (!urlLineEdit->text().isEmpty()) {
                                 url.setUrl(urlLineEdit->text());
                             }
                             moreToolsMenu = menuFactory.createMenuFromGroupingNames({groupingName}, url);
                             menuButton->setText(QString("menu for: '%1' (URL arg: %2...").arg(groupingName, url.isEmpty() ? "<empty>" : "<see URL 1>"));
                             menuButton->setMenu(moreToolsMenu);
                         });
    }

    selectButton->setMenu(groupingNamesMenu);

    auto hLayout = new QHBoxLayout();
    auto vLayout = new QVBoxLayout();
    hLayout->addWidget(labelInfo);
    hLayout->addLayout(vLayout);
    vLayout->addWidget(labelLineEdit);
    vLayout->addWidget(urlLineEdit);
    vLayout->addWidget(selectButton);
    vLayout->addWidget(menuButton);
    dlg->setLayout(hLayout);
    dlg->setBaseSize(300, 150);
    QObject::connect(dlg, &QDialog::finished, dlg, [dlg]() {
        qDebug() << "delete dlg;";
        delete dlg;
    });
    dlg->exec();
}

void KMoreToolsTestInteractive::testLazyMenu()
{
    KMoreToolsMenuFactory menuFactory("unittest-kmoretools/4");

    auto moreToolsMenu = menuFactory.createMenuFromGroupingNames({"git-clients-for-folder"});

    auto dlg = new QDialog();
    auto button = new QPushButton("Test the lazy menu", dlg);
    button->setMenu(moreToolsMenu);
    auto label =
        new QLabel("Test the menu and hit Esc to exit if you are done. Note that changes made via the Configure dialog will have no immediate effect.", dlg);
    label->setWordWrap(true);
    auto layout = new QHBoxLayout();
    layout->addWidget(button);
    layout->addWidget(label);
    dlg->setLayout(layout);
    QObject::connect(dlg, &QDialog::finished, dlg, [dlg]() {
        qDebug() << "delete dlg;";
        delete dlg;
    });
    dlg->exec();
}

QTEST_MAIN(KMoreToolsTestInteractive)

#include "kmoretoolstest_interactive.moc"
