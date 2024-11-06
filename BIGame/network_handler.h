#pragma once
#include <winsock2.h>
#include <iostream>
#include <vector>

#include "vector2.h"

class entity;
class wall_script;
class transform_component;

struct network_data
{
    int data_size;
    const char* flag;
    char* data;
};

class network_handler
{
public:
    static std::vector<network_data> last_data;
    
    static void send_data(const SOCKET s, const char* data, const int flags = 0);

    static char* rcv_data(const SOCKET s);

    static void receive_data_thread(const SOCKET& connect_socket, std::vector<network_data>& last_data_, bool& is_receiving, const bool& should_listen);
    static void process_received_data(entity& player, entity& client, entity& wall);

    template <typename T, typename U>
    static bool is_of_type(const U& obj) {
        return typeid(obj) == typeid(T);
    }

    static char* serialize_bool(const bool& b) 
    {
        const auto serialized = new char[2];
        serialized[0] = b ? '1' : '0';
        serialized[1] = '\0';
        return serialized;
    }

    static bool deserialize_bool(const char* serialized) 
    {
        if (serialized == nullptr || std::strlen(serialized) != 1) 
        {
            throw std::invalid_argument("Invalid serialized boolean");
        }
        return serialized[0] == '1';
    }
    
    static char* serialize_vector2(const vector2& v)
    {
        const auto serialized = static_cast<char*>(malloc(42));
        if (serialized)
        {
            std::snprintf(serialized, 42, "%f,%f", v.x, v.y);
        }
        return serialized;
    }

#pragma warning(push)
#pragma warning(disable: 4996)

    static vector2 deserialize_vector2(const char* serialized)
    {
        float x, y;
        if (std::sscanf(serialized, "%f,%f", &x, &y) != 2)
        {
            // Handle error
            throw std::runtime_error("Invalid input string for deserialization");
        }
        return vector2(x, y);
    }

    static char* serialize_network_data(const network_data& data)
{
    // Calculate the total length needed including separator (e.g., comma) and null terminator
    const size_t flag_length = std::strlen(data.flag);
    const size_t data_length = data.data_size;
    const size_t total_length = flag_length + data_length + 1 + 10 + 1; // +1 for the first separator (comma), +10 for max int length, +1 for null

    // Allocate memory for the serialized string
    const auto serialized = static_cast<char*>(malloc(total_length));
    if (!serialized) {
        return nullptr; // Allocation failed
    }

    // Serialize the data into the allocated memory
    std::snprintf(serialized, total_length, "%s,%d,%s", data.flag, data.data_size, data.data);

    // Return the serialized result
    return serialized;
}

    static network_data deserialize_network_data(const char* serialized) {
        network_data data{0, "", nullptr};
    
        // Pointer to use for tokenizing
        const char* pos = std::strchr(serialized, ',');
        if (!pos) {
            std::cerr << "Error: Malformed input, missing first comma." << '\n';
            return data;
        }
    
        // Calculate the length of the flag
        size_t flag_length = pos - serialized;
    
        // Allocate memory for the flag and copy the flag data
        char* flag = new char[flag_length + 1];
        std::strncpy(flag, serialized, flag_length);
        flag[flag_length] = '\0';
        data.flag = flag;
    
        // Move to the `data_size` component
        pos++;
    
        // Find the second comma
        const char* next_pos = std::strchr(pos, ',');
        if (!next_pos) {
            std::cerr << "Error: Malformed input, missing second comma." << '\n';
            delete[] flag;
            return data;
        }
    
        // Extract the `data_size` string
        size_t size_str_length = next_pos - pos;
        char* size_str = new char[size_str_length + 1];
        std::strncpy(size_str, pos, size_str_length);
        size_str[size_str_length] = '\0';
    
        // Convert the `size_str` to integer for `data_size`
        data.data_size = std::atoi(size_str);
        delete[] size_str;
    
        // Move to the `data` component
        next_pos++;
    
        // Extract the `data`
        size_t data_length = std::strlen(next_pos);
        data.data = new char[data_length + 1];
        std::strncpy(data.data, next_pos, data_length);
        data.data[data_length] = '\0';
    
        return data;
    }

    /*
    template <typename T>
    static char* serialize(const T& object)
    {
        const auto buffer = new char[sizeof(T)];
        std::memcpy(buffer, &object, sizeof(T));
        std::cout << buffer << std::endl;
        return buffer;
    }

    template <typename T>
    static T deserialize(const char* buffer)
    {
        T object;
        std::memcpy(&object, buffer, sizeof(T));
        return object;
    }
    */

    static char* make_data(const char* flag, char* data)
    {
        const auto ser_data = new network_data();
        ser_data->data_size = sizeof(data);
        ser_data->flag = flag;
        ser_data->data = data;
        return serialize_network_data(*ser_data);
    }

    static network_data get_data(const char& data)
    {
        auto new_data = network_data();
        new_data = deserialize_network_data(&data);
        return new_data;
    }

    //static void update_replication(SOCKET s);
};
