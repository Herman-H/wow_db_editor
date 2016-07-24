#ifndef SELECTER_DIALOG
#define SELECTER_DIALOG

#include <QString>
class QDialog;

class selecter_dialog
{
public:
    virtual QDialog * getDialog()       const = 0;
    virtual QString   getString(int id) const = 0;
    virtual bool      hasSelection()    const = 0;
    virtual void      setId(int id)           = 0;
    virtual void      reset()                 = 0;
};

#endif // SELECTER_DIALOG

