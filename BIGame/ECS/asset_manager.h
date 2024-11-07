#pragma once

#include <memory>
#include <vector>

class vector2;
class entity;
class manager;

class asset_manager
{
public:
    explicit asset_manager(manager* m);
    ~asset_manager();
    
    static std::vector<std::unique_ptr<entity>> instantiate_entities_buffer;

    static entity* instantiate(vector2 pos, float scale, bool replicates, uint32_t id = -1);
    static void create_buffered_entities();
    static entity* instantiate(vector2 pos, float scale, bool replicates, float rotation);
    static entity* instantiate(vector2 pos, float scale, bool replicates, const char* image);
    struct object_replication_data;

private:
    manager* manager_;
};
