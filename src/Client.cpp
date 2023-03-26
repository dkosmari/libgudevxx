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
                filter.push_back(s.c_str());
            filter.push_back(nullptr);
            return g_udev_client_new(filter.data());
        }

    }

    void
    Client::dispatch_uevent_signal(GUdevClient* /*client_*/,
                                   gchar*       action_,
                                   GUdevDevice* device_,
                                   gpointer     data)
    {
        Client* client = reinterpret_cast<Client*>(data);
        std::string action = action_;
        Device device = Device::view(device_);
        client->on_uevent(action, device);
        if (client->uevent_callback)
            client->uevent_callback(action, device);
    }


    /// Default constructor: don't listen to any events
    Client::Client() :
        Base{g_udev_client_new(nullptr), true}
    {}


    /// Listen events for subsystems
    Client::Client(const std::vector<std::string>& subsystems) :
        Base{make_filter_client(subsystems), true}
    {}


    Client::~Client()
    {
        g_signal_handler_disconnect(gobj(), uevent_handler);
    }


    gulong
    Client::connect_uevent()
    {
        return g_signal_connect(gobj(),
                                "uevent",
                                G_CALLBACK(dispatch_uevent_signal),
                                this);
    }


    /*------------------*/
    /* query operations */
    /*------------------*/


    std::vector<Device>
    Client::query(const std::string& subsystem)
    {
        const char* arg = subsystem.empty() ? nullptr : subsystem.c_str();
        GList* list = g_udev_client_query_by_subsystem(gobj(),
                                                       arg);
        auto vec = utils::gobj_list_to_vector<GUdevDevice*>(list);
        std::vector<Device> result;
        for (auto& d : vec)
            result.push_back(Device::own(d));
        return result;

    }


    std::optional<Device>
    Client::get(const std::string& subsystem,
                const std::string& name)
    {
        auto d = g_udev_client_query_by_subsystem_and_name(gobj(),
                                                           subsystem.c_str(),
                                                           name.c_str());
        if (d)
            return Device::own(d);
        return {};
    }


    std::optional<Device>
    Client::get(GUdevDeviceType type,
                std::uint64_t number)
    {
        auto d = g_udev_client_query_by_device_number(gobj(),
                                                      type,
                                                      number);
        if (d)
            return Device::own(d);
        return {};
    }


    std::optional<Device>
    Client::get(const std::filesystem::path &device_path)
    {
        auto d = g_udev_client_query_by_device_file(gobj(),
                                                    device_path.c_str());
        if (d)
            return Device::own(d);
        return {};
    }


    std::optional<Device>
    Client::get_sysfs(const std::filesystem::path &sysfs_path)
    {
        auto d = g_udev_client_query_by_sysfs_path(gobj(),
                                                   sysfs_path.c_str());
        if (d)
            return Device::own(d);
        return {};
    }


    void
    Client::on_uevent(const std::string& /*action*/,
                      const Device& /*device*/)
    {}

}
