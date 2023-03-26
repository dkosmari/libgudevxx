/*
 *  libgudevxx - a C++ wrapper for libgudev
 *  Copyright (C) 2021-2023  Daniel K. O.
 *
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
#include "detail/GObjectBase.hpp"


namespace gudev {


    using std::optional;
    using std::string;
    using std::uint64_t;
    using std::vector;
    using std::filesystem::path;


    struct Client : detail::GObjectBase<GUdevClient, Client> {


        using Base = detail::GObjectBase<GUdevClient, Client>;


        Client(); // don't listen on any subsystem
        Client(const vector<string>& subsystems);

        virtual ~Client();


        // query operations

        vector<Device>
        query(const string& subsystem = "");

        optional<Device>
        get(const string& subsystem,
            const string& name);

        optional<Device>
        get(GUdevDeviceType type,
            uint64_t number);

        optional<Device>
        get(const path& device_path);

        optional<Device>
        get_sysfs(const path& sysfs_path);


        // callback for "uevent" signal
        std::function<void(const string&, const Device& device)> uevent_callback;


    protected:

        // or override this method
        virtual
        void
        on_uevent(const string& action,
                  const Device& device);


    private:

        using Base::Base;
        gulong uevent_handler = connect_uevent();

        gulong connect_uevent();

        static
        void
        dispatch_uevent_signal(GUdevClient* client_,
                               gchar*       action_,
                               GUdevDevice* device_,
                               gpointer     data);

    }; // class Client


} // namespace gudev


#endif
