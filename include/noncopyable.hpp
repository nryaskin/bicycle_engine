#ifndef BICYCLE_ENGINE_NONCOPYABLE_HPP
#define BICYCLE_ENGINE_NONCOPYABLE_HPP

namespace bicycle_engine {
    class noncopyable {
    protected:
        noncopyable() = default;
        ~noncopyable() = default;
    private:
        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
    };
}

#endif /* BICYCLE_ENGINE_NONCOPYABLE_HPP */
