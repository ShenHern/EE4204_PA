cmake_minimum_required(VERSION 3.10)
project(Ex3 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Client
        Client/headsock.h
        Client/tcp_client3.c)
add_executable(Server
        Server/headsock.h
        Server/tcp_ser3.c)