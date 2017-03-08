
#include "vty.h"
#include "command.h"
#include "keyfunction.h"


int main()
{
    vty::add_keyfunction(new KF_question('?'                ));
    vty::add_keyfunction(new KF_return  ('\r'               ));
    vty::add_keyfunction(new KF_delete  (0x7f               ));
    vty::add_keyfunction(new KF_ctrl_B  (slankdev::AC_Ctrl_B));
    vty::add_command(new Cmd_halt("halt"));
    vty::add_command(new Cmd_quit("quit"));
    vty::add_command(new Cmd_show("show"));
    vty::set_port(9999);

    vty vty;
    vty.dispatch();
}


