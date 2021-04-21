cmake_minimum_required(VERSION 3.19)
project(SampleThreadSafeQueue C)

set(CMAKE_C_STANDARD 99)
add_library(c-pthread-queue STATIC ConsumerProducer.c)
link_libraries("-lpthread")
link_libraries("-lczmq")
add_executable(ConsumerProducer ConsumerProducer.c)
add_executable(PushPullsample simplePushPull.c)