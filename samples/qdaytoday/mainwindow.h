/** Janela principal.
* Ela está interligada ao controle principal que recebe o conteúdo e dispara os eventos.

TODO
 - Mudar título para tempo de uso.
  - Talvez exportar esse tempo para algum lugar.
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <string>

namespace Ui {
class MainWindow;
}

enum KnownCategories
{
    kcUnknown,
    kcCinema,
    kcCinemaqui,
    kcSeries,
    kcBlog,
    kcBitForge,
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_keyenter(int key);
    void on_input(const QString& line);
    void on_enter(const QString& line);
    void on_stop();
    void on_open();
    void on_tick();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QLabel* timerLabel;
    QLabel* categoryLabel;

private:
    KnownCategories ParseCategory(const std::string& category);

private:
    time_t m_startTime, m_restartTime, m_lastKeyTime;
    int m_seconds;
    std::string m_lastPath;
    std::string m_lastTitle;
    std::string m_lastCategoryS;
    KnownCategories m_lastCategory;

    void ChangeTitle();
};

#endif // MAINWINDOW_H
