/**
 * @file        Signal base
 *
 * @author      Barath Kannan
 * @date        Sat Feb 24 18:00:10 2018
 * @ingroup     EXO
 */

#ifndef BEAK_EXO_DETAIL_SIGNAL_BASE_H
#define BEAK_EXO_DETAIL_SIGNAL_BASE_H

#include <any>
#include <functional>
#include <memory>
#include <mutex>
#include <type_traits>

namespace beak::exo::detail {

template <typename... Args>
struct function_type
{
    using type = std::function<void(Args...)>;
};

template <>
struct function_type<void>
{
    using type = std::function<void()>;
};

template <typename... Args>
using function_type_t = typename function_type<Args...>::type;

template <typename... Args>
class signal_base : public std::enable_shared_from_this<signal_base<Args...>>
{
    using sft = std::enable_shared_from_this<signal_base<Args...>>;
    using slot_function = function_type_t<Args...>;

public:
    //default-constructable
    signal_base() = default;

    //parented signal_base
    template <typename... SArgs>
    signal_base(std::weak_ptr<signal_base<SArgs...>> weak)
    {
        _parent_eraser = [ this, weak = std::move(weak) ]()
        {
            if (auto shared = weak.lock()) {
                shared->disconnect(*this);
            }
        };
    }

    ~signal_base()
    {
        if (_parent_eraser) {
            _parent_eraser();
        }
    }

    //non-copyable
    signal_base(const signal_base&) = delete;
    signal_base& operator=(const signal_base&) = delete;

    //non-move-able
    signal_base(signal_base&&) = delete;
    signal_base& operator=(signal_base&&) = delete;

    template <typename F, typename R = std::invoke_result_t<F, Args...>>
    auto connect(F&& f) -> std::shared_ptr<signal_base<R>>
    {
        // shared ptr to new signal parented to this
        auto s = std::make_shared<signal_base<R>>(sft::weak_from_this());
        auto raw = s.get();
        _slots.push_back(
            {[&f, raw](Args&&... args) {
                 if constexpr (std::is_void_v<R>) {
                     std::invoke(f, std::forward<Args>(args)...);
                     raw->emit();
                 }
                 else {
                     raw->emit(std::invoke(f, std::forward<Args>(args)...));
                 }
             },
             std::weak_ptr<signal_base<R>>(s)});
        return s;
    }

    template <typename... FArgs>
    void emit(FArgs&&... fargs) const
    {
        for (auto&& slot : _slots) {
            slot._function(std::forward<FArgs>(fargs)...);
        }
    }

private:
    template <typename... SArgs>
    void disconnect(const signal_base<SArgs...>& s)
    {
        using signal_type = signal_base<SArgs...>;
        using owner_type = std::shared_ptr<signal_type>;
        using weak_type = std::weak_ptr<signal_type>;
        _slots.erase(std::remove_if(_slots.begin(), _slots.end(), [&](const slot_type& slot) -> bool {
            //check that the type matches
            if (slot._owner.type() == typeid(weak_type)) {
                auto weak = std::any_cast<weak_type>(slot._owner);
                if (owner_type shared = weak.lock(); !shared) {
                    //because we are already in the destructor, weak ptrs cannot be elevated
                    return true;
                }
            }
            return false;
        }));
    }

    struct slot_type
    {
        slot_function _function; // function bound to this slot
        std::any _owner;         // type erased weak_ptr to the owning signal
    };

    std::vector<slot_type> _slots;

    //calls the parents disconnect method for this signal
    //invoked by the destructor
    std::function<void()> _parent_eraser;
};

} //namespace beak::exo::detail

#endif // BEAK_EXO_DETAIL_SIGNAL_BASE_H
