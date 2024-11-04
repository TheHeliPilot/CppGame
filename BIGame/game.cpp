#include "game.h"

#include <ws2tcpip.h>

#include "texture_manager.h"
#include "ECS/keyboard_handler.h"
#include "ECS/components.h"

#define DEFAULT_PORT "2302"

//networking
struct addrinfo *result = NULL, *ptr = NULL, hints;
SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ConnectSocket = INVALID_SOCKET;
SOCKET ClientSocket;

// ReSharper disable once CppInconsistentNaming
SDL_Renderer* game::renderer = nullptr;
SDL_Event game::event;

manager game::manager_;
auto collision = new class collision;
std::vector<collider_component*> game::colliders;
std::vector<physics_component*> game::physics_components;

WSADATA game::wsa_data;

auto& player = game::manager_.add_entity(); 
auto& wall = game::manager_.add_entity(); 

game::game()
= default;

game::~game()
= default;

void game::init(const char* title, int x_pos, int y_pos, int width, int height, bool fullscreen)
{
    is_server_ = true;
    
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "SDL innit successful\n";
        
        window_ = SDL_CreateWindow(title, x_pos, y_pos, width, height, flags);
        if(window_)
            std::cout << "Window created\n";

        renderer = SDL_CreateRenderer(window_, -1, 0);
        if(renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            std::cout << "Renderer created\n";
        }
        
        is_running_ = true;
    }
    else
    {
        is_running_ = false;
    }

    //networking
    
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

        
        iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
        if (iResult != 0) {
            printf("getaddrinfo failed: %d\n", iResult);
            WSACleanup();
            return;
        }

        std::cout << "getaddrinfo done\n";

        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        
        if (ListenSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return;
        }

        std::cout << "Socket created\n";

        // Setup the TCP listening socket
        iResult = bind( ListenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
        if (iResult == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            return;
        }
        freeaddrinfo(result);

        std::cout << "Bind done\n";

        if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
            printf( "Listen failed with error: %ld\n", WSAGetLastError() );
            closesocket(ListenSocket);
            WSACleanup();
            return;
        }

        std::cout << "Listening on port " << DEFAULT_PORT << "\n";

        ClientSocket = INVALID_SOCKET;

        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
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
            printf("getaddrinfo failed: %d\n", iResult);
            WSACleanup();
            return;
        }

        std::cout << "getaddrinfo done\n";

        // Attempt to connect to the first address returned by
        // the call to getaddrinfo
        ptr=result;

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return;
        }
        std::cout << "Socket created\n";

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
        }

        // Should really try the next address returned by getaddrinfo
        // if the connect call failed
        // But for this simple example we just free the resources
        // returned by getaddrinfo and print an error message

        freeaddrinfo(result);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("Unable to connect to server!\n");
            WSACleanup();
            return;
        }
        std::cout << "Connected\n";
        
    }
    
    //objekty
    player.add_component<transform_component>();
    player.add_component<sprite_component>("assets/textures/player.png");
    player.add_component<collider_component>();
    player.add_component<keyboard_handler>();
    player.add_component<physics_component>(.7f);
    player.add_component<player_movement>(2);

    wall.add_component<transform_component>(100,100);
    wall.add_component<sprite_component>("assets/textures/player.png");
    wall.add_component<collider_component>();

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
    for (const auto physics_component : physics_components)
    {
        for (const auto collider_component : colliders)
        {
            if(physics_component->entity == collider_component->entity) continue;
            
            if(collision->aabb(physics_component->entity->get_component<class collider_component>(), *collider_component))
            {
                physics_component->entity->get_component<transform_component>().position = physics_component->late_pos;
            }
        }
    }
    
    manager_.refresh();
    manager_.update();
}

void game::render()
{
    SDL_RenderClear(renderer);

    manager_.draw();

    SDL_RenderPresent(renderer);
}

void game::clean()
{
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Vsetko clean";
}