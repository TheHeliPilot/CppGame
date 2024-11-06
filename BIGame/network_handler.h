#pragma once
#include <winsock2.h>
#include <iostream>

#include "vector2.h"

class entity;
class wall_script;
class transform_component;

struct replication_data
{
    vector2 server_position;
    vector2 client_pos;
};

struct network_data
{
    const char* flag;
    char* data;
};

class network_handler
{
public:
    static network_data last_data;
    
    static void send_data(const SOCKET s, const char* data, const int flags = MSG_PARTIAL);

    static char* rcv_data(const SOCKET s);

    static void receive_data_thread(const SOCKET& connect_socket, network_data& last_data_, bool& is_receiving, const bool& should_listen);

    template <typename T>
    static char* serialize(const T& object)
    {
        const auto buffer = new char[sizeof(T)];
        std::memcpy(buffer, &object, sizeof(T));
        return buffer;
    }

    template <typename T>
    static T deserialize(const char* buffer)
    {
        T object;
        std::memcpy(&object, buffer, sizeof(T));
        return object;
    }

    static char* make_data(const char* flag, char* data)
    {
        const auto ser_data = new network_data();
        ser_data->flag = flag;
        ser_data->data = data;
        return serialize(*ser_data);
    }

    static network_data get_data(const char& data)
    {
        auto new_data = network_data();
        new_data = deserialize<network_data>(&data);
        return new_data;
    }

    //static void update_replication(SOCKET s);
};
