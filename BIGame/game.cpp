#include "game.h"

#include <thread>
#include <ws2tcpip.h>

#include "collision.h"
#include "network_handler.h"
#include "vector2.h"
#include "CustomScripts/player_movement.h"
#include "ECS/keyboard_handler.h"
#include "ECS/components.h"

#define DEFAULT_PORT "2302"

//networking
struct addrinfo *result = nullptr, *ptr = nullptr, hints;
SOCKET game::listen_socket = INVALID_SOCKET;
SOCKET game::connect_socket = INVALID_SOCKET;

SDL_Renderer* game::renderer = nullptr;
SDL_Event game::event;

// ReSharper disable once CppInconsistentNaming
manager game::manager_;
asset_manager* game::asset_manager = new ::asset_manager(&manager_);
vector2 game::mouse_pos;
auto collision = new class collision;
std::vector<collider_component*> game::colliders;
std::vector<physics_component*> game::physics_components;

//threads
std::thread recv_thread;

WSADATA game::wsa_data;

auto& player = game::manager_.add_entity();
auto& client = game::manager_.add_entity();
auto& wall = game::manager_.add_entity();

enum group_labes : size_t
{
    group_default,
    group_arrows
};

std::vector<entity> spawned_entities;

game::game()
= default;

game::~game()
= default;

void game::init(const char* title, const int x_pos, const int y_pos, const int width, const int height, const bool fullscreen, const bool is_server)
{
    is_server_ = is_server;

    mouse_pos = *new vector2(0, 0);

    const int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "SDL innit successful\n";
        
        window_ = SDL_CreateWindow(title, x_pos, y_pos, width, height, flags);
        if(window_)
            std::cout << "Window created\n";

        renderer = SDL_CreateRenderer(window_, -1, 0);
        if(renderer)
        {
            SDL_SetRenderDrawColor(renderer, 110, 108, 105,255);
            std::cout << "Renderer created\n";
        }
        
        is_running_ = true;
    }
    else
    {
        is_running_ = false;
    }

#pragma region networking setup
    
    int iResult = WSAStartup(MAKEWORD(2,2), &wsa_data);
    if(iResult != 0)
    {
        std::cout << "WSAStartup failed\n";
    }

    std::cout << "WSA Init\n";
    
    if(is_server_)
    {
        ZeroMemory(&hints, sizeof (hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        
        iResult = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &result);
        if (iResult != 0) {
            printf("get addr info failed: %d\n", iResult);
            WSACleanup();
            return;
        }

        std::cout << "get addr info done\n";

        listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        
        if (listen_socket == INVALID_SOCKET) {
            printf("Error at socket(): %d\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return;
        }

        std::cout << "Socket created\n";

        // Set up the TCP listening socket
        iResult = bind( listen_socket, result->ai_addr, static_cast<int>(result->ai_addrlen));
        if (iResult == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(listen_socket);
            WSACleanup();
            return;
        }
        freeaddrinfo(result);

        std::cout << "Bind done\n";

        if ( listen( listen_socket, SOMAXCONN ) == SOCKET_ERROR ) {
            printf( "Listen failed with error: %d\n", WSAGetLastError() );
            closesocket(listen_socket);
            WSACleanup();
            return;
        }

        std::cout << "Listening on port " << DEFAULT_PORT << "\n";

        connect_socket = INVALID_SOCKET;

        // Accept a client socket
        connect_socket = accept(listen_socket, nullptr, nullptr);
        if (connect_socket == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            closesocket(listen_socket);
            WSACleanup();
            return;
        }

        std::cout << "Client connected\n";
    }
    else
    {
        ZeroMemory( &hints, sizeof(hints) );
        hints.ai_family   = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
        if (iResult != 0) {
            printf("get addr info failed: %d\n", iResult);
            WSACleanup();
            return;
        }

        std::cout << "get addr info done\n";

        // Attempt to connect to the first address returned by
        // the call to get addr info
        ptr=result;

        // Create a SOCKET for connecting to server
        connect_socket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);

        if (connect_socket == INVALID_SOCKET) {
            printf("Error at socket(): %d\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return;
        }
        std::cout << "Socket created\n";

        // Connect to server.
        iResult = connect( connect_socket, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
        if (iResult == SOCKET_ERROR) {
            closesocket(connect_socket);
            connect_socket = INVALID_SOCKET;
        }

        // Should really try the next address returned by getaddrinfo
        // if the connect call failed
        // But for this simple example we just free the resources
        // returned by getaddrinfo and print an error message

        freeaddrinfo(result);

        if (connect_socket == INVALID_SOCKET) {
            printf("Unable to connect to server!\n");
            WSACleanup();
            return;
        }
        std::cout << "Connected\n";
        
    }

    //thread
    network_handler::last_data = {};
    recv_thread = std::thread(&network_handler::receive_data_thread, std::ref(connect_socket), std::ref(network_handler::last_data), std::ref(is_receiving_), std::ref(is_running_));
    
#pragma endregion

    ///ECS

    //static
    wall.add_component<transform_component>(200,200, 16, 16, 3);
    wall.add_component<sprite_component>("assets/textures/wall.png");
    wall.add_component<collider_component>("wall", true);
    wall.add_component<wall_script>();

    //player
    player.add_component<transform_component>();
    player.add_component<sprite_component>("assets/textures/player.png");
    player.add_component<collider_component>("player");
    player.add_component<keyboard_handler>();
    player.add_component<physics_component>(.7f);
    player.add_component<player_movement>(2);

    client.add_component<transform_component>(500,300);
    client.add_component<sprite_component>("assets/textures/player2.png");
    //client.add_component<collider_component>();
    
    std::cout << "Objects created\n";
}

void game::handle_events()
{
    
    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_QUIT:
            is_running_ = false;
            break;

        default: break;
    }
}

void game::update()
{
    //std::cout << "Updating game\n";
    
    //collision handle
    handle_collision();

    //sending data
    network_handler::send_data(connect_socket, network_handler::make_data("player_pos", network_handler::serialize_vector2(player.get_component<transform_component>().position)));


    //handle data
    network_handler::process_received_data(player, client, wall);
    

    int mouse_x;
    int mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    mouse_pos.x = static_cast<float>(mouse_x);
    mouse_pos.y = static_cast<float>(mouse_y);
    
    manager_.refresh();
    manager_.update();
    
}


auto& default_obj = game::manager_.get_group(group_default);
auto& arrow_obj = game::manager_.get_group(group_arrows);

void game::render()
{
    SDL_RenderClear(renderer);

    manager_.draw();
    
    //for(const auto& d : default_obj)
    //{
    //    d->draw();
    //}
    //for(const auto& a : arrow_obj)
    //{
    //    a->draw();
    //}

    SDL_RenderPresent(renderer);
}

void game::clean()
{
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Vsetko clean";
}

void game::handle_collision()
{
    for (const auto physics_component : game::physics_components)
    {
        for (const auto collider_component : game::colliders)
        {
            if(physics_component->entity == collider_component->entity) continue;
            
            if(collision->aabb(physics_component->entity->get_component<class collider_component>(), *collider_component))
            {
                physics_component->entity->get_component<class collider_component>().collided(*collider_component, collider_component->collision_tag);
                
                if(!collider_component->trigger)
                    physics_component->entity->get_component<transform_component>().position = physics_component->late_pos;
            }
        }
    }
}