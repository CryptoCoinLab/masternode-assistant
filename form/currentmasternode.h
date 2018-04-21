#ifndef CURRENTMASTERNODE_H
#define CURRENTMASTERNODE_H
#include <cmasternode.h>
#include <QWidget>
#include "masternodedlg.h"

namespace Ui {
class CurrentMasternode;
}

class CurrentMasternode : public QWidget
{
    Q_OBJECT
public:
    explicit CurrentMasternode(QWidget *parent = 0);
    ~CurrentMasternode();
    void setValues(const CMasternode& cmn);

private slots:
    void on_pb_edit_clicked();

private:
    Ui::CurrentMasternode *ui;
    CMasternode m_cmn;
    MasternodeDlg m_mndlg;
};

#endif // CURRENTMASTERNODE_H
