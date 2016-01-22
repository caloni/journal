#define _CRT_SECURE_NO_WARNINGS
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "\bitforge\samples\ss.h"
#include "\bitforge\samples\Files\Files.h"
#include <fstream>
#include <string>
#include <regex>
#include <iomanip>
#include <time.h>
#include <windows.h>
#include <Shellapi.h>
#include <Shlwapi.h>
#include <algorithm>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "User32.lib")

using namespace std;


string GetCurrentDT(const char *format = "%Y-%m-%d %H:%M:%S")
{
    char szDT[100] = "0000-00-00 00:00:00";
    time_t tNow = time(0);
    tm *tmNow = localtime(&tNow);

    if( tmNow )
        strftime(szDT, sizeof(szDT), format, tmNow);

    return szDT;
}

string GetDT(time_t tNow, const char *format = "%Y-%m-%d %H:%M:%S")
{
    char szDT[100] = "0000-00-00 00:00:00";
    tm *tmNow = localtime(&tNow);

    if( tmNow )
        strftime(szDT, sizeof(szDT), format, tmNow);

    return szDT;
}

string FormatTimeElapsed(size_t seconds)
{
    size_t hh = seconds / (60 * 60);
    size_t mm = (seconds - hh * 60 * 60) / 60;
    size_t sss = (seconds - mm * 60 - hh * 60 * 60);
    return ss() << setfill('0') << setw(2) << hh << ":" << setw(2) << mm << ":" << setw(2) << sss;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timerLabel = new QLabel();
    categoryLabel = new QLabel();
    statusBar()->addPermanentWidget(categoryLabel);
    statusBar()->addPermanentWidget(timerLabel);

    m_lastTitle = "qdaytoday";
    m_lastCategoryS = "v. " __DATE__;
    m_startTime = 0;

    ChangeTitle();
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &MainWindow::on_tick);
    timer->setSingleShot(false);
    timer->start(1000);

    connect(ui->plainTextEdit, &DayToDayInput::input, this, &MainWindow::on_input);
    connect(ui->plainTextEdit, &DayToDayInput::enter, this, &MainWindow::on_enter);
    connect(ui->plainTextEdit, &DayToDayInput::stop, this, &MainWindow::on_stop);
    connect(ui->plainTextEdit, &DayToDayInput::open, this, &MainWindow::on_open);

    if (StrStrI(GetCommandLine(), L"/clip"))
    {
        ss lineS;


        //if (HWND activeWnd = GetForegroundWindow())
        //{
        //    char title[500] = {};
        //    GetWindowTextA(activeWnd, title, 500);

        //    if (title[0])
        //        lineS << "clip: " << title << "\n\n";
        //}

        if (OpenClipboard(NULL))
        {
            if (HANDLE clipH = GetClipboardData(CF_TEXT))
            {
                if (PVOID clipData = GlobalLock(clipH))
                {
                    string line = lineS << (char*)clipData;
                    QString qline = QString().fromLatin1(line.c_str());
                    string lineTranslated = qline.toStdString();
                    on_input(qline);
                    GlobalUnlock(clipH);
                    QTimer::singleShot(0, this, SLOT(close()));
                }
            }
            CloseClipboard();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool SpecialChar(char c)
{
    bool ret = !(c == ' ' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
    return ret;
}

bool IsSpace(char c)
{
    bool ret = c == ' ';
    return ret;
}

void ReplaceOrErase(string& titleS, string::iterator it)
{
    static map<char, char> st_specialChars;

    if (st_specialChars.empty())
    {
        st_specialChars['á'] = 'a';
        st_specialChars['ã'] = 'a';
        st_specialChars['à'] = 'a';
        st_specialChars['ä'] = 'a';

        st_specialChars['ó'] = 'o';
        st_specialChars['õ'] = 'o';
        st_specialChars['ò'] = 'o';
        st_specialChars['ö'] = 'o';

        st_specialChars['é'] = 'e';
        st_specialChars['è'] = 'e';
        st_specialChars['ë'] = 'e';
        st_specialChars['ê'] = 'e';

        st_specialChars['í'] = 'i';
        st_specialChars['ì'] = 'i';
        st_specialChars['ï'] = 'i';

        st_specialChars['ú'] = 'u';
        st_specialChars['ù'] = 'u';
        st_specialChars['ü'] = 'u';

        st_specialChars['ç'] = 'c';
        st_specialChars['ñ'] = 'n';


        st_specialChars['Á'] = 'A';
        st_specialChars['Ã'] = 'A';
        st_specialChars['À'] = 'A';
        st_specialChars['Ä'] = 'A';

        st_specialChars['Ó'] = 'O';
        st_specialChars['Õ'] = 'O';
        st_specialChars['Ò'] = 'O';
        st_specialChars['Ö'] = 'O';

        st_specialChars['É'] = 'E';
        st_specialChars['È'] = 'E';
        st_specialChars['Ë'] = 'E';
        st_specialChars['Ê'] = 'E';

        st_specialChars['Í'] = 'I';
        st_specialChars['Ì'] = 'I';
        st_specialChars['Ï'] = 'I';

        st_specialChars['Ú'] = 'U';
        st_specialChars['Ù'] = 'U';
        st_specialChars['Ü'] = 'U';

        st_specialChars['Ç'] = 'C';
        st_specialChars['Ñ'] = 'N';
    }

    char fnd = *it;
    auto match = st_specialChars.find(fnd);

    if (match != st_specialChars.end())
        *it = match->second;
    else
        titleS.erase(it);
}

std::string Slug(QString title)
{
    auto titleS = title.toLatin1().toStdString();
    string::iterator it;
    while ((it = find_if(titleS.begin(), titleS.end(), SpecialChar)) != titleS.end())
        ReplaceOrErase(titleS, it);
    while ((it = find_if(titleS.begin(), titleS.end(), IsSpace)) != titleS.end())
        titleS.replace(it, it + 1, "-");
    if (titleS.size())
        titleS = "-" + titleS;
    return QString(titleS.c_str()).toLower().toStdString();
}


KnownCategories MainWindow::ParseCategory(const string& category)
{
    KnownCategories ret;

    if (category == "cinema")
        ret = kcCinema;
    else if (category == "cinemaqui")
        ret = kcCinemaqui;
    else if (category == "series")
        ret = kcSeries;
    else if (category == "blog" || category == "caloni")
        ret = kcBlog;
    else
        ret = kcUnknown;

    return ret;
}


void ParseTitle(string& title, string& category)
{
    regex getCat("^([a-z]+): .*$");
    string catSubst = regex_replace(title, getCat, "$1");
    if (catSubst.size() && catSubst != title)
    {
        regex getTit("^([a-z]+: )*(.*$)");
        string titSubst = regex_replace(title, getTit, "$2");
        if (titSubst.size())
        {
            category = catSubst;
            title = titSubst;
        }
    }
}


string FindCategoryPath(const string& category)
{
    string ret = ss() << "_posts\\" << GetCurrentDT("%Y-%m-%d") << "-" << category << ".md";

    vector<string> files;
    if (FindFiles(ss() << "_posts\\*" << category << ".md", files) && files.size())
        ret = ss() << "_posts\\" << files[0];

    return ret;
}


void MainWindow::on_input(const QString &line)
{
    ofstream ofs("daytoday.txt", ios::app);
    time_t tNow = time(0);
    string startDt = GetDT(m_startTime);
    string currDt = GetDT(tNow);
    size_t seconds = tNow - m_startTime;

    QStringList lines = line.split("\n");

    if (seconds > 60)
        ofs << startDt << ' ' << currDt << ' ' << FormatTimeElapsed(seconds) << endl;
    for(int i = 0; i < lines.size(); ++i)
    {
        QString currLine = lines[i];
        if(currLine.size())
            ofs << startDt << ' ' << currLine.toStdString() << endl;
    }

    // atualizando post daytoday
    //{
    //    string path = ss() << "pages\\3daytoday.md";
    //    ofstream ofs(path, ios_base::app);
    //    ofs << endl << endl << "**" << startDt << "**" << endl;

    //    for (int i = 0; i < lines.size(); ++i)
    //    {
    //        QString currLine = lines[i];
    //        ofs << currLine.toStdString() << endl;
    //    }
    //}

    string category, title;
    if (lines.size() > 1)
    {
        title = lines[0].toStdString();
        ParseTitle(title, category);
    }

    if( ! category.empty() )
    {
        ofstream ofs;

        switch (m_lastCategory = ParseCategory(category))
        {
        case kcCinema:
            m_lastPath = ss() << "..\\cinetenisverde.github.io\\_posts\\" << GetCurrentDT("%Y-%m-%d") << Slug(title.c_str()) << ".md";
            ofs.open(m_lastPath);
            ofs << "---\n"
                << "title: \"" << title << "\"\n"
                << "imdb: \"0000000\"\n"
                << "stars: \"3/5\"\n"
                << "---\n";
            break;

        case kcCinemaqui:
            m_lastPath = ss() << "..\\cinetenisverde.github.io\\cinemaqui\\" << GetCurrentDT("%Y-%m-%d") << Slug(title.c_str()) << ".md";
            ofs.open(m_lastPath);
            ofs << "---\n"
                << "title: \"" << title << "\"\n"
                << "---\n";
            break;

        case kcSeries:
            m_lastPath = ss() << "..\\cinetenisverde.github.io\\_posts\\series\\" << GetCurrentDT("%Y-%m-%d") << Slug(title.c_str()) << ".md";
            ofs.open(m_lastPath);
            ofs << "---\n"
                << "title: \"" << title << "\"\n"
                << "category: \"" << "series" << "\"\n"
                << "---\n";
            break;

        case kcBlog:
            m_lastPath = ss() << "..\\caloni.github.io\\_posts\\" << GetCurrentDT("%Y-%m-%d") << Slug(title.c_str()) << ".md";
            ofs.open(m_lastPath);
            ofs << "---\n"
                << "title: \"" << title << "\"\n"
                << "tags: [ ]\n"
                << "---\n";
            break;

        default:
            m_lastPath = FindCategoryPath(category);

            if (PathFileExistsA(m_lastPath.c_str()))
            {
                ofs.open(m_lastPath, ios::app);
                ofs << "\n### " << title << "\n\n";
            }
            else
            {
                ofs.open(m_lastPath);
                ofs << "---\n"
                    << "title: \"" << title << "\"\n"
                    << "categories: " << category << "\n"
                    << "---\n";
            }
            break;
        }

        for(int i = 1; i < lines.size(); ++i)
        {
            QString currLine = lines[i];
            string line = currLine.toStdString();
            if (i == 1 && line.empty())
                continue;
            ofs << line << endl;
        }
    }
}

void MainWindow::on_enter(const QString &line)
{
    QStringList lines = line.split("\n");
    string category, title;
    if (lines.size() > 0)
    {
        title = lines[0].toStdString();
        ParseTitle(title, category);
    }

    if( ! category.empty() )
        m_lastCategoryS = category;

    if( ! title.empty() )
        m_lastTitle = title;
}

void MainWindow::on_stop()
{
    switch (m_lastCategory)
    {
    case kcCinema:
    case kcCinemaqui:
    case kcBlog:
        ShellExecuteA(NULL, "edit", m_lastPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
        break;
    }

    QApplication::quit();
}

void MainWindow::on_open()
{
    ShellExecute(NULL, L"edit", L"daytoday.txt", NULL, NULL, SW_SHOWNORMAL);
    QApplication::quit();
}

void MainWindow::on_tick()
{
    ChangeTitle();
}

void MainWindow::ChangeTitle()
{
    if (m_startTime)
    {
        time_t tNow = time(0);
        QString timeElapsed = FormatTimeElapsed(tNow - m_startTime).c_str();
        setWindowTitle(QString(m_lastTitle.c_str()) + " - qdaytoday");
        timerLabel->setText(timeElapsed);
        categoryLabel->setText(QString(m_lastCategoryS.c_str()));
    }
    else
    {
        m_startTime = time(0);
        setWindowTitle(QString(m_lastTitle.c_str()) + " - qdaytoday");
        timerLabel->setText("00:00:00");
        categoryLabel->setText(QString(m_lastCategoryS.c_str()));
    }
}
