#include "ecs.h"

std::unordered_map<unsigned int, entity*> manager::entity_map_;

void entity::add_group(const group group)
{
    group_bit_set_[group] = true;
    manager.add_to_group(this, group);
}