/*
 * libgudevxx - a C++ wrapper for libgudev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "gudevxx/Enumerator.hpp"

#include "utils.hpp"


namespace gudev {

    Enumerator::Enumerator(std::nullptr_t)
        noexcept
    {}


    Enumerator::Enumerator(Client& client)
    {
        create(client);
    }


    void
    Enumerator::create(Client& client)
    {
        auto ptr = g_udev_enumerator_new(client.data());
        if (!ptr)
            throw std::runtime_error{"Could not create new GUdevEnumerator"};
        destroy();
        acquire(ptr);
    }


    Enumerator::Enumerator(Enumerator&& other)
        noexcept = default;


    Enumerator&
    Enumerator::operator =(Enumerator&& other)
        noexcept = default;


    Enumerator&
    Enumerator::match_subsystem(const std::string& subsystem)
    {
        g_udev_enumerator_add_match_subsystem(raw, subsystem.data());
        return *this;
    }


    Enumerator&
    Enumerator::nomatch_subsystem(const std::string& subsystem)
    {
        g_udev_enumerator_add_nomatch_subsystem(raw, subsystem.data());
        return *this;
    }


    Enumerator&
    Enumerator::match_sysfs_attr(const std::string& key,
                                 const std::string& val)
    {
        g_udev_enumerator_add_match_sysfs_attr(raw, key.data(), val.data());
        return *this;
    }


    Enumerator&
    Enumerator::nomatch_sysfs_attr(const std::string& key,
                                   const std::string& val)
    {
        g_udev_enumerator_add_nomatch_sysfs_attr(raw, key.data(), val.data());
        return *this;
    }


    Enumerator&
    Enumerator::match_property(const std::string& key,
                               const std::string& val)
    {
        g_udev_enumerator_add_match_property(raw, key.data(), val.data());
        return *this;
    }


    Enumerator&
    Enumerator::match_name(const std::string& name)
    {
        g_udev_enumerator_add_match_name(raw, name.data());
        return *this;
    }


    Enumerator&
    Enumerator::match_tag(const std::string& tag)
    {
        g_udev_enumerator_add_match_tag(raw, tag.data());
        return *this;
    }


    Enumerator&
    Enumerator::match_initialized()
    {
        g_udev_enumerator_add_match_is_initialized(raw);
        return *this;
    }


    Enumerator&
    Enumerator::add_sysfs_path(const std::filesystem::path& sysfs_path)
    {
        g_udev_enumerator_add_sysfs_path(raw, sysfs_path.c_str());
        return *this;
    }


    std::vector<Device>
    Enumerator::execute()
    {
        GList* list = g_udev_enumerator_execute(raw);
        auto devs = utils::gobj_list_to_vector<GUdevDevice*>(list);
        std::vector<Device> result;
        for (auto& d : devs)
            result.push_back(Device::make_owner(d));
        return result;
    }

} // namespace gudev
