#ifndef CE_THREAD_H
#define CE_THREAD_H

#include <thread>

template <typename _Fn, typename... _Args>
class Thread {
private:
    std::thread thread_object;
public:
    Thread(_Fn &&_Fx, _Args &&..._Ax);
};

#endif