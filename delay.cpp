#include <delay.h>

void msdelay(int msec)
{
    QEventLoop loop;

    QTimer::singleShot(msec, &loop, &QEventLoop::quit);

    loop.exec();
}
