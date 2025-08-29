/*
 *  libgudevxx - a C++ wrapper for libgudev
 *
 *  Copyright (C) 2025  Daniel K. O.
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIBGUDEVXX_ENUMERATOR_HPP
#define LIBGUDEVXX_ENUMERATOR_HPP

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

#include <gudev/gudev.h>

#include "Client.hpp"
#include "GObjectWrapper.hpp"


namespace gudev {

    struct Enumerator :
        detail::GObjectWrapper<GUdevEnumerator> {

        using BaseType = detail::GObjectWrapper<GUdevEnumerator>;


        Enumerator(std::nullptr_t)
            noexcept;

        Enumerator(Client& client);


        Enumerator&
        match_subsystem(const std::string& subsystem);

        Enumerator&
        nomatch_subsystem(const std::string& subsystem);

        Enumerator&
        match_sysfs_attr(const std::string& key,
                         const std::string& val);

        Enumerator&
        nomatch_sysfs_attr(const std::string& key,
                           const std::string& val);

        Enumerator&
        match_property(const std::string& key,
                       const std::string& val);

        Enumerator&
        match_name(const std::string& name);

        Enumerator&
        match_tag(const std::string& tag);

        Enumerator&
        match_initialized();

        Enumerator&
        add_sysfs_path(const std::filesystem::path& sysfs_path);


        std::vector<Device>
        execute();

    };

} // namespace gudev

#endif
