/*
 *  libgudevxx - a C++ wrapper for libgudev
 *  Copyright (C) 2021-2023  Daniel K. O.
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdexcept>
#include <utility>

#include "gudevxx/Client.hpp"

#include "utils.hpp"


namespace gudev {

    namespace {

        GUdevClient*
        make_filter_client(const std::vector<std::string>& subsystems)
        {
            std::vector<const char*> filter;
            for (auto& s : subsystems)
                filter.push_back(s.data());
            filter.push_back(nullptr);
            return g_udev_client_new(filter.data());
        }

    } // namespace



    Client::Client() :
        BaseType{g_udev_client_new(nullptr), detail::Purpose::adopt}
    {}


    Client::Client(std::nullptr_t)
        noexcept
    {}


    Client::Client(const std::vector<std::string>& subsystems) :
        BaseType{make_filter_client(subsystems), detail::Purpose::adopt}
    {}


    Client::~Client()
    {
        if (is_valid())
            g_signal_handler_disconnect(data(), uevent_handler);
    }


    Client::Client(Client&& other)
        noexcept = default;


    Client&
    Client::operator =(Client&& other)
        noexcept = default;


    gulong
    Client::connect_uevent()
    {
        return g_signal_connect(data(),
                                "uevent",
                                G_CALLBACK(dispatch_uevent_signal),
                                nullptr);
    }


    /*------------------*/
    /* query operations */
    /*------------------*/


    std::vector<Device>
    Client::query(const std::string& subsystem)
    {
        const char* arg = subsystem.empty() ? nullptr : subsystem.data();
        GList* list = g_udev_client_query_by_subsystem(data(),
                                                       arg);
        auto vec = utils::gobj_list_to_vector<GUdevDevice*>(list);
        std::vector<Device> result;
        for (auto& d : vec)
            result.push_back(Device{d, detail::Purpose::adopt});
        return result;
    }


    std::optional<Device>
    Client::get(const std::string& subsystem,
                const std::string& name)
    {
        auto d = g_udev_client_query_by_subsystem_and_name(data(),
                                                           subsystem.c_str(),
                                                           name.c_str());
        if (d)
            return Device{d, detail::Purpose::adopt};
        return {};
    }


    std::optional<Device>
    Client::get(GUdevDeviceType type,
                GUdevDeviceNumber number)
    {
        auto d = g_udev_client_query_by_device_number(data(),
                                                      type,
                                                      number);
        if (d)
            return Device{d, detail::Purpose::adopt};
        return {};
    }


    std::optional<Device>
    Client::get(const std::filesystem::path &device_path)
    {
        auto d = g_udev_client_query_by_device_file(data(),
                                                    device_path.c_str());
        if (d)
            return Device{d, detail::Purpose::adopt};
        return {};
    }


    std::optional<Device>
    Client::get_sysfs(const std::filesystem::path &sysfs_path)
    {
        auto d = g_udev_client_query_by_sysfs_path(data(),
                                                   sysfs_path.c_str());
        if (d)
            return Device{d, detail::Purpose::adopt};
        return {};
    }


    Client*
    Client::get_wrapper(GUdevClient* c)
        noexcept
    {
        return BaseType::get_wrapper<Client>(c);
    }


    void
    Client::on_uevent(const std::string& /*action*/,
                      Device& /*device*/)
    {}


    void
    Client::dispatch_uevent_signal(GUdevClient* client_,
                                   gchar*       action_,
                                   GUdevDevice* device_,
                                   gpointer     /* data */)
    {
        Client* client = get_wrapper(client_);
        if (!client) {
            g_warning("Could not find C++ wrapper for %p\n", client_);
            return;
        }
        std::string action = action_;
        Device* device_ptr = Device::get_wrapper(device_);
        if (device_ptr) {
            client->on_uevent(action, *device_ptr);
            if (client->uevent_callback)
                client->uevent_callback(action, *device_ptr);
        } else {
            Device device{device_, detail::Purpose::view};
            client->on_uevent(action, device);
            if (client->uevent_callback)
                client->uevent_callback(action, device);
        }
    }

}
