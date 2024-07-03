#include <coroutine>
#include <atomic>

class async_manual_reset_event {
public:
    async_manual_reset_event(bool initiallySet = false) noexcept;

    async_manual_reset_event(const async_manual_reset_event&) = delete;
    async_manual_reset_event& operator=(const async_manual_reset_event&) = delete;
    async_manual_reset_event(async_manual_reset_event&&) = delete;
    async_manual_reset_event& operator=(const async_manual_reset_event&&) = delete;

    bool is_set() const noexcept;

    struct awaiter;
    awaiter operator co_await() const noexcept;

    void set() noexcept;
    void reset() noexcept;

private:
    friend struct awaiter;

    mutable std::atomic<void *> m_state;
};

struct async_manual_reset_event::awaiter {
    awaiter(const async_manual_reset_event& event) noexcept
        : m_event(event)
        {}

    bool await_ready() const noexcept;
    bool await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept;
    void await_resume() noexcept {}
    friend struct async_manual_reset_event;
private:
    awaiter * m_next;
    const async_manual_reset_event& m_event;
    std::coroutine_handle<> m_awaitingCoro;
};

bool async_manual_reset_event::awaiter::await_ready() const noexcept {
    return m_event.is_set();
}

bool async_manual_reset_event::awaiter::await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept {
    const void * const setState = &m_event;
    m_awaitingCoro = awaitingCoroutine;

    void* oldValue = m_event.m_state.load(std::memory_order_acquire);

    do {
        if (oldValue == setState) return false;
        
        m_next = static_cast<awaiter*>(oldValue);

    } while (!m_event.m_state.compare_exchange_weak(
                oldValue,
                this,
                std::memory_order_release, std::memory_order_acquire));
    return true;
}

async_manual_reset_event::async_manual_reset_event(bool initiallySet) noexcept
: m_state(initiallySet ? this : nullptr)
{}

bool async_manual_reset_event::is_set() const noexcept {
    return m_state.load(std::memory_order_acquire) == this;
}

void async_manual_reset_event::reset() noexcept {
    void *oldValue = this;
    m_state.compare_exchange_strong(oldValue, nullptr, std::memory_order_acquire);
}

void async_manual_reset_event::set() noexcept {
    void* oldValue = m_state.exchange(this, std::memory_order_acq_rel);
    if (oldValue != this) {
        auto* waiters = static_cast<awaiter*>(oldValue);
        while(waiters != nullptr) {
            auto* next = waiters->m_next;
            waiters->m_awaitingCoro.resume();
            waiters = next;
        }
    }
}

async_manual_reset_event::awaiter
async_manual_reset_event::operator co_await() const noexcept {
    return awaiter { *this };
}

int main() {
    return 0;
}
