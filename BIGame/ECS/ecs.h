#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class component;
class entity;
class manager;

using component_id = size_t;
using group = size_t;

inline component_id get_new_component_type_id()
{
    static component_id last_id = 0u;
    return last_id++;
}

template <typename T> inline component_id get_component_type_id() noexcept
{
    static_assert(std::is_base_of<component, T>::value, "");
    static component_id type_id = get_new_component_type_id();
    return type_id;
}

constexpr size_t max_components = 32;
constexpr size_t max_groups = 32;

using component_bit_set = std::bitset<max_components>;
using group_bit_set = std::bitset<max_groups>;

using component_array = std::array<component*, max_components>;

class component
{
public:
    entity* entity;

    virtual void init();
    virtual void update();
    virtual void late_update();
    virtual void draw();

    virtual ~component() = default;
};

inline void component::init()
{
}

inline void component::update()
{
}

inline void component::late_update()
{
}

inline void component::draw()
{
}

class entity
{
private:
    // ReSharper disable once CppInconsistentNaming
    manager& manager;
    bool active_ = true;
    std::vector<std::unique_ptr<component>> components;

    component_array component_array_ = {};
    component_bit_set bit_set_;
    group_bit_set group_bit_set_;

public:
    entity(class manager& manager) : manager(manager) {}
    
    void update()
    {
        for (const auto& component : components) component->update();
        for (const auto& component : components) component->late_update();
    }
    void draw()
    {
        for (const auto& component : components) component->draw();
    }
    bool is_active() const { return active_; }
    void set_active(const bool active) { active_ = active; }
    void destroy()
    {
        remove_all_components();
        active_ = false;
    }
    
    void remove_all_components()
    {
        components.clear();
        component_array_.fill(nullptr);
        bit_set_.reset();
    }

    bool has_group(group group) const
    {
        return group_bit_set_[group];
    }

    void add_group(group group);
    void destroy_group(group group)
    {
        group_bit_set_[group] = false;
    }

    template <typename T> bool has_component() const
    {
        return bit_set_[get_component_type_id<T>()];
    }

    template <typename T, typename ... TArgs>
    T& add_component(TArgs&&... m_args)
    {
        T* c(new T(std::forward<TArgs>(m_args)...));
        c->entity = this;
        std::unique_ptr<component> u_ptr{c};
        components.emplace_back(std::move(u_ptr));

        component_array_[get_component_type_id<T>()] = c;
        bit_set_[get_component_type_id<T>()] = true;

        c->init();
        return *c;
    }

    template <typename T> T& get_component() const
    {
        auto ptr(component_array_[get_component_type_id<T>()]);
        return *static_cast<T*>(ptr);
    }
    
};

class manager
{
private:
    std::vector<std::unique_ptr<entity>> entities_;
    std::array<std::vector<entity*>, max_groups> grouped_elements_;

public:
    void update()
    {
        for (auto& e : entities_) e->update();
    }
    void draw()
    {
        for (auto& e : entities_) e->draw();
    }
    void refresh()
    {
        for(auto i(0u); i<max_groups; ++i)
        {
            auto& group(grouped_elements_[i]);
            group.erase(std::remove_if(begin(group), end(group), [i](entity* e)
            {
                return !e->is_active() || !e->has_group(i);
            }), end(group));
        }
        
        entities_.erase(std::remove_if(begin(entities_), end(entities_),
            [](const std::unique_ptr<entity> &m_entity)
            {
                return !m_entity->is_active();
            }), end(entities_));
    }

    void add_to_group(entity* entity, group group)
    {
        grouped_elements_[group].emplace_back(entity);
    }

    std::vector<entity*>& get_group(group group)
    {
        return grouped_elements_[group];
    }
    
    entity& add_entity()
    {
        const auto e(new entity(*this));
        std::unique_ptr<entity> u_ptr{e};
        entities_.emplace_back(std::move(u_ptr));
        return *e;
    }
    
};