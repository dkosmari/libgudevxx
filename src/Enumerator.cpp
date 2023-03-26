/*
 *  libgudevxx - a C++ wrapper for libgudev
 *  Copyright (C) 2021-2023  Daniel K. O.
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "gudevxx/Enumerator.hpp"

#include "utils.hpp"


namespace gudev {


    Enumerator::Enumerator(Client& client) :
        Base{g_udev_enumerator_new(client.gobj()), true}
    {}


    void
    Enumerator::match_subsystem(const string &subsystem)
    {
        g_udev_enumerator_add_match_subsystem(gobj(), subsystem.c_str());
    }


    void
    Enumerator::nomatch_subsystem(const string &subsystem)
    {
        g_udev_enumerator_add_nomatch_subsystem(gobj(), subsystem.c_str());
    }


    void
    Enumerator::match_sysfs_attr(const string &key, const string &val)
    {
        g_udev_enumerator_add_match_sysfs_attr(gobj(), key.c_str(), val.c_str());
    }


    void
    Enumerator::nomatch_sysfs_attr(const string &key, const string &val)
    {
        g_udev_enumerator_add_nomatch_sysfs_attr(gobj(), key.c_str(), val.c_str());
    }


    void
    Enumerator::match_property(const string &key, const string &val)
    {
        g_udev_enumerator_add_match_property(gobj(), key.c_str(), val.c_str());
    }


    void
    Enumerator::match_name(const string &name)
    {
        g_udev_enumerator_add_match_name(gobj(), name.c_str());
    }


    void
    Enumerator::match_tag(const string &tag)
    {
        g_udev_enumerator_add_match_tag(gobj(), tag.c_str());
    }


    void
    Enumerator::match_initialized()
    {
        g_udev_enumerator_add_match_is_initialized(gobj());
    }


    void
    Enumerator::add_sysfs_path(const path &sysfs_path)
    {
        g_udev_enumerator_add_sysfs_path(gobj(), sysfs_path.c_str());
    }


    vector<Device>
    Enumerator::execute()
    {
        GList* list = g_udev_enumerator_execute(gobj());
        auto devs = utils::gobj_list_to_vector<GUdevDevice*>(list);
        vector<Device> result;
        for (auto& d : devs)
            result.push_back(Device::own(d));
        return result;
    }


} // namespace gudev
