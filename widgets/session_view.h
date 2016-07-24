#ifndef SESSION_VIEW_H
#define SESSION_VIEW_H

#include <QDialog>

class session_manager;

namespace Ui {
class session_view;
}

class session_view : public QDialog
{
    Q_OBJECT

    const session_manager * sess_mgr;

public:
    explicit session_view(const session_manager * sess_mgr, QWidget *parent = 0);
    ~session_view();

    void update();


private:
    Ui::session_view *ui;
};

#endif // SESSION_VIEW_H
