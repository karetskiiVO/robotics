#pragma once
#ifndef EMULATEDBLOCK_h
#define EMULATEDBLOCK_h

#include <vector>
#include <iostream>

#include <interfaces.h>

#include <utils/MeasureUnits/MeasureUnits.h>

class EmulatedBlock;

class Component {
protected:
    // TODO: безопасные указатели
    EmulatedBlock* emulatedBlock = nullptr;

    virtual ~Component() = default;
public:
    friend class EmulatedBlock;
};


class EmulatedBlock : public IBlockCommunicator {
  private:
    std::vector<Component*> components;

    std::vector<ITimeStamper*> timeStampers;
    std::vector<IReceiver*> receivers;

  public:
    template <typename ComponentType>
    ComponentType* AddComponent (ComponentType* component) {
        static_cast<Component*>(component)->emulatedBlock = this;

        /*
        ** Если компонент удовлетворяет некоторому интерфейсу, кладем указатель на нго в соответствующий буфер
        */

        if constexpr (std::is_base_of<ITimeStamper, ComponentType>::value) {
            timeStampers.push_back(component);
        }

        if constexpr (std::is_base_of<IReceiver, ComponentType>::value) {
            receivers.push_back(component);
        }

        components.push_back(component);
        return component;
    }

    template <typename ComponentType, typename... Args>
    ComponentType* AddComponent (Args... args) {
        auto component = new ComponentType(args...); // Не уверен на счет синтаксиса
        static_cast<Component*>(component)->emulatedBlock = this;

        /*
        ** Если компонент удовлетворяет некоторому интерфейсу, кладем указатель на нго в соответствующий буфер
        */

        if constexpr (std::is_base_of<ITimeStamper, ComponentType>::value) {
            timeStampers.push_back(component);
        }

        if constexpr (std::is_base_of<IReceiver, ComponentType>::value) {
            receivers.push_back(component);
        }

        components.push_back(component);
        return component;
    }

    template <typename ComponentType>
    ComponentType* GetComponent () {
        /*
        ** ищем в массиве компонентов первый удовлетворяющий типу
        */

        // TODO: оптимизация для интерфейсов "верхнего" уровня - специализацие темплайта например
        for (auto rawcomponent : components) {
            auto component = dynamic_cast<ComponentType*>(rawcomponent);
            if (component != nullptr) return component;
        }

        return nullptr;
    }

    template <typename ComponentType>
    std::vector<ComponentType*> GetComponents () {
        auto res = std::vector<ComponentType*>(); // Для того чтоб работало RVO в MSVC
        /*
        ** ищем в массиве компонентов все удовлетворяющие типу
        */

        for (auto rawcomponent : components) {
            auto component = dynamic_cast<ComponentType*>(rawcomponent);
            if (component != nullptr) res.push_back(component);
        }

        return res;
    }

    EmulatedBlock() = default;
    EmulatedBlock& operator=(const EmulatedBlock&)   = delete;
    EmulatedBlock& operator= (EmulatedBlock&& other) = default;
    EmulatedBlock (const EmulatedBlock&)             = delete;
    EmulatedBlock (EmulatedBlock&&)                  = default;

    ~EmulatedBlock () {
        for (auto component : components) delete component;
    }
private:
    void OnTimeStamp (MeasureUnits::Time timeStamp) {
        for (auto timeStamper : timeStampers) timeStamper->OnTimeStamp(timeStamp);
    }

    void OnReceive (Packet packet) {
        for (auto receiver : receivers) receiver->OnReceive(packet);
    }
};

#endif // emulatedblock.h
