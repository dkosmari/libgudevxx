/*
 * libgudevxx - a C++ wrapper for libgudev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIBGUDEVXX_GOBJECT_WRAPPER_HPP
#define LIBGUDEVXX_GOBJECT_WRAPPER_HPP

#include <tuple>
#include <utility>

#include <glib.h>
#include <glib-object.h>

#include "basic_wrapper.hpp"


namespace gudev::detail {

    template<typename CType>
    class GObjectWrapper :
        public basic_wrapper<CType*> {

        using BaseType = basic_wrapper<CType*>;

    public:

        void
        set_wrapper()
            noexcept
        {
            if (!this->is_valid())
                return;
            gpointer ptr = g_object_get_data(G_OBJECT(this->raw), "cpp-wrapper");
            if (ptr != nullptr)
                return;
            g_object_set_data(G_OBJECT(this->raw), "cpp-wrapper", this);
        }


        void
        unset_wrapper()
            noexcept
        {
            if (!this->is_valid())
                return;
            gpointer ptr = g_object_get_data(G_OBJECT(this->raw), "cpp-wrapper");
            if (this == reinterpret_cast<GObjectWrapper*>(ptr))
                g_object_set_data(G_OBJECT(this->raw), "cpp-wrapper", nullptr);
            else
                g_warning("Unsetting wrapper in object %p that did not have it.", this->raw);
        }


        void
        ref()
            noexcept
        {
            if (this->is_valid())
                g_object_ref(this->raw);
        }

    protected:


        // Inherit constructors.
        using BaseType::BaseType;


        GObjectWrapper(CType* obj) :
            BaseType{obj}
        {
            this->set_wrapper();
        }


        ~GObjectWrapper()
            noexcept
        {
            this->destroy();
        }


        /// Move constructor.
        GObjectWrapper(GObjectWrapper&& other)
            noexcept
        {
            this->acquire(other.release());
        }


        /// Move assignment.
        GObjectWrapper&
        operator =(GObjectWrapper&& other)
            noexcept
        {
            if (this != &other) {
                this->destroy();
                this->acquire(other.release());
            }
            return *this;
        }


        template<typename Derived>
        static
        Derived*
        get_wrapper(CType* raw)
        {
            if (!raw)
                return nullptr;
            gpointer ptr = g_object_get_data(G_OBJECT(raw), "cpp-wrapper");
            return reinterpret_cast<Derived*>(ptr);
        }

    public:

        void
        destroy()
            noexcept override
        {
            auto ptr = this->release();
            if (ptr)
                g_object_unref(ptr);
        }


        void
        acquire(BaseType::raw_type new_raw)
            noexcept
        {
            BaseType::acquire(new_raw);
            this->set_wrapper();
        }


        void
        alias(BaseType::raw_type new_raw)
            noexcept
        {
            BaseType::acquire(new_raw);
            this->set_wrapper();
            this->ref();
        }


        CType*
        release()
        {
            unset_wrapper();
            return {BaseType::release(), };
        }

    };

} // namespace gudev::detail

#endif
