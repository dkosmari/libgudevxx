/*
 *  libgudevxx - a C++ wrapper for libgudev
 *
 *  Copyright (C) 2025  Daniel K. O.
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIBGUDEVXX_GOBJECT_BASE_HPP
#define LIBGUDEVXX_GOBJECT_BASE_HPP


#include <memory>
#include <stdexcept>

#include <glib.h>


namespace gudev::detail {

    
    template<typename CType,
             typename Derived>
    class GObjectBase {        

        struct Deleter {
            void operator ()(CType* obj) const
            {
                g_object_unref(obj);
            }
        };

        std::unique_ptr<CType, Deleter> ptr;

        
    protected:
        
        GObjectBase(CType* obj,
                    bool steal) :
            ptr{obj}
        {
            if (!gobj())
                throw std::logic_error{"Constructing from a null pointer."};
            if (!steal)
                g_object_ref(gobj());
        }


    public:

        static
        Derived
        own(CType* obj)
        {
            return Derived{obj, true};
        }


        static
        Derived
        view(CType* obj)
        {
            return Derived{obj, false};
        }


        CType*
        gobj() const
        {
            return ptr.get();
        }


        CType*
        gobj()
        {
            return ptr.get();
        }

        
    };

}


#endif
