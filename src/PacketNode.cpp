#include "PacketNode.hpp"

#include <cstring>
#include <boost/stacktrace.hpp>
#include <spdlog/spdlog.h>

TLVPacket::TLVPacket() noexcept {
    data = new char[HEADER_SECTION + MAX_LENGTH]{};
}

TLVPacket::TLVPacket(uint16_t tag, const char* value, size_t valueLength) noexcept {
    if (valueLength > MAX_LENGTH) {
        spdlog::warn("Node is too long to send. It will be truncated.");
        valueLength = MAX_LENGTH;
    }

    this->data = new char[HEADER_SECTION + valueLength];
    *reinterpret_cast<uint16_t*>(this->data) = boost::asio::detail::socket_ops::host_to_network_short(tag);
    *reinterpret_cast<uint16_t*>(this->data + TAG_SECTION) = boost::asio::detail::socket_ops::host_to_network_short(valueLength);
    std::memcpy(this->data + HEADER_SECTION, value, valueLength);
}

TLVPacket::TLVPacket(uint16_t tag, std::string_view value) noexcept : TLVPacket(tag, value.data(), value.length()) {}

TLVPacket::~TLVPacket() noexcept {
    delete data;
}

TLVPacket::TLVPacket(const TLVPacket& other) noexcept {
    uint16_t valueLength = other.getLength();
    data = new char[HEADER_SECTION + valueLength];
    std::memcpy(data, other.data, HEADER_SECTION + valueLength);
}

TLVPacket::TLVPacket(TLVPacket&& rvalue) noexcept {
    data = rvalue.data;
    rvalue.data = nullptr;
}

TLVPacket& TLVPacket::operator=(const TLVPacket& other) noexcept {
    if (&other == this) return *this;
    delete data;
    uint16_t valueLength = other.getLength();
    data = new char[HEADER_SECTION + valueLength];
    std::memcpy(data, other.data, HEADER_SECTION + valueLength);
    return *this;
}

TLVPacket& TLVPacket::operator=(TLVPacket&& rvalue) noexcept {
    if (&rvalue == this) return *this;
    data = rvalue.data;
    rvalue.data = nullptr;
    return *this;
}
