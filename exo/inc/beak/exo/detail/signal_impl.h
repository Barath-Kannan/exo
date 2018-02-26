/**
 * @file        signal
 *
 * @author      Barath Kannan
 * @date        Thu Feb 22 00:04:15 2018
 * @ingroup     exo
 */

#ifndef BEAK_EXO_SIGNAL_IMPL_H
#define BEAK_EXO_SIGNAL_IMPL_H

#include <beak/exo/detail/signal_base.h>
#include <beak/exo/enum.h>

namespace beak::exo::detail {

template <typename... Args>
class signal_impl
{
    using base_type = signal_base<Args...>;

public:
    //default constructable
    signal_impl(ThreadSafe ts = ThreadSafe::On)
        : _signal_base(std::make_shared<base_type>(ts))
    {}

    signal_impl(std::shared_ptr<base_type> s)
        : _signal_base(std::move(s))
    {}

    //non-copyable
    signal_impl(const signal_impl&) = delete;
    signal_impl& operator=(const signal_impl&) = delete;

    //move-able
    signal_impl(signal_impl&&) = default;
    signal_impl& operator=(signal_impl&&) = default;

    template <typename F, typename... ExtraArgs, typename R = std::invoke_result_t<F, ExtraArgs..., Args...>>
    [[nodiscard]] auto connect(F&& f, ExtraArgs&&... extra_args) -> signal_impl<R>
    {
        return _signal_base->connect(std::forward<F>(f), std::forward<ExtraArgs>(extra_args)...);
    }

    template <typename... FArgs>
    void emit(FArgs&&... fargs)
    {
        _signal_base->emit(std::forward<FArgs>(fargs)...);
    }

private:
    std::shared_ptr<base_type> _signal_base;
};

} //namespace beak::exo::detail

#endif // BEAK_EXO_SIGNAL_IMPL_H
