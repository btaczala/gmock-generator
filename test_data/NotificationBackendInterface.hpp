#ifndef NOTIFICATIONBACKENDINTERFACE_HPP_GQ7J3RIO
#define NOTIFICATIONBACKENDINTERFACE_HPP_GQ7J3RIO

#include <cstdint>
#include <functional>

struct NotificationBackendInterface {
    using OnNotify = std::function<std::uint32_t(
        // clang-format off
        const std::string&,  /*app_name*/ 
        const std::string&,  /*summary*/ 
        const std::string&,  /*body*/ 
        std::uint32_t,       /*replaces_id*/
        const std::string&,  /*app_icon*/
        std::int32_t         /*expiration_timeout*/
        // clang-format on
        )>;

    virtual ~NotificationBackendInterface() = default;
    virtual void setOnNotify(const OnNotify& onNotify) = 0;
};

#endif /* end of include guard: NOTIFICATIONBACKENDINTERFACE_HPP_GQ7J3RIO */
