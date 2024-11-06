#pragma once
#include <winsock2.h>
#include <iostream>
#include "ECS/transform_component.h"
//#include "game.h"

class network_handler
{
public:
    static void send_data(const SOCKET s, const char* data, const int len, const int flags = 0);

    static char* rcv_data(const SOCKET s);

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

    static void send_transform_data(const SOCKET connect_socket, const transform_component& transform);
    static void receive_transform_data(const SOCKET connect_socket, transform_component& transform);
    static void send_wall_script_data(SOCKET connect_socket, const wall_script& wallScript);
    static void receive_wall_script_data(SOCKET connect_socket, wall_script& wallScript);
    static void send_toggle_command(SOCKET connect_socket);
    static void receive_toggle_command(SOCKET connect_socket, wall_script& wall_script);
    static void handle_server_operations(SOCKET connect_socket, const entity& player, const entity& wall);
    static void handle_client_operations(SOCKET connect_socket, const entity& client, const entity& wall);

    //static void update_replication(SOCKET s);
};
