cmake_minimum_required(VERSION 3.7)
project(SampleThreadSafeQueue C)

set(CMAKE_C_STANDARD 99)
add_library(c-pthread-queue STATIC ConsumerProducer.c)
link_libraries("-lpthread")
link_libraries("-lczmq")
link_libraries("-lzmq")
link_libraries("-lm")
link_libraries("-ljson-c")
add_executable(ConsumerProducer ConsumerProducer.c)
add_executable(PUB simplePUB.c)
add_executable(SUB simpleSUB.c)
add_executable(PushPullsample simplePushPull.c)