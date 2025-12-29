#pragma once

#include "Session.hpp"
#include "PacketNode.hpp"

class LogicNode {
    friend class LogicSystem;
public:
    LogicNode() noexcept = default;
    LogicNode(const LogicNode& other) noexcept;
    LogicNode(std::shared_ptr<Session> session, const TLVPacket& node) noexcept;
    LogicNode(std::shared_ptr<Session> session, TLVPacket&& node) noexcept;
    LogicNode(LogicNode&& rvalue) noexcept;

    LogicNode& operator=(const LogicNode& lvalue) noexcept;
    LogicNode& operator=(LogicNode&& rvalue) noexcept;

    uint16_t getTag() { return tlvPacket->getTag(); }
    uint16_t getLength() { return tlvPacket->getLength(); }
    std::string_view getMessage() { return tlvPacket->getMessage(); }

public:
    std::shared_ptr<Session> session;
    std::unique_ptr<TLVPacket> tlvPacket;
};
