#include "IpcWindow.h"
#include "qapplication.h"

int main(int argc, char *argv[]){
    QApplication a(argc,argv);
    IpcWindow ipc_window;
    ipc_window.show();
    return a.exec();

}
