#include "network_handler.h"

#include <mutex>
#include <SDL_timer.h>

#include "vector2.h"
#include "CustomScripts/button_script.h"
#include "ECS/transform_component.h"


class transform_component;

std::mutex last_data_mutex;
std::vector<network_data> network_handler::last_data;

void network_handler::send_data(const SOCKET s, const char* data, const int flags)
{
    const int res = send(s, data, static_cast<int>(strlen(data)), flags);
    //std::cout << "send_data size: " << sizeof(data) << '\n';
}

char* network_handler::rcv_data(const SOCKET s)
{
    constexpr int buffer_size = 1024;
        char recv_data[buffer_size];
        
        const int res = recv(s, recv_data, buffer_size, 0);
        if (res > 0) {
            const auto data = new char[res + 1];
            std::memcpy(data, recv_data, res);
            data[res] = '\0';
            
            return data;
        }

        return nullptr;
}


void network_handler::receive_data_thread(
    const SOCKET& connect_socket,
    std::vector<network_data>& last_data_,
    bool& is_receiving,
    const bool& should_listen)
{
    while (should_listen)
    {
        is_receiving = true;
        const auto* rcv_data = network_handler::rcv_data(connect_socket);
        const network_data data = get_data(*rcv_data);
        if (sizeof(data.data) == data.data_size)
        {
            std::cout << "Received data from client" << '\n';
            {
                std::lock_guard<std::mutex> lock(last_data_mutex);
                last_data.push_back(data);
            }
            std::cout << "pushed back data: " << data.flag << '\n';
            is_receiving = false;
        }
    }
}

void network_handler::process_received_data(entity& player, const entity& client, const entity& button, const entity& plate1)
{
    std::vector<network_data> temp_data;

    {
        std::lock_guard<std::mutex> lock(last_data_mutex);
        temp_data = last_data;
        last_data.clear();
        last_data.reserve(1); // Reserve space for dummy data
    }

    for (const auto& data : temp_data)
    {
        //std::cout << "received flag: " << data.flag << "\n";

        if (strcmp(data.flag, "player_pos") == 0)
        {
            client.get_component<transform_component>().position = deserialize_vector2(data.data);
        }
        else if (strcmp(data.flag, "button_switch") == 0)
        {
            button.get_component<button_script>().is_on = deserialize_bool(data.data);
        }
        else if (strcmp(data.flag, "plate") == 0)
        {
            plate1.get_component<button_script>().is_on = deserialize_bool(data.data);
        }
    }
}