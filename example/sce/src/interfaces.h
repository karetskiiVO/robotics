#pragma once

#ifndef INTERFACES_h
#define INTERFACES_h

#include <packet.h>
#include <nlohmann/json.hpp>

class IReceiver {
public:
    virtual void OnReceive (Packet) = 0;

    virtual ~IReceiver () = default;
};

class ITimeStamper {
public:
    virtual void OnTimeStamp (MeasureUnits::Time) = 0;

    virtual ~ITimeStamper () = default;
};

class IBlockCommunicator {
public:
    virtual ~IBlockCommunicator () = default;

private:
    virtual void OnReceive (Packet packet) = 0;

    friend void Send (IBlockCommunicator* block, Packet packet);
};

void Send (IBlockCommunicator* block, Packet packet);

class IConfigurator {
public:
    virtual void SetConfig (const nlohmann::json& config) = 0;
    void SetConfig (const std::string& rawconfig) {
        SetConfig(nlohmann::json::parse(rawconfig));
    }

    virtual nlohmann::json GetConfig () const = 0;

    virtual ~IConfigurator () = default;
};

#endif // interfaces.h