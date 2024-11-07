#pragma once
#include <string>
#include <vector>

class vector2;
class entity;
class manager;

class asset_manager
{
public:
    asset_manager(manager* m);
    ~asset_manager();
    
    static std::vector<entity> instantiated_entities = {};

    entity* instantiate(vector2 pos, float scale = 1, const std::string& image = "NO_IMAGE") const;

private:
    manager* manager_;
};
