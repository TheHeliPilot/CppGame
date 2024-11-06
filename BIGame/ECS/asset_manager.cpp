#include "asset_manager.h"
#include "components.h"

asset_manager::asset_manager(class manager* m) : manager_(m) {}
asset_manager::~asset_manager() = default;

entity* asset_manager::instantiate_asset(vector2 pos, float scale) const
{
    auto& e(manager_->add_entity());
    e.add_component<transform_component>(pos, scale);
    return &e;
}
