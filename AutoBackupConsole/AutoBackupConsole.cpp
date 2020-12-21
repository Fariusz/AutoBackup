#include <iostream>
#include "Dialog.h"

int main(int argc, const char* argv[])
{
    setlocale(LC_ALL, "");
    Dialog userDialog;
    Command lastCommand;
    do
    {
        lastCommand = userDialog.showMainDialog();
    } while (lastCommand != Command::Shutdown);
    return 0;
}