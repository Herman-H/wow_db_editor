#ifndef SELECTER_BUTTON_H
#define SELECTER_BUTTON_H

#include <QObject>

class QHBoxLayout;
class QVBoxLayout;
class TextEditAndDisplay;
class QPushButton;
class QDialog;
class selecter_dialog;

class selecter_button : public QObject
{
    Q_OBJECT
private:
    QWidget *                           widget;
    QHBoxLayout *                       layout;
    QPushButton *                       button;
    QPushButton *                       clr;
    selecter_dialog * const             dialog;

    const int                           invalidIdDefault;
    bool                                isSelectingValidId;
    int                                 selected;
public:
    selecter_button(selecter_dialog * dialog, int unselectedId = 0);
    ~selecter_button();
    QWidget * getButton();

    int selectedId();
    void setId(int id);
public slots:
    void buttonPushed();
    void clear();
signals:
    void selection();
};

#endif // SELECTER_BUTTON_H
