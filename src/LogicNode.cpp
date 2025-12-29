#include "LogicNode.hpp"

#include <boost/asio.hpp>

LogicNode::LogicNode(const LogicNode& other) noexcept
    : session(other.session), tlvPacket(std::make_unique<TLVPacket>(*other.tlvPacket)) {}

LogicNode::LogicNode(std::shared_ptr<Session> session, const TLVPacket& packet) noexcept
    : session(session), tlvPacket(std::make_unique<TLVPacket>(packet)) {}

LogicNode::LogicNode(std::shared_ptr<Session> session, TLVPacket&& packet) noexcept
    : session(session), tlvPacket(std::make_unique<TLVPacket>(std::forward<TLVPacket>(packet))) {}

LogicNode::LogicNode(LogicNode&& rvalue) noexcept
    : session(rvalue.session), tlvPacket(std::move(rvalue.tlvPacket)) {}

LogicNode& LogicNode::operator=(const LogicNode& lvalue) noexcept {
    if (&lvalue == this) return *this;
    session = lvalue.session;
    tlvPacket.reset(new TLVPacket(*lvalue.tlvPacket));
    return *this;
}

LogicNode& LogicNode::operator=(LogicNode&& rvalue) noexcept {
    if (&rvalue == this) return *this;
    session = rvalue.session;
    tlvPacket = std::move(rvalue.tlvPacket);
    return *this;
}
