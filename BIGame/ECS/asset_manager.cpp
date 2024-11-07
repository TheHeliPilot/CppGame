#include "asset_manager.h"
#include "components.h"



asset_manager::asset_manager(class manager* m) : manager_(m) {}
asset_manager::~asset_manager() = default;

std::vector<std::unique_ptr<entity>> asset_manager::instantiate_entities_buffer;

entity* asset_manager::instantiate(vector2 pos, float scale, const bool replicates, uint32_t id) {
    //std::cout << "Buffering entity to be instantiated at position: (" << pos.x << ", " << pos.y << ") with scale: " << scale << '\n';

    //TODO networked object spawning -> PREFABS??
    //if(id == static_cast<uint32_t>(-1))
    id = game::manager_.next_id();  // Generate next ID
//
    //if(game::is_server_)
    //{
    //    network_handler::send_data(game::connect_socket, network_handler::make_data("object_instantiate", network_handler::serialize_uint32_t(id)));
    //}
    //else {
    //    
    //    return nullptr;
    //}
    
    auto e = std::make_unique<entity>(game::manager_, id);
    e->add_component<transform_component>(pos, scale);
    e->get_component<transform_component>().replicates = replicates;
    instantiate_entities_buffer.push_back(std::move(e));

    return instantiate_entities_buffer.back().get();
}

entity* asset_manager::instantiate(vector2 pos, float scale, const bool replicates, float rotation) {
    auto id = game::manager_.next_id();  // Generate next ID
    auto e = std::make_unique<entity>(game::manager_, id);
    e->add_component<transform_component>(pos, scale, rotation);
    e->get_component<transform_component>().replicates = replicates;
    instantiate_entities_buffer.push_back(std::move(e));

    return instantiate_entities_buffer.back().get();
}

entity* asset_manager::instantiate(vector2 pos, float scale, const bool replicates, const char* image) {
    auto id = game::manager_.next_id();  // Generate next ID
    auto e = std::make_unique<entity>(game::manager_, id);
    e->add_component<transform_component>(pos, scale);
    e->get_component<transform_component>().replicates = replicates;
    e->add_component<sprite_component>(image);
    instantiate_entities_buffer.push_back(std::move(e));

    return instantiate_entities_buffer.back().get();
}

void asset_manager::create_buffered_entities() {
    for (auto& buffered_entity : instantiate_entities_buffer) {
        auto& created_entity = game::manager_.add_entity(std::move(buffered_entity));
        //std::cout << "Buffered entity with ID " << created_entity.get_id() << " instantiated.\n";
    }

    // Clear the buffer after processing
    instantiate_entities_buffer.clear();
}