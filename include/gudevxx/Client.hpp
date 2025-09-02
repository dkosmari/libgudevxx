/*
 * libgudevxx - a C++ wrapper for libgudev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIBGUDEVXX_CLIENT_HPP
#define LIBGUDEVXX_CLIENT_HPP

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include <gudev/gudev.h>

#include "Device.hpp"
#include "GObjectWrapper.hpp"


namespace gudev {

    class Client :
        public detail::GObjectWrapper<GUdevClient> {

        using BaseType = detail::GObjectWrapper<GUdevClient>;

    public:

        /// Default constructor: don't listen to any events
        Client();

        /// Construct invalid (null) client.
        Client(std::nullptr_t)
            noexcept;

        /// Listen events for subsystems
        Client(const std::vector<std::string>& subsystems);


        void
        create();

        void
        create(const std::vector<std::string>& subsystems);


        void
        destroy()
            noexcept override;


        /// Move constructor.
        Client(Client&& other)
            noexcept;

        /// Move assignment.
        Client&
        operator =(Client&& other)
            noexcept;


        // query operations

        std::vector<Device>
        query(const std::string& subsystem = "");

        std::optional<Device>
        get(const std::string& subsystem,
            const std::string& name);

        std::optional<Device>
        get(GUdevDeviceType type,
            GUdevDeviceNumber number);

        std::optional<Device>
        get(const std::filesystem::path& device_path);

        std::optional<Device>
        get_sysfs(const std::filesystem::path& sysfs_path);


        /// Callback for "uevent" signal.
        std::function<void (const std::string&, Device& device)> uevent_callback;


        static
        Client*
        get_wrapper(GUdevClient* cli)
            noexcept;


        static
        Client
        make_alias(GUdevClient* cli)
            noexcept;


        static
        Client
        make_owner(GUdevClient* cli)
            noexcept;


    protected:

        /// Virtual method for "uevent" signal.
        virtual
        void
        on_uevent(const std::string& action,
                  Device& device);

    private:

        // Inherit constructors.
        using BaseType::BaseType;


        void
        connect_uevent_handler()
            noexcept;

        void
        disconnect_uevent_handler()
            noexcept;


        static
        void
        dispatch_uevent_signal(GUdevClient* cli,
                               gchar*       act,
                               GUdevDevice* dev,
                               gpointer     data)
            noexcept;

    }; // class Client

} // namespace gudev


#endif
