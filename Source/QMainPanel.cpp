#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QApplication>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QGraphicsDropShadowEffect>

#include "QMainPanel.h"
#include "Library.h"
#include "Browser.h"

#ifdef Q_OS_WIN
#include <windowsx.h>
#endif
#include <QFontDatabase>

TabLabel* tabFactory(TabLabel* label, QString name, QString text)
{
    label->setObjectName(name);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    label->setMinimumWidth(80);
    label->setMaximumWidth(110);
    label->setAlignment(Qt::AlignTop);
    label->setFont(QFont("Sansation", 14));
    label->setText(text);
    label->setStyleSheet("color: #FFF;");

    return label;
}

#ifdef Q_OS_WIN
QMainPanel::QMainPanel(HWND hWnd) : QWinWidget(hWnd)
{

    windowHandle = hWnd;
#else
QMainPanel::QMainPanel(QMainWindow *window) : QWidget(window)
{
    qWindow = window;
#endif

    setObjectName("mainPanel");

    // Create stacked widget for main content
    stack = new QStackedWidget();

    // Prepare UI objects for each tab
    libraryPtr = new Library();
    browserPtr = new Browser();
    stack->addWidget(libraryPtr);
    stack->addWidget(browserPtr);
    stack->setCurrentWidget(libraryPtr);

    // System layout
    QHBoxLayout *systemLayout = new QHBoxLayout;
    systemLayout->setSpacing(0);
    systemLayout->setMargin(8);

    // Header spacing
    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->setMargin(0);

    // Header layout
    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->setSpacing(0);
    headerLayout->setMargin(0);


    // Window title
    QLabel *windowTitle = new QLabel(this);
    windowTitle->setObjectName("windowTitle");
    windowTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    windowTitle->setMinimumWidth(175);
    windowTitle->setMaximumWidth(175);
    windowTitle->setAlignment(Qt::AlignTop);
    windowTitle->setFont(QFont("Sansation", 18));
    windowTitle->setText("Project \nASCENSION");
    windowTitle->setStyleSheet("color: #7D818C;");
    windowTitle->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Post-initialization header spacing
    topLayout->addLayout(systemLayout);
    topLayout->addLayout(headerLayout);
    topLayout->addSpacing(10);

    headerLayout->addSpacing(20);
    headerLayout->addWidget(windowTitle);
    headerLayout->addSpacing(40);

    // Header tabs
    libraryTab = new TabLabel(this);
    libraryTab = tabFactory(libraryTab, "libraryTab", "LIBRARY");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(libraryTab);
    libraryTab->setStyleSheet("font-weight: bold; color: lightgreen;");

    storeTab = new TabLabel(this);
    storeTab = tabFactory(storeTab, "storeTab", "  STORE");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(storeTab);

    modsTab = new TabLabel(this);
    modsTab = tabFactory(modsTab, "modsTab", " MODS");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(modsTab);

    newsTab = new TabLabel(this);
    newsTab = tabFactory(newsTab, "newsTab", "NEWS");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(newsTab);

    browserTab = new TabLabel(this);
    browserTab = tabFactory(browserTab, "browserTab", "BROWSER");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(browserTab);

    activeTab = libraryTab;

    headerLayout->addStretch();

    // System buttons
    systemLayout->addStretch();

    // Minimize
    QPushButton *pushButtonMinimize = new QPushButton("", this);
    pushButtonMinimize->setObjectName("pushButtonMinimize");
    systemLayout->addWidget(pushButtonMinimize);
    QObject::connect(pushButtonMinimize, SIGNAL(clicked()), this, SLOT(pushButtonMinimizeClicked()));

    // Maximize
    QPushButton *pushButtonMaximize = new QPushButton("", this);
    pushButtonMaximize->setObjectName("pushButtonMaximize");
    systemLayout->addWidget(pushButtonMaximize);
    QObject::connect(pushButtonMaximize, SIGNAL(clicked()), this, SLOT(pushButtonMaximizeClicked()));

    // Close
    QPushButton *pushButtonClose = new QPushButton("", this);
    pushButtonClose->setObjectName("pushButtonClose");
    systemLayout->addWidget(pushButtonClose);
    QObject::connect(pushButtonClose, SIGNAL(clicked()), this, SLOT(pushButtonCloseClicked()));

    // Main panel layout
    QGridLayout *mainGridLayout = new QGridLayout();
    mainGridLayout->setSpacing(0);
    mainGridLayout->setMargin(0);
    setLayout(mainGridLayout);

    // Central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Main panel scroll area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("mainPanelScrollArea");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Vertical layout example
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(5);
    verticalLayout->setMargin(0);
    verticalLayout->setAlignment(Qt::AlignHCenter);
    verticalLayout->addLayout(topLayout, 1);

    verticalLayout->addWidget(stack, 4);

    // Connect signals
    connect(libraryTab, SIGNAL(clicked()), this, SLOT(setTabLibrary()));
    connect(browserTab, SIGNAL(clicked()), this, SLOT(setTabBrowser()));

    // Show
    centralWidget->setLayout(verticalLayout);
    scrollArea->setWidget(centralWidget);
    mainGridLayout->addWidget(scrollArea);

    show();
}

// Tab swap slots
void QMainPanel::setTabLibrary()
{
    if (stack->currentWidget()->objectName() != "libraryUI")
    {
        activeTab->setStyleSheet("font-weight: regular; color: #FFF;");
        stack->setCurrentWidget(libraryPtr);
        activeTab = libraryTab;
        activeTab->setStyleSheet("font-weight: bold; color: lightgreen;");
    }
}

void QMainPanel::setTabBrowser()
{
    if(stack->currentWidget()->objectName() != "browserUI")
    {
        activeTab->setStyleSheet("font-weight: regular; color: #FFF;");
        stack->setCurrentWidget(browserPtr);
        activeTab = browserTab;
        activeTab->setStyleSheet("font-weight: bold; color: lightgreen;");
    }
}

// Button events
void QMainPanel::pushButtonMinimizeClicked()
{
#ifdef Q_OS_WIN
    ShowWindow(parentWindow(), SW_MINIMIZE);
#else
    qWindow->showMinimized();
#endif
}

void QMainPanel::pushButtonMaximizeClicked()
{
#ifdef Q_OS_WIN
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(parentWindow(), &wp);
    if (wp.showCmd == SW_MAXIMIZE)
    {
        ShowWindow(parentWindow(), SW_RESTORE);
    }
    else
    {
        ShowWindow(parentWindow(), SW_MAXIMIZE);
    }
#else
    if (qWindow->isMaximized())
    {
        qWindow->showNormal();
    }
    else
    {
        qWindow->showMaximized();
    }
#endif
}

void QMainPanel::pushButtonCloseClicked()
{
#ifdef Q_OS_WIN
    PostQuitMessage(0);
#else
    qWindow->close();
#endif
}

#ifdef Q_OS_WIN
#if QT_VERSION >= 0x050000
bool QMainPanel::nativeEvent(const QByteArray &, void *msg, long *)
{
#else
bool QMainPanel::winEvent(MSG *message, long *)
{
#endif
#if QT_VERSION >= 0x050000
    MSG *message = (MSG *)msg;
#endif
    switch(message->message)
    {
    case WM_SYSKEYDOWN:
    {
        if (message->wParam == VK_SPACE)
        {
            RECT winrect;
            GetWindowRect(windowHandle, &winrect);
            TrackPopupMenu(GetSystemMenu(windowHandle, false), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, windowHandle, NULL);
            break;
        }
    }
    case WM_KEYDOWN:
    {
        if (message->wParam == VK_F5 ||
                message->wParam == VK_F6 ||
                message->wParam == VK_F7
                )
        {
            SendMessage(windowHandle, WM_KEYDOWN, message->wParam, message->lParam);
            break;
        }
    }
    }

    return false;
}

void QMainPanel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        ReleaseCapture();
        SendMessage(windowHandle, WM_NCLBUTTONDOWN, HTCAPTION, 0);
    }

    if (event->type() == QEvent::MouseButtonDblClick)
    {
        if (event->button() == Qt::LeftButton)
        {
            WINDOWPLACEMENT wp;
            wp.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement(parentWindow(), &wp);
            if (wp.showCmd == SW_MAXIMIZE)
            {
                ShowWindow(parentWindow(), SW_RESTORE);
            }
            else
            {
                ShowWindow(parentWindow(), SW_MAXIMIZE);
            }
        }
    }
}
#endif
