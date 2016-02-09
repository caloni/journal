/** Controle para adição de uma nova nota ao DayToDay.
* Essa janela controle o uso de algumas teclas especiais, como Enter, Shift e Ctrl. Também pergunta se o conteúdo
deve ser salvo e ao ativar o salvar ele envia um evento e fecha a janela.

TODO
 - Suporte a zoom (Ctrl+, Ctrl-).
*/
#ifndef DAYTODAYINPUT
#define DAYTODAYINPUT
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QMessageBox>

class DayToDayInput : public QPlainTextEdit
{
    Q_OBJECT

public:
    DayToDayInput(QWidget * parent = 0) : QPlainTextEdit(parent)
    {
    }

protected:
    virtual void keyPressEvent(QKeyEvent * e)
    {
        if( e->key() == Qt::Key_Return)
        {
            // new entry
            if( e->modifiers() & Qt::ControlModifier )
            {
                input(toPlainText());
                setPlainText("");

                // and next
                if ( !(e->modifiers() & Qt::ShiftModifier) )
                {
                    stop();
                }
            }
            // just an enter
            else
            {
                enter(toPlainText());
                QPlainTextEdit::keyPressEvent(e);
            }
        }
        else if(e->key() == Qt::Key_Escape )
        {
            bool exit = true;

            if (toPlainText().size())
            {
                auto reply = QMessageBox::question(this, "QDayToDay",
                    "Exit without saving?",
                    QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::No)
                    exit = false;
            }

            if (exit)
                stop();
        }
        else if( e->key() == Qt::Key_O && e->modifiers() & Qt::ControlModifier)
        {
            open();
        }
        else
        {
            keyenter(e->key());
            QPlainTextEdit::keyPressEvent(e);
        }
    }

signals:
    void keyenter(int key);
    void input(const QString& line);
    void enter(const QString& line);
    void open();
    void stop();
};

#endif // DAYTODAYINPUT

