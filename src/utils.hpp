/*
 * libgudevxx - a C++ wrapper for libgudev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIBGUDEVXX_UTILS_HPP
#define LIBGUDEVXX_UTILS_HPP

#include <cstddef>
#include <vector>

#include <glib.h>


namespace gudev::utils {

    template<typename T>
    std::vector<T>
    gobj_list_to_vector(GList* list)
    {
        try  {
            std::vector<T> result;
            for (GList* i = list; i; i = i->next)
                result.push_back(reinterpret_cast<T>(i->data));
            g_list_free(list);
            return result;
        }
        catch (...) {
            for (GList* i = list; i; i = i->next)
                g_object_unref(i->data);
            g_list_free(list);
            throw;
        }
    }


    template<typename T = std::string>
    inline
    std::vector<T>
    strv_to_vector(const char* const strv[])
    {
        std::vector<T> result;
        for (std::size_t i = 0; strv[i]; ++i)
            result.push_back(strv[i]);
        return result;
    }

} // namespace gudev::utils

#endif
