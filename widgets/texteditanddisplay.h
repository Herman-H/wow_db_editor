#ifndef TEXTEDITANDDISPLAY_H
#define TEXTEDITANDDISPLAY_H

#include <QWidget>

namespace Ui {
class TextEditAndDisplay;
}

class TextEditAndDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit TextEditAndDisplay(QWidget *parent = 0);
    ~TextEditAndDisplay();

    void setText(const QString &str);
    QString text() const
    {
        return string;
    }
    void saveChanges();

    bool isTextChanged() const;

    virtual bool eventFilter(QObject *, QEvent *);

    //void displayComparison();

signals:
    void textChanged();

private:
    Ui::TextEditAndDisplay *ui;
    QString string;
private slots:
    void onTextChanged();
};

#endif // TEXTEDITANDDISPLAY_H
