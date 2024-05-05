# Boost ASIO

In main project I think it would be great to receive wayland objects in synchronous maner.
Spending some time with google I foung out that there are a lot of recomendation to use asio completion token,
but documentation on this matter is not easy to understand.
So to learn some features I am going to start with implementing some
simple async functions which I am going to call in synchronous maner.

# Definitions

Let's start with language which Boost uses to desribe our entities in Boost terms.
First of all functions that we want to implement called `asynchronus operations`. Which is not suprising but here we are going to start.
So we need to implement our `asynchronus operation` for example for main project it can be functino named `get_output` and this operation shall allow seamless composition with operations provied by boost.

# Sample
For experimental sample we are going to simulate poll in the following way.

Since my concern is using Boost ASIO with other third party library I am going to provide class interface `SlowDevice` which defines callbask with return value of `std::string msg` which, as I understand doesn't comply with Boost asynchronus model.

Method `get_hello` accepts callback sets it internally and starts thread which is going to wait for simplicity 5 seconds before posting result to callback.
After thread creation `get_hello` returns without waiting for thread completion.

