#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class component;
class entity;

using component_id = size_t;

inline component_id get_component_type_id()
{
    static component_id last_id = 0;
    return last_id++;
}

template <typename T> inline component_id get_component_type_id() noexcept
{
    static_assert(std::is_base_of<component, T>::value, "");
    static component_id type_id = get_component_type_id();
    return type_id;
}

constexpr size_t max_components = 32;

using component_bit_set = std::bitset<max_components>;
using component_array = std::array<component*, max_components>;

class component
{
public:
    entity* entity;

    virtual void init();
    virtual void update();
    virtual void draw();

    virtual ~component() = default;
};

inline void component::init()
{
}

inline void component::update()
{
}

inline void component::draw()
{
}

class entity
{
private:
    bool active_ = true;
    std::vector<std::unique_ptr<component>> components;
    
    component_array component_array_ = {};
    component_bit_set bit_set_;

public:
    
    void update()
    {
        for (const auto& component : components) component->update();
    }
    void draw()
    {
        for (const auto& component : components) component->draw();
    }
    bool is_active() const { return active_; }
    void set_active(bool active) { active_ = active; }
    void destroy() {active_ = false;}

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
public:
    void update()
    {
        for (auto& e : entities) e->update();
    }
    void draw()
    {
        for (auto& e : entities) e->draw();
    }
    void refresh()
    {
        entities.erase(std::remove_if(begin(entities), end(entities),
            [](const std::unique_ptr<entity> &m_entity)
            {
                return !m_entity->is_active();
            }), end(entities));
    }

    entity& add_entity()
    {
        entity* e(new entity());
        std::unique_ptr<entity> u_ptr{e};
        entities.emplace_back(std::move(u_ptr));
        return *e;
    }
    
    std::vector<std::unique_ptr<entity>> entities;
};