#include "texteditanddisplay.h"
#include "ui_texteditanddisplay.h"

TextEditAndDisplay::TextEditAndDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextEditAndDisplay),
    string(QString{""})
{
    ui->setupUi(this);
    ui->plainTextEdit->hide();
    ui->plainTextEdit->installEventFilter(this);
    ui->textEdit->installEventFilter(this);
    ui->plainTextEdit->setPlainText(string);
    ui->textEdit->setText(string);
    QObject::connect(ui->plainTextEdit,SIGNAL(textChanged()),this,SLOT(onTextChanged()));
}

TextEditAndDisplay::~TextEditAndDisplay()
{
    delete ui;
}

void TextEditAndDisplay::setText(const QString &str)
{
    string = str;
    string.replace('\r', "");
    ui->plainTextEdit->setPlainText(string);
    ui->textEdit->setText(string);
}

bool TextEditAndDisplay::eventFilter(QObject * o, QEvent * e)
{
    if(o == ui->plainTextEdit && e->type() == QEvent::FocusOut)
    {
        ui->textEdit->setText(ui->plainTextEdit->toPlainText());
        ui->textEdit->show();
        ui->plainTextEdit->hide();
    }
    else if(o == ui->textEdit && e->type() == QEvent::FocusIn)
    {
        ui->textEdit->hide();
        ui->plainTextEdit->show();
        ui->plainTextEdit->setFocus();
    }
}

void TextEditAndDisplay::saveChanges()
{
    string = ui->plainTextEdit->toPlainText();
    ui->textEdit->setText(string);
}

void TextEditAndDisplay::onTextChanged()
{
    emit textChanged();
}

bool TextEditAndDisplay::isTextChanged() const
{
    return string != ui->plainTextEdit->toPlainText();
}

/*void TextEditAndDisplay::displayComparison()
{
    QString str1 = string;
    QString str2 = ui->plainTextEdit->toPlainText();
    for(int i = 0; i < std::max(string.size(), ui->plainTextEdit->toPlainText().size()); i++)
    {
        bool b = false;
        if(str1.size() > i && str2.size() > i)
        {
            b = str1.at(i) == str2.at(i);
        }
        QString c1 = "-";
        QString c2 = "-";

        if(str1.size() > i)
        {
            c1 = str1.at(i);
            if(c1 == "\n")
            {
                c1 = "\\n";
            }
            else if(c1 == "\t")
            {
                c1 = "\\t";
            }
            else if(c1 == " ")
            {
                c1 = "space";
            }
            else if(c1 == "\r")
            {
                c1 = "\\r";
            }
        }
        if(str2.size() > i)
        {
            c2 = str2.at(i);
            if(c2 == "\n")
            {
                c2 = "\\n";
            }
            else if(c2 == "\t")
            {
                c2 = "\\t";
            }
            else if(c2 == " ")
            {
                c2 = "space";
            }
            else if(c2 == "\r")
            {
                c2 = "\\r";
            }
        }

        QString d = (b ? QString{"true"} : QString{"false"}) + QString{": "} + QString{c1} + QString{" : "} + QString{c2};

        qDebug(d.toLocal8Bit());
    }
}*/
