/*
    SPDX-FileCopyrightText: 2014, 2015 Gregor Mi <codestruct@posteo.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "kmoretools.h"
#include "kmoretools_p.h"
#include "kmoretoolsmenufactory.h"
#include "kmoretoolspresets.h"

#include <QTest>

/**
 * The tests in this class have some runtime requirements.
 * Details see comment in implementation of each test case.
 */
class KMoreToolsTest2 : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();

    // use cases:
    void testInstalledAppStructure();
    void testInstalledAppSetInitialItemText();
    void test_buildMenu_ShowConfigureMenuItem();
};

void KMoreToolsTest2::init()
{
}

void KMoreToolsTest2::cleanup()
{
}

/**
 * NOTE: we assume kate is installed
 */
void KMoreToolsTest2::testInstalledAppStructure()
{
    KMoreTools kmt(_("unittest-kmoretools/2"));
    const auto kateApp = kmt.registerServiceByDesktopEntryName(_("org.kde.kate"));
    const auto menuBuilder = kmt.menuBuilder();
    menuBuilder->addMenuItem(kateApp);
    QString s = menuBuilder->menuStructureAsString(false);
    qDebug() << s;
    QCOMPARE(s, QString(_("|main|:org.kde.kate.|more|:|notinstalled|:")));
}

/**
 * NOTE: we assume kate is installed
 * and that the translated Name is "Kate"
 */
void KMoreToolsTest2::testInstalledAppSetInitialItemText()
{
    KMoreTools kmt(_("unittest-kmoretools/2"));
    const auto kateApp = kmt.registerServiceByDesktopEntryName(_("org.kde.kate"));
    const auto menuBuilder = kmt.menuBuilder();
    auto kateAppItem = menuBuilder->addMenuItem(kateApp);
    kateAppItem->setInitialItemText(kateApp->formatString(_("$Name in super-user mode")));
    auto action = kateAppItem->action();
    QVERIFY(action); // because kate is installed;
    QCOMPARE(action->text(), QString(_("Kate in super-user mode")));
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

bool menuAtLeastNoActionWithText(const QMenu *menu, const QString &text)
{
    const auto lstActions = menu->actions();
    for (auto a : lstActions) {
        if (a->text() == text) {
            qDebug() << a->text();
            return false;
        }
    }

    return true;
}

/**
 * NOTE: we assume kate is installed
 */
void KMoreToolsTest2::test_buildMenu_ShowConfigureMenuItem()
{
    {
        KMoreTools kmt(_("unittest-kmoretools/2"));
        const auto kateApp = kmt.registerServiceByDesktopEntryName(_("org.kde.kate"));
        // porcelain: other (interactive) tests will reuse the kmt id so we make sure that
        // no user configuration disturbs this test:
        const auto menuBuilder = kmt.menuBuilder(_("porcelain"));
        menuBuilder->addMenuItem(kateApp);
        QMenu menu;
        menuBuilder->buildByAppendingToMenu(&menu); // == KMoreTools::ConfigureDialogAccessible_Always
        qDebug() << menuBuilder->menuStructureAsString(true);
        QVERIFY(menuAtLeastOneActionWithText(&menu, _("Configure..."))); // "Kate", "Separator", "Configure..."

        {
            menu.clear();
            menuBuilder->buildByAppendingToMenu(&menu, KMoreTools::ConfigureDialogAccessible_Defensive);
            QVERIFY(menuAtLeastNoActionWithText(&menu, _("Configure...")));
        }
    }

    {
        KMoreTools kmt(_("unittest-kmoretools/2"));
        const auto kateApp = kmt.registerServiceByDesktopEntryName(_("org.kde.kate"));
        const auto mynotInstalledApp = kmt.registerServiceByDesktopEntryName(_("mynotinstalledapp"));
        const auto menuBuilder = kmt.menuBuilder(_("porcelain"));
        menuBuilder->addMenuItem(kateApp);
        menuBuilder->addMenuItem(mynotInstalledApp);
        QMenu menu;
        menuBuilder->buildByAppendingToMenu(&menu); // == KMoreTools::ConfigureDialogAccessible_Always

        auto doAssert = [](QMenu *menu) {
            const QList<QAction *> actions = menu->actions();
            QCOMPARE(actions.count(), 3); // "Kate", "Separator", "More..."
            auto moreMenu = actions[2]->menu();
            QCOMPARE(moreMenu->actions().count(), 4); // "Not-installed-section", "Not installed app", "Separator", "Configure menu..."
            auto configureMenu = actions[3];
            QCOMPARE(configureMenu->data().toString(), QString(_("configureItem")));
        };

        doAssert(&menu);

        {
            menu.clear();
            menuBuilder->buildByAppendingToMenu(&menu, KMoreTools::ConfigureDialogAccessible_Defensive);
            doAssert(&menu); // = same as _Always because there is one not-installed item
        }
    }
}

QTEST_MAIN(KMoreToolsTest2)

#include "kmoretoolstest2.moc"
