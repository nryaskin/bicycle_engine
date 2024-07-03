#include <string>
#include <type_traits>

#include <boost/asio.hpp>

#include "slow_device.hpp"

// I am going to use to try out api examples from following links
//https://think-async.com/Asio/asio-1.28.0/doc/asio/overview/model/completion_tokens.html
//https://www.boost.org/doc/libs/1_84_0/doc/html/boost_asio/overview/composition/compose.html

template<boost::asio::completion_token_for<void(std::string)> CompletionToken>
auto async_do_work(SlowDevice& sd, CompletionToken&& token) {
    auto init = [] (auto completion_handler,
                    SlowDevice& sd) {
        sd.get_hello(std::move(completion_handler));
    };

    return boost::asio::async_initiate<CompletionToken, void(std::string)>(init, token, sd);
}

void do_work() {
//    std::string msg = co_await sd.get_hello();
    SlowDevice sd;
    output_current_time("start: ");
    auto f = async_do_work(sd, boost::asio::use_future);
    output_current_time("before future get: ");
    std::cout << f.get() << "\n";
    output_current_time("after future get: ");

}

boost::asio::awaitable<void> do_work_awaitable() {
    SlowDevice sd;
    auto msg = async_do_work(sd, boost::asio::use_awaitable);
    output_current_time("before await: ");
    std::cout << co_await msg << "\n";
    output_current_time("after await: ");
    co_return;
}

int main() {
    boost::asio::io_context io;
    io.run();
    return 0;
}
