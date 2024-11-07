#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <unordered_map>

//#include "components.h"

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
    uint32_t id_;
    // ReSharper disable once CppInconsistentNaming
    manager& manager;
    bool active_ = true;
    std::vector<std::unique_ptr<component>> components;

    component_array component_array_ = {};
    component_bit_set bit_set_;
    group_bit_set group_bit_set_;

public:
    entity(class manager& manager, const uint32_t id) : id_(id), manager(manager) {}
    
    void update() {
        //std::cout << "Updating inside entity with ID: " << id_ << '\n';
        try {
            for (const auto& component : components) {
                if (component) {
                    component->update();
                    //std::cout << "Updating component of type: " << typeid(*component).name() << '\n';
                }
            }
            for (const auto& component : components) {
                if (component) {
                    component->late_update();
                    //std::cout << "Late updating component of type: " << typeid(*component).name() << '\n';
                }
            }
        } catch (const std::exception& ex) {
            std::cerr << "Exception during update of entity ID " << id_ << ": " << ex.what() << '\n';
        } catch (...) {
            std::cerr << "Unknown exception during update of entity ID " << id_ << '\n';
        }
        //std::cout << "Ending update inside entity with ID: " << id_ << '\n';
    }

    void draw() {
        //std::cout << "Drawing entity ID " << id_ << '\n';
        try {
            for (const auto& component : components) {
                if (component) {
                    //std::cout << "Drawing component of type: " << typeid(*component).name() << '\n';
                    component->draw();
                }
            }
        } catch (const std::exception& ex) {
            std::cerr << "Exception during draw of entity ID " << id_ << ": " << ex.what() << '\n';
        } catch (...) {
            std::cerr << "Unknown exception during draw of entity ID " << id_ << '\n';
        }
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

    bool has_group(const group group) const
    {
        return group_bit_set_[group];
    }

    void add_group(group group);
    void destroy_group(const group group)
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
        T* c = new T(std::forward<TArgs>(m_args)...);
        c->entity = this;
        std::unique_ptr<component> u_ptr{c};
        components.emplace_back(std::move(u_ptr));

        component_array_[get_component_type_id<T>()] = c;
        bit_set_[get_component_type_id<T>()] = true;

        //std::cout << "Component added: " << typeid(T).name() << " to entity ID " << id_ << '\n';
        c->init();
        return *c;
    }

    template <typename T>
    T& get_component() const
    {
        auto ptr = component_array_[get_component_type_id<T>()];
        if (ptr == nullptr)
        {
            throw std::runtime_error("Component not found for the requested type");
        }
        return *static_cast<T*>(ptr);
    }

    uint32_t get_id() const { return id_; }
    
};

class manager
{
private:
    std::array<std::vector<entity*>, max_groups> grouped_elements_;
    static std::unordered_map<uint32_t, entity*> entity_map_;
    uint32_t next_id_ = 0;

public:
    std::vector<std::unique_ptr<entity>> entities_;

    void update()
    {
        //std::cout << "Current entities in update: " << entities_.size() << '\n';
        
        try{
            for(auto& e : entities_) {
                if (e->is_active()) {
                    //std::cout << "Updating entity ID: " << e->get_id() << '\n';
                    e->update();
                } else {
                    //std::cout << "Entity ID " << e->get_id() << " is not active and will not be updated.\n";
                }
            }
        } catch (const std::exception &ex) {
            std::cerr << "Exception caught: " << ex.what() << '\n';
        } catch (...) {
            std::cerr << "Unknown error caught.\n";
        }
        //std::cout << "Ending update in manager\n";
    }
    void draw()
    {
        //std::cout << "Entities in draw loop: " << entities_.size() << '\n';

        for(auto& e : entities_) {
            if (e->is_active()) {
                //std::cout << "Drawing entity ID: " << e->get_id() << '\n';
                //e->draw();
            } else {
                //std::cout << "Entity ID " << e->get_id() << " is not active and will not be drawn.\n";
            }
        }
    }
    void refresh() {
        //std::cout << "Refreshing entities\n";
        for (auto i = 0u; i < max_groups; ++i) {
            auto& group = grouped_elements_[i];
            group.erase(std::remove_if(begin(group), end(group),
                [i](const entity* e) {
                    //std::cout << "Checking grouped entity ID " << e->get_id() << " for group " << i << '\n';
                    return !e->is_active() || !e->has_group(i);
                }), end(group));
        }

        entities_.erase(std::remove_if(begin(entities_), end(entities_),
            [](const std::unique_ptr<entity> &m_entity) {
                //std::cout << "Checking entity ID " << m_entity->get_id() << " for removal\n";
                return !m_entity->is_active();
            }), end(entities_));

        //std::cout << "Remaining active entities after refresh:\n";
        for (const auto& e : entities_) {
            //std::cout << "Entity ID: " << e->get_id() << '\n';
        }
    }

    void add_to_group(entity* entity, const group group)
    {
        grouped_elements_[group].emplace_back(entity);
    }

    std::vector<entity*>& get_group(const group group)
    {
        return grouped_elements_[group];
    }
    
    //entity& add_entity()
    //{
    //    const auto e(new entity(*this));
    //    std::unique_ptr<entity> u_ptr{e};
    //    entities_.emplace_back(std::move(u_ptr));
    //    return *e;
    //}

    entity& add_entity(std::unique_ptr<entity> e) {
        entity& e_ref = *e;
        //std::cout << "Entity created with ID " << e_ref.get_id() << '\n';

        e_ref.set_active(true);  // Ensure the entity is active

        entities_.emplace_back(std::move(e));

        // Use the entity's ID here, no need to handle next_id_ incrementation
        entity_map_[e_ref.get_id()] = &e_ref;

        //std::cout << "Entity added with ID " << e_ref.get_id() << '\n';
        //std::cout << "Entity state is_active: " << e_ref.is_active() << '\n';
        //std::cout << "Entities count: " << entities_.size() << '\n';

        for (const auto& entity : entities_) {
            //std::cout << "Entity ID: " << entity->get_id() << " | Active: " << entity->is_active() << '\n';
        }

        return e_ref;
    }

    static entity* get_entity_by_id(const uint32_t id)
    {
        const auto it = entity_map_.find(id);
        if (it != entity_map_.end())
        {
            return it->second;
        }
        return nullptr; // Return nullptr if the ID doesn't exist
    }

    uint32_t next_id() {
        return next_id_++;
    }
};