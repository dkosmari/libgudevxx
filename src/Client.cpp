/*
 * libgudevxx - a C++ wrapper for libgudev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: GPL-3.0-or-later
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



    Client::Client()
    {
        create();
    }


    Client::Client(std::nullptr_t)
        noexcept
    {}


    Client::Client(const std::vector<std::string>& subsystems)
    {
        create(subsystems);
    }


    void
    Client::create()
    {
        auto ptr = g_udev_client_new(nullptr);
        if (!ptr)
            throw std::runtime_error{"Could not create new GUdevClient"};
        destroy();
        acquire(ptr);
        connect_uevent_handler();
    }


    void
    Client::create(const std::vector<std::string>& subsystems)
    {
        auto ptr = make_filter_client(subsystems);
        if (!ptr)
            throw std::runtime_error{"Could not create new GUdevClient"};
        destroy();
        acquire(ptr);
        connect_uevent_handler();
    }


    void
    Client::destroy()
        noexcept
    {
        disconnect_uevent_handler();
        BaseType::destroy();
    }


    Client::Client(Client&& other)
        noexcept = default;


    Client&
    Client::operator =(Client&& other)
        noexcept = default;


    void
    Client::connect_uevent_handler()
        noexcept
    {
        if (raw)
            g_signal_connect(raw,
                             "uevent",
                             G_CALLBACK(dispatch_uevent_signal),
                             nullptr);
    }


    void
    Client::disconnect_uevent_handler()
        noexcept
    {
        if (!raw)
            return;
        guint n = g_signal_handlers_disconnect_by_func(raw,
                                                       reinterpret_cast<gpointer>(dispatch_uevent_signal),
                                                       nullptr);
        if (n != 1)
            g_warning("Client::disconnect_uevent_handler() disconnected: %u\n", n);
    }


    /*------------------*/
    /* query operations */
    /*------------------*/


    std::vector<Device>
    Client::query(const std::string& subsystem)
    {
        const char* arg = subsystem.empty() ? nullptr : subsystem.data();
        GList* list = g_udev_client_query_by_subsystem(raw,
                                                       arg);
        auto vec = utils::gobj_list_to_vector<GUdevDevice*>(list);
        std::vector<Device> result;
        for (auto& d : vec)
            result.push_back(Device::make_owner(d));
        return result;
    }


    std::optional<Device>
    Client::get(const std::string& subsystem,
                const std::string& name)
    {
        auto d = g_udev_client_query_by_subsystem_and_name(raw,
                                                           subsystem.c_str(),
                                                           name.c_str());
        if (d)
            return Device::make_owner(d);
        return {};
    }


    std::optional<Device>
    Client::get(GUdevDeviceType type,
                GUdevDeviceNumber number)
    {
        auto d = g_udev_client_query_by_device_number(raw,
                                                      type,
                                                      number);
        if (d)
            return Device::make_owner(d);
        return {};
    }


    std::optional<Device>
    Client::get(const std::filesystem::path &device_path)
    {
        auto d = g_udev_client_query_by_device_file(raw,
                                                    device_path.c_str());
        if (d)
            return Device::make_owner(d);
        return {};
    }


    std::optional<Device>
    Client::get_sysfs(const std::filesystem::path &sysfs_path)
    {
        auto d = g_udev_client_query_by_sysfs_path(raw,
                                                   sysfs_path.c_str());
        if (d)
            return Device::make_owner(d);
        return {};
    }


    Client*
    Client::get_wrapper(GUdevClient* cli)
        noexcept
    {
        return BaseType::get_wrapper<Client>(cli);
    }


    Client
    Client::make_alias(GUdevClient* cli)
        noexcept
    {
        Client result{nullptr};
        result.alias(cli);
        return result;
    }


    Client
    Client::make_owner(GUdevClient* cli)
        noexcept
    {
        return Client{cli};
    }


    void
    Client::on_uevent(const std::string& /*action*/,
                      Device& /*device*/)
    {}


    void
    Client::dispatch_uevent_signal(GUdevClient* cli,
                                   gchar*       act,
                                   GUdevDevice* dev,
                                   gpointer     /* data */)
        noexcept
    {
        try {
            Client* client = get_wrapper(cli);
            if (!client) {
                g_warning("Could not find C++ wrapper for %p\n", cli);
                return;
            }
            std::string action = act;
            Device* device_ptr = Device::get_wrapper(dev);
            if (device_ptr) {
                client->on_uevent(act, *device_ptr);
                if (client->uevent_callback)
                    client->uevent_callback(action, *device_ptr);
            } else {
                auto device = Device::make_alias(dev);
                client->on_uevent(action, device);
                if (client->uevent_callback)
                    client->uevent_callback(action, device);
            }
        }
        catch (std::exception& e) {
            g_warning("Exception in signal handler: %s\n", e.what());
        }
    }

} // namespace gudev
