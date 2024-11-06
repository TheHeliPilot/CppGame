#pragma once

class vector2;
class entity;
class manager;

class asset_manager
{
public:
    asset_manager(manager* m);
    ~asset_manager();
    
    entity* instantiate_asset(vector2 pos, float scale = 1) const;

private:
    manager* manager_;
};
