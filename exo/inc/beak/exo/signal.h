/**
 * @file        signal
 *
 * @author      Barath Kannan
 * @date        Sun Feb 25 09:34:31 2018
 * @ingroup     exo
 */

#ifndef BEAK_EXO_SIGNAL_H
#define BEAK_EXO_SIGNAL_H

#include <beak/exo/detail/signal_impl.h>

namespace beak::exo{

template <typename... Args>
class signal
{
public:
    //default constructable
    signal() = default;

    //non-copyable
    signal(const signal&) = delete;
    signal& operator=(const signal&) = delete;

    //move-able
    signal(signal&&) = default;
    signal& operator=(signal&&) = default;

    template <typename F, typename R = std::invoke_result_t<F, Args...>>
    [[nodiscard]] auto connect(F&& f) -> signal<R>
    {
        return _signal_impl.connect(std::forward<F>(f));
    }

    template <typename... FArgs>
    void emit(FArgs&&... fargs)
    {
        _signal_impl.emit(std::forward<FArgs>(fargs)...);
    }

    template <typename... FArgs>
    void operator()(FArgs&&... fargs)
    {
        emit(std::forward<FArgs>(fargs)...);
    }

private:
    signal(detail::signal_impl<Args...> signal_impl)
    : _signal_impl(std::move(signal_impl)){}
    detail::signal_impl<Args...> _signal_impl;
};

} // namespace beak::exo

#endif // BEAK_EXO_SIGNAL_H

