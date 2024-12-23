# C++ Game (engine?) with SDL and Winsocks

## Overview
This is a C++ game (more of an engine) created using the SDL graphics library for rendering and Winsocks for networking. The game features a Client-Server architecture, basic collision detection, and a simple interaction system.

## Gameplay
- **Controls**:
  - `W` - Move Up
  - `S` - Move Down
  - `A` - Move Left
  - `D` - Move Right
  - `Space` - Interact

- **How to Play**:
  1. Run BIGame with true/false for server/client parameters. (only 1 client can connect!)

- **Game Scene**:
  - Contains two interactable buttons.
  - Includes a pressure plate that controls a light.

## Main Features

### Entity-Component System (ECS)
The game employs an Entity-Component System (ECS) for efficient object and component handling.

### Networking
- **Client-Server Connection**:
  - No server authority model. Clients handle their own interactions and states.
  - Network communication uses flags and possible object IDs for data management. This allows for a generalized protocol to handle similar events with specific identifiers.

- **Asynchronous Data Reception**:
  - Network data is received on a different thread as Winsock `recv` waits till data arrives, ensuring smooth gameplay without blocking main game logic.

### Collision Detection
- Implements Axis-Aligned Bounding Box (AABB) collisions to ensure entities interact properly within the game world.

## Network Protocol
- **Data Sending**:
  - Utilizes flags for different events. For instance, a `button_pressed` flag is used with an object ID to identify which button was pressed, minimizing the need for multiple flags for similar actions.

- **Data Receiving**:
  - Handled on a different thread to ensure continuous gameplay and prevent delays.

## Setup
1. Ensure that both `server.exe` and `client.exe` are in the same network. (localhost, I did not make the network changable by parameters)
2. Run BIGame with true/false for server/client parameters. (only 1 client can connect!)

## Dependencies
- SDL graphics library
- Winsocks for networking

## Future Improvements
- Prefabs to allow network entity instantiation.
- Implement multiple client support.
- Enhance collision detection.
