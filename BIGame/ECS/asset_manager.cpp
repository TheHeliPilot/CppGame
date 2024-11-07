#include "asset_manager.h"
#include "components.h"

asset_manager::asset_manager(class manager* m) : manager_(m) {}
asset_manager::~asset_manager() = default;

std::vector<entity> asset_manager::instantiated_entities;

entity* asset_manager::instantiate(vector2 pos, float scale, const std::string& image) const
{
    entity e = game::manager_.add_entity();
    instantiated_entities.push_back(e);
    e.add_component<transform_component>(pos, scale);
    if(image != "NO_IMAGE")
        e.add_component<sprite_component>(image);
    return &e;
}
