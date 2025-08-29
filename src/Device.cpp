/*
 *  libgudevxx - a C++ wrapper for libgudev
 *
 *  Copyright (C) 2025  Daniel K. O.
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <algorithm>

#include "gudevxx/Device.hpp"

#include "utils.hpp"


namespace gudev {

    std::optional<std::string>
    Device::subsystem() const
    {
        auto r = g_udev_device_get_subsystem(gobj());
        if (r)
            return r;
        return {};
    }


    std::optional<std::string>
    Device::devtype() const
    {
        auto r = g_udev_device_get_devtype(gobj());
        if (r)
            return r;
        return {};
    }


    std::optional<std::string>
    Device::name() const
    {
        auto r = g_udev_device_get_name(gobj());
        if (r)
            return r;
        return {};
    }


    std::optional<std::string>
    Device::number() const
    {
        auto r = g_udev_device_get_number(gobj());
        if (r)
            return r;
        return {};
    }


    std::optional<std::filesystem::path>
    Device::sysfs() const
    {
        auto r = g_udev_device_get_sysfs_path(gobj());
        if (r)
            return r;
        return {};
    }


    std::optional<std::string>
    Device::driver() const
    {
        auto r = g_udev_device_get_driver(gobj());
        if (r)
            return r;
        return {};
    }


    std::optional<std::string>
    Device::action() const
    {
        auto r = g_udev_device_get_action(gobj());
        if (r)
            return r;
        return {};
    }


    std::optional<std::uint64_t>
    Device::seqnum() const
    {
        auto s = g_udev_device_get_seqnum(gobj());
        if (s)
            return s;
        return {};
    }


    Device::Type
    Device::type() const
    {
        auto t = g_udev_device_get_device_type(gobj());
        return static_cast<Type>(t);
    }


    std::optional<std::uint64_t>
    Device::device_number() const
    {
        auto n = g_udev_device_get_device_number(gobj());
        if (n)
            return n;
        return {};
    }


    std::optional<std::filesystem::path>
    Device::device_file() const
    {
        auto f = g_udev_device_get_device_file(gobj());
        if (f)
            return f;
        return {};
    }


    std::vector<std::filesystem::path>
    Device::device_symlinks() const
    {
        auto a = g_udev_device_get_device_file_symlinks(gobj());
        return utils::strv_to_vector<std::filesystem::path>(a);
    }


    std::optional<Device>
    Device::parent() const
    {
        auto p = g_udev_device_get_parent(gobj());
        if (p)
            return Device::own(p);
        return {};
    }


    std::optional<Device>
    Device::parent(const std::string& subsystem) const
    {
        auto p = g_udev_device_get_parent_with_subsystem(gobj(),
                                                         subsystem.c_str(),
                                                         nullptr);
        if (p)
            return Device::own(p);
        return {};
    }


    std::optional<Device>
    Device::parent(const std::string& subsystem,
                   const std::string& devtype) const
    {
        auto p = g_udev_device_get_parent_with_subsystem(gobj(),
                                                         subsystem.c_str(),
                                                         devtype.c_str());
        if (p)
            return Device::own(p);
        return {};
    }


    std::vector<std::string>
    Device::tags() const
    {
        auto t = g_udev_device_get_tags(gobj());
        return utils::strv_to_vector(t);
    }


    bool
    Device::has_tag(const std::string& tag) const
    {
        auto list = tags();
        return find(list.begin(), list.end(), tag) != list.end();
    }


    bool
    Device::initialized() const
    {
        return g_udev_device_get_is_initialized(gobj());
    }


    std::chrono::microseconds
    Device::since_initialized() const
    {
        auto u = g_udev_device_get_usec_since_initialized(gobj());
        return std::chrono::microseconds{u};
    }



    std::vector<std::string>
    Device::property_keys() const
    {
        auto k = g_udev_device_get_property_keys(gobj());
        return utils::strv_to_vector(k);
    }


    bool
    Device::has_property(const std::string& key) const
    {
        return g_udev_device_has_property(gobj(), key.c_str());
    }


    std::optional<std::string>
    Device::property(const std::string& key) const
    {
        auto p = g_udev_device_get_property(gobj(), key.c_str());
        if (p)
            return p;
        return {};
    }


    template<>
    int
    Device::property_as<int>(const std::string& key) const
    {
        return g_udev_device_get_property_as_int(gobj(), key.c_str());
    }


    template<>
    std::uint64_t
    Device::property_as<std::uint64_t>(const std::string& key) const
    {
        return g_udev_device_get_property_as_uint64(gobj(), key.c_str());
    }


    template<>
    double
    Device::property_as<double>(const std::string& key) const
    {
        return g_udev_device_get_property_as_double(gobj(), key.c_str());
    }


    template<>
    bool
    Device::property_as<bool>(const std::string& key) const
    {
        return g_udev_device_get_property_as_boolean(gobj(), key.c_str());
    }


    template<>
    std::string
    Device::property_as<std::string>(const std::string& key) const
    {
        return property(key).value_or("");
    }


    std::vector<std::string>
    Device::property_tokens(const std::string& key) const
    {
        auto t = g_udev_device_get_property_as_strv(gobj(), key.c_str());
        return utils::strv_to_vector(t);
    }


    std::vector<std::string>
    Device::sysfs_attr_keys() const
    {
        auto k = g_udev_device_get_sysfs_attr_keys(gobj());
        return utils::strv_to_vector(k);
    }


    bool
    Device::has_sysfs_attr(const std::string& key) const
    {
        return g_udev_device_has_sysfs_attr(gobj(), key.c_str());
    }


    std::optional<std::string>
    Device::sysfs_attr(const std::string& key) const
    {
        auto a = g_udev_device_get_sysfs_attr(gobj(), key.c_str());
        if (a)
            return a;
        return {};
    }


    template<>
    int
    Device::sysfs_attr_as<int>(const std::string& key) const
    {
        return g_udev_device_get_sysfs_attr_as_int(gobj(), key.c_str());
    }


    template<>
    std::uint64_t
    Device::sysfs_attr_as<uint64_t>(const std::string& key) const
    {
        return g_udev_device_get_sysfs_attr_as_uint64(gobj(), key.c_str());
    }


    template<>
    double
    Device::sysfs_attr_as<double>(const std::string& key) const
    {
        return g_udev_device_get_sysfs_attr_as_double(gobj(), key.c_str());
    }


    template<>
    bool
    Device::sysfs_attr_as<bool>(const std::string& key) const
    {
        return g_udev_device_get_sysfs_attr_as_boolean(gobj(), key.c_str());
    }


    template<>
    std::string
    Device::sysfs_attr_as<std::string>(const std::string& key) const
    {
        return sysfs_attr(key).value_or("");
    }


    std::vector<std::string>
    Device::sysfs_attr_tokens(const std::string& key) const
    {
        auto t = g_udev_device_get_sysfs_attr_as_strv(gobj(), key.c_str());
        return utils::strv_to_vector(t);
    }



    std::string
    to_string(Device::Type type)
    {
        switch (type) {
            case Device::Type::no_device:
                return "no device file";
            case Device::Type::block_device:
                return "block_device";
            case Device::Type::char_device:
                return "character device";
            default:
                throw std::logic_error{"invalid device type"};
        }
    }


    std::ostream&
    operator<<(std::ostream& out,
               Device::Type type)
    {
        return out << to_string(type);
    }

} // namespace gudev
