/*
 *  libgudevxx - a C++ wrapper for libgudev
 *  Copyright (C) 2021-2023  Daniel K. O.
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef LIBGUDEVXX_ENUMERATOR_HPP
#define LIBGUDEVXX_ENUMERATOR_HPP


#include <filesystem>
#include <string>
#include <vector>

#include <gudev/gudev.h>

#include "Client.hpp"
#include "detail/GObjectBase.hpp"


namespace gudev {


    struct Enumerator : detail::GObjectBase<GUdevEnumerator, Enumerator> {

        using Base = detail::GObjectBase<GUdevEnumerator, Enumerator>;


        Enumerator(Client& client);


        void match_subsystem(const std::string& subsystem);

        void nomatch_subsystem(const std::string& subsystem);

        void match_sysfs_attr(const std::string& key,
                              const string& val);

        void nomatch_sysfs_attr(const std::string& key,
                                const std::string& val);

        void match_property(const std::string& key,
                            const std::string& val);

        void match_name(const std::string& name);

        void match_tag(const std::string& tag);

        void match_initialized();

        void add_sysfs_path(const std::filesystem::path& sysfs_path);


        std::vector<Device> execute();

    };


}

#endif
