#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Manager;
class Entity;

using EntityID = int;

inline EntityID getNewID()
{
    static EntityID lastID = 1;
    return lastID++;
}

using Group = std::size_t;
using ComponentID = std::size_t;

inline ComponentID getNewComponentTypeID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

constexpr std::size_t maxGroups = 32;
constexpr std::size_t maxComponents = 32;

using GroupBitSet = std::bitset<maxGroups>;
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component *, maxComponents>;

class Component
{
public:
    Entity *entity;

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}

    virtual ~Component() {}
};

class Entity
{
private:
    int id;
    Manager &manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;

    ComponentBitSet componentBitSet;
    ComponentArray componentArray;
    GroupBitSet groupBitSet;

public:
    Entity(Manager &mManager) : manager(mManager)
    {
        id = getNewID();

        char string[100];
        sprintf(string, "#%05d", id);
        std::cout << "Entity: " << string << std::endl;
    }

    void update()
    {
        for (auto &c : components)
            c->update();
    }

    void draw()
    {
        for (auto &c : components)
            c->draw();
    }
    void destroy() { active = false; }
    bool isActive() const { return active; }

    bool hasGroup(Group mGroup)
    {
        return groupBitSet[mGroup];
    }

    void addGroup(Group mGroup);
    void delGroup(Group mGroup)
    {
        groupBitSet[mGroup] = false;
    }

    template <typename T>
    bool hasComponent() const
    {
        return componentBitSet[getComponentTypeID<T>];
    }

    template <typename T, typename... TArgs>
    T &addComponent(TArgs &&...mArgs)
    {
        T *c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    template <typename T>
    T &getComponent() const
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T *>(ptr);
    }

    ~Entity()
    {
        char sid[100];
        sprintf(sid, "#%05d", id);
        std::cout << "~Entity: " << sid << std::endl;
    }
};

class Manager
{
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity *>, maxGroups> groupedEntities;

public:
    void update()
    {
        for (auto &e : entities)
            e->update();
    }
    void draw()
    {
        for (auto &e : entities)
            e->draw();
    }

    void refresh()
    {
        for (auto i(0u); i < maxGroups; i++)
        {
            auto &v(groupedEntities[i]);
            v.erase(
                std::remove_if(
                    std::begin(v), std::end(v),
                    [i](Entity *mEntity)
                    {
                        return !mEntity->isActive() || !mEntity->hasGroup(i);
                    }),
                std::end(v));
        }

        entities.erase(
            std::remove_if(
                std::begin(entities), std::end(entities),
                [](const std::unique_ptr<Entity> &mEntity)
                {
                    return !mEntity->isActive();
                }),
            std::end(entities));
    }

    void AddToGroup(Entity *mEntity, Group mGroup)
    {
        groupedEntities[mGroup].emplace_back(mEntity);
    }

    std::vector<Entity *> &getGroup(Group mGroup)
    {
        return groupedEntities[mGroup];
    }

    Entity &addEntity()
    {
        Entity *e = new Entity(*this);
        std::unique_ptr<Entity> uPtr{e};
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
};