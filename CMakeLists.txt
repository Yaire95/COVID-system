cmake_minimum_required(VERSION 3.16)
project(exam C)

set(CMAKE_C_STANDARD 99)

add_executable(exam SpreaderDetectorBackend.c SpreaderDetectorParams.h People.in Meetings.in)