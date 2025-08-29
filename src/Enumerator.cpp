/*
 *  libgudevxx - a C++ wrapper for libgudev
 *
 *  Copyright (C) 2025  Daniel K. O.
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "gudevxx/Enumerator.hpp"

#include "utils.hpp"


namespace gudev {

    Enumerator::Enumerator(Client& client) :
        Base{g_udev_enumerator_new(client.gobj()), true}
    {}


    void
    Enumerator::match_subsystem(const std::string &subsystem)
    {
        g_udev_enumerator_add_match_subsystem(gobj(), subsystem.c_str());
    }


    void
    Enumerator::nomatch_subsystem(const std::string &subsystem)
    {
        g_udev_enumerator_add_nomatch_subsystem(gobj(), subsystem.c_str());
    }


    void
    Enumerator::match_sysfs_attr(const std::string &key,
                                 const std::string &val)
    {
        g_udev_enumerator_add_match_sysfs_attr(gobj(), key.c_str(), val.c_str());
    }


    void
    Enumerator::nomatch_sysfs_attr(const std::string &key,
                                   const std::string &val)
    {
        g_udev_enumerator_add_nomatch_sysfs_attr(gobj(), key.c_str(), val.c_str());
    }


    void
    Enumerator::match_property(const std::string &key,
                               const std::string &val)
    {
        g_udev_enumerator_add_match_property(gobj(), key.c_str(), val.c_str());
    }


    void
    Enumerator::match_name(const std::string &name)
    {
        g_udev_enumerator_add_match_name(gobj(), name.c_str());
    }


    void
    Enumerator::match_tag(const std::string &tag)
    {
        g_udev_enumerator_add_match_tag(gobj(), tag.c_str());
    }


    void
    Enumerator::match_initialized()
    {
        g_udev_enumerator_add_match_is_initialized(gobj());
    }


    void
    Enumerator::add_sysfs_path(const std::filesystem::path &sysfs_path)
    {
        g_udev_enumerator_add_sysfs_path(gobj(), sysfs_path.c_str());
    }


    std::vector<Device>
    Enumerator::execute()
    {
        GList* list = g_udev_enumerator_execute(gobj());
        auto devs = utils::gobj_list_to_vector<GUdevDevice*>(list);
        std::vector<Device> result;
        for (auto& d : devs)
            result.push_back(Device::own(d));
        return result;
    }

} // namespace gudev
