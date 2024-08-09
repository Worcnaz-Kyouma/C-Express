#include "Thread.hpp"
#include <thread>
#include <utility>

template <typename _Fn, typename... _Args>
Thread<_Fn, _Args...>::Thread(_Fn &&_Fx, _Args &&..._Ax):
thread_object(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...) {}