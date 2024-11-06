#include "network_handler.h"

#include <SDL_timer.h>

#include "vector2.h"
#include "CustomScripts/wall_script.h"
#include "ECS/transform_component.h"


class transform_component;

network_data network_handler::last_data;

void network_handler::send_data(const SOCKET s, const char* data, const int flags)
{
    const int res = send(s, data, sizeof(data), flags);
    //std::cout << "send_data_client: " << res << '\n';
}

char* network_handler::rcv_data(const SOCKET s)
{
    char recv_data[1024];
    const int res = recv(s, recv_data, 1024, MSG_PARTIAL);
    if(res > 0)
        return recv_data;

    return nullptr;
}

void network_handler::receive_data_thread(const SOCKET& connect_socket, network_data& last_data_, bool& is_receiving, const bool& should_listen)
{
    while(should_listen)
    {
        last_data.flag = "OLD_DATA";
        is_receiving = true;
        const auto* rcv_data = network_handler::rcv_data(connect_socket);
        last_data_ = get_data(*rcv_data);
        is_receiving = false;
        SDL_Delay(10);
    }
}
