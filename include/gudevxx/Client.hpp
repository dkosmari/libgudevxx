/*
 *  libgudevxx - a C++ wrapper for libgudev
 *
 *  Copyright (C) 2025  Daniel K. O.
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIBGUDEVXX_CLIENT_HPP
#define LIBGUDEVXX_CLIENT_HPP

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <gudev/gudev.h>

#include "Device.hpp"
#include "GObjectBase.hpp"


namespace gudev {

    class Client :
        public detail::GObjectBase<GUdevClient, Client> {

    public:

        using Base = detail::GObjectBase<GUdevClient, Client>;


        Client(); // don't listen on any subsystem
        Client(const std::vector<std::string>& subsystems);

        virtual
        ~Client();


        // query operations

        std::vector<Device>
        query(const std::string& subsystem = "");

        std::optional<Device>
        get(const std::string& subsystem,
            const std::string& name);

        std::optional<Device>
        get(GUdevDeviceType type,
            std::uint64_t number);

        std::optional<Device>
        get(const std::filesystem::path& device_path);

        std::optional<Device>
        get_sysfs(const std::filesystem::path& sysfs_path);


        // callback for "uevent" signal
        std::function<void (const std::string&,
                            const Device& device)> uevent_callback;

    protected:

        // or override this method
        virtual
        void
        on_uevent(const std::string& action,
                  const Device& device);

    private:

        gulong uevent_handler = connect_uevent();

        gulong
        connect_uevent();

        static
        void
        dispatch_uevent_signal(GUdevClient* client_,
                               gchar*       action_,
                               GUdevDevice* device_,
                               gpointer     data);

    }; // class Client

} // namespace gudev


#endif
