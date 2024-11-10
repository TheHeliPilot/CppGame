#pragma once
#include <winsock2.h>
#include <iostream>
#include <vector>

#include "vector2.h"
#include "ECS/ecs.h"

class entity;
class button_script;
class transform_component;

struct network_data
{
    int data_size;
    entity_id entity_index;
    const char* flag;
    char* data;
};


struct transform_replication_data
{
    vector2 pos;
    int scale;
    float rotation;
};

class network_handler
{
public:
    static std::vector<network_data> last_data;
    
    static void send_data(const SOCKET s, const char* data, const int flags = 0);

    static char* rcv_data(const SOCKET s);

    static void receive_data_thread(const SOCKET& connect_socket, std::vector<network_data>& last_data_, bool& is_receiving, const bool& should_listen);
    static void process_received_data(entity* player, const entity* client, const entity* button, const entity* plate1);

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
        // Calculate the total length needed including separators (e.g., commas) and null terminator
        const size_t flag_length = std::strlen(data.flag);
        const size_t data_length = data.data_size;
        const size_t total_length = flag_length + data_length + 1 + 10 + 1 + 10 + 1; // +1 for separators (commas), +10 for max int length, +1 for null

        // Allocate memory for the serialized string
        const auto serialized = static_cast<char*>(malloc(total_length));
        if (!serialized) {
            return nullptr; // Allocation failed
        }

        // Serialize the data into the allocated memory
        std::snprintf(serialized, total_length, "%s,%d,%d,%s", data.flag, data.data_size, data.entity_index, data.data);

        // Return the serialized result
        return serialized;
    }

    static network_data deserialize_network_data(const char* serialized)
    {
        network_data data{0, 0, "", nullptr};
        
        // First comma position
        const char* pos = std::strchr(serialized, ',');
        if (!pos) {
            std::cerr << "Error: Malformed input, missing first comma." << '\n';
            return data;
        }
        
        // Calculate the length of the flag
        const size_t flag_length = pos - serialized;
        
        // Allocate memory for the flag and copy the flag data
        const auto flag = new char[flag_length + 1];
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
        const size_t size_str_length = next_pos - pos;
        const auto size_str = new char[size_str_length + 1];
        std::strncpy(size_str, pos, size_str_length);
        size_str[size_str_length] = '\0';
        
        // Convert the `size_str` to integer for `data_size`
        data.data_size = std::atoi(size_str);
        delete[] size_str;
        
        // Move to the `entity_index` component
        next_pos++;
        
        // Find the third comma
        pos = std::strchr(next_pos, ',');
        if (!pos) {
            std::cerr << "Error: Malformed input, missing third comma." << '\n';
            delete[] flag;
            return data;
        }
        
        // Extract the `entity_index` string
        const size_t index_str_length = pos - next_pos;
        const auto index_str = new char[index_str_length + 1];
        std::strncpy(index_str, next_pos, index_str_length);
        index_str[index_str_length] = '\0';
        
        // Convert the `index_str` to integer for `entity_index`
        data.entity_index = std::atoi(index_str);
        delete[] index_str;
        
        // Move to the `data` component
        pos++;
        
        // Extract the `data`
        const size_t data_length = std::strlen(pos);
        data.data = new char[data_length + 1];
        std::strncpy(data.data, pos, data_length);
        data.data[data_length] = '\0';
        
        return data;
    }

    static char* serialize_object_replication_data(const transform_replication_data& obj)
    {
        // Calculate the required buffer size
        constexpr size_t buffer_size = sizeof(vector2) + sizeof(float) + sizeof(float);
        const auto buffer = new char[buffer_size];

        // Copy data into the buffer
        size_t offset = 0;
        std::memcpy(buffer + offset, &obj.pos, sizeof(vector2));
        offset += sizeof(vector2);
        std::memcpy(buffer + offset, &obj.scale, sizeof(float));
        offset += sizeof(float);
        std::memcpy(buffer + offset, &obj.rotation, sizeof(float));

        return buffer;
    }

    static transform_replication_data deserialize_object_replication_data(const char* buffer)
    {
        transform_replication_data obj;

        // Extract data from the buffer
        size_t offset = 0;
        std::memcpy(&obj.pos, buffer + offset, sizeof(vector2));
        offset += sizeof(vector2);
        std::memcpy(&obj.scale, buffer + offset, sizeof(float));
        offset += sizeof(float);
        std::memcpy(&obj.rotation, buffer + offset, sizeof(float));

        return obj;
    }

    static char* serialize_uint32_t(const uint32_t value) {
        // Allocate a buffer of 4 bytes
        const auto buffer = new char[sizeof(uint32_t)];
    
        // Copy the uint32_t value into the buffer
        std::memcpy(buffer, &value, sizeof(uint32_t));
    
        return buffer;
    }

    static uint32_t deserialize_uint32_t(const char* buffer) {
        uint32_t value;
    
        // Copy the bytes from the buffer into the uint32_t variable
        std::memcpy(&value, buffer, sizeof(uint32_t));
    
        return value;
    }
    
    static char* make_data(const char* flag, char* data, const int entity_index = -1)
    {
        const auto ser_data = new network_data();
        ser_data->data_size = sizeof(data);
        ser_data->flag = flag;
        ser_data->data = data;
        ser_data->entity_index = entity_index;
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
