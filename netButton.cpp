#include "netButton.h"

netButton::netButton(MainWindow * mw, std::string id, int index) {
    p_mw = mw;
    p_Id = id;
    Index = index;
}

void netButton::on_clicked() {

    p_mw->on_buttonShow_clicked(p_Id, Index);
}

