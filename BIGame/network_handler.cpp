#include "network_handler.h"



class transform_component;

struct replication_data
{
    vector2 server_position;
    vector2 client_pos;
};

void network_handler::send_data(const SOCKET s, const char* data, const int len, const int flags)
{
    const int res = send(s, data, len, flags);
    //std::cout << "send_data_client: " << res << '\n';
}

char* network_handler::rcv_data(const SOCKET s)
{
    char recv_data[1024];
    const int res = recv(s, recv_data, 1024, 0);
    if(res > 0)
        return recv_data;

    return nullptr;
}

void network_handler::send_transform_data(const SOCKET connect_socket, const transform_component& transform) {
    network_handler::send_data(connect_socket, network_handler::serialize(transform.position), sizeof(vector2));
}

void network_handler::receive_transform_data(const SOCKET connect_socket, transform_component& transform) {
    const auto* data = network_handler::rcv_data(connect_socket);
    if (data != nullptr) {
        transform.position = network_handler::deserialize<vector2>(data);
    } else {
        std::cout << "received no data\n";
    }
}

void network_handler::send_wall_script_data(const SOCKET connect_socket, const wall_script& wallScript) {
    network_handler::send_data(connect_socket, network_handler::serialize(wallScript.is_on), sizeof(bool));
}

void network_handler::receive_wall_script_data(const SOCKET connect_socket, wall_script& wallScript) {
    const auto* data = network_handler::rcv_data(connect_socket);
    if (data != nullptr) {
        const bool received_state = network_handler::deserialize<bool>(data);
        if (wallScript.is_on != received_state) {
            wallScript.is_on = received_state;
        }
    } else {
        std::cout << "received no data\n";
    }
}

void network_handler::send_toggle_command(const SOCKET connect_socket) {
    const auto dta = "toggle_button";
    network_handler::send_data(connect_socket, dta, strlen(dta));
}

void network_handler::receive_toggle_command(const SOCKET connect_socket, wall_script& wall_script) {
    const auto* data = network_handler::rcv_data(connect_socket);
    if (data != nullptr) {
        if (strcmp(data, "toggle_button") == 0) {
            wall_script.is_on = !wall_script.is_on;
        }
    } else {
        std::cout << "received no data\n";
    }
}

void network_handler::handle_server_operations(const SOCKET connect_socket, const entity& player, const entity& wall) {
    send_transform_data(connect_socket, player.get_component<transform_component>());
    receive_wall_script_data(connect_socket, wall.get_component<wall_script>());
}

void network_handler::handle_client_operations(const SOCKET connect_socket, const entity& client, const entity& wall) {
    receive_transform_data(connect_socket, client.get_component<transform_component>());
    receive_wall_script_data(connect_socket, wall.get_component<wall_script>());
    receive_toggle_command(connect_socket, wall.get_component<wall_script>());
}

/*
void network_handler::update_replication(const SOCKET s)
{
    if(game::is_server)
    {
        int count = 0;
        for (const auto transform_component : game::transform_components)
        {
            if(transform_component->replicates)
            {
                count++;
            }
        }
        const auto replication_data = new transform_replication_data;
        replication_data->transforms = new vector2[count];
        int index = 0;
        for (const auto transform_component : game::transform_components)
        {
            if (transform_component->replicates)
            {
                replication_data->transforms[index++] = transform_component->position;
            }
        }
        const auto buffer = new char[sizeof(transform_replication_data)];
        std::memcpy(buffer, replication_data, sizeof(transform_replication_data));
        send_data(s, buffer, sizeof(transform_replication_data));
    }
    else
    {
        const auto replication_data = new transform_replication_data;
        memcpy(replication_data, rcv_data(s), sizeof(transform_replication_data));

        int index = 0;
        for (const auto transform_component : game::transform_components)
        {
            if (transform_component->replicates)
            {
                transform_component->position = replication_data->transforms[index++];
            }
        }
    }
}*/
