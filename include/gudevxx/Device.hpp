/*
 *  libgudevxx - a C++ wrapper for libgudev
 *  Copyright (C) 2021-2023  Daniel K. O.
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef LIBGUDEVXX_DEVICE_HPP
#define LIBGUDEVXX_DEVICE_HPP


#include <chrono>
#include <concepts>
#include <cstdint>
#include <filesystem>
#include <iosfwd>
#include <optional>
#include <string>
#include <vector>

#include <gudev/gudev.h>

#include "detail/GObjectBase.hpp"


namespace gudev {

    class Device :
        public detail::GObjectBase<GUdevDevice, Device> {

    public:


        using Base = detail::GObjectBase<GUdevDevice, Device>;

        enum class Type {
            no_device    = G_UDEV_DEVICE_TYPE_NONE,
            block_device = G_UDEV_DEVICE_TYPE_BLOCK,
            char_device  = G_UDEV_DEVICE_TYPE_CHAR
        };


        std::optional<std::string>
        subsystem() const;

        std::optional<std::string>
        devtype() const;

        std::optional<std::string>
        name() const;

        std::optional<std::string>
        number() const;

        std::optional<std::filesystem::path>
        sysfs() const;

        std::optional<std::string>
        driver() const;

        std::optional<std::string>
        action() const;

        std::optional<std::uint64_t>
        seqnum() const;

        Type type() const;

        std::optional<std::uint64_t>
        device_number() const;

        std::optional<std::filesystem::path>
        device_file() const;

        std::vector<std::filesystem::path>
        device_symlinks() const;

        std::optional<Device>
            parent() const;

        std::optional<Device>
        parent(const std::string& subsystem) const;

        std::optional<Device>
        parent(const std::string& subsystem,
               const std::string& devtype) const;

        std::vector<std::string>
        tags() const;

        bool
        has_tag(const std::string& tag) const;

        bool
        initialized() const;

        std::chrono::microseconds
        since_initialized() const;

        std::vector<std::string>
        property_keys() const;

        bool
        has_property(const std::string& key) const;

        std::optional<std::string>
        property(const std::string& key) const;

        // T = int, uint64_t, double, bool, string
        template<typename T>
        T
        property_as(const std::string& key) const;

        std::vector<std::string>
        property_tokens(const std::string& key) const;


        std::vector<std::string>
        sysfs_attr_keys() const;

        bool
        has_sysfs_attr(const std::string& key) const;

        std::optional<std::string>
        sysfs_attr(const std::string& key) const;

        // T = int, uint64_t, double, bool, string
        template<typename T>
        T
        sysfs_attr_as(const std::string& key) const;

        std::vector<std::string>
        sysfs_attr_tokens(const std::string& key) const;


    private:

        using Base::Base;


    }; // class Device


    std::string
    to_string(Device::Type type);


    std::ostream&
    operator <<(std::ostream& out,
                Device::Type type);


} // namespace gudev


#endif
