#pragma once

#include <boost/asio.hpp>
#include <cstdint>
#include <string_view>

class TLVPacket final {
public:
    TLVPacket() noexcept;
    TLVPacket(uint16_t tag, const char* value, size_t length) noexcept;
    TLVPacket(uint16_t tag, std::string_view value) noexcept;
    ~TLVPacket() noexcept;

    TLVPacket(const TLVPacket& other) noexcept;
    TLVPacket(TLVPacket&& rvalue) noexcept;
    TLVPacket& operator=(const TLVPacket& other) noexcept;
    TLVPacket& operator=(TLVPacket&& rvalue) noexcept;

public:
    static constexpr int TAG_SECTION = sizeof(uint16_t);
    static constexpr int LENGTH_SECTION = sizeof(uint16_t);
    static constexpr int HEADER_SECTION = TAG_SECTION + LENGTH_SECTION;
    static constexpr uint16_t MAX_LENGTH = 0x1000;  // Maximun value section length
    std::uint16_t getTag() const { return boost::asio::detail::socket_ops::network_to_host_short(*reinterpret_cast<uint16_t*>(this->data)); }
    std::uint16_t getLength() const { return boost::asio::detail::socket_ops::network_to_host_short(*reinterpret_cast<uint16_t*>(this->data + TAG_SECTION)); }
    std::string_view getMessage() const { return std::string_view(data + HEADER_SECTION, getLength()); }
public:
    char* data;
};
