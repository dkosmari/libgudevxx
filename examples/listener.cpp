/*
 *  libgudevxx - a C++ wrapper for libgudev
 *
 *  Copyright (C) 2025  Daniel K. O.
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

/*------------------------------------------------------.
| This is a test program for the gudevxx::Client class. |
|                                                       |
| It acts like 'udevadm monitor'.                       |
|                                                       |
| Usage:                                                |
|                                                       |
|     ./test-listener [subsystem ...]                   |
|                                                       |
| Examples:                                             |
|                                                       |
|     ./test-listener                                   |
|                                                       |
|     ./test-listener input                             |
|                                                       |
|     ./test-listener usb bluetooth                     |
`------------------------------------------------------*/


#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <signal.h>

#include <glib.h>
#include <glib-unix.h>
#include <glibmm.h>

#include <gudevxx/Client.hpp>

using std::cout;
using std::endl;
using std::setw;
using std::string;
using std::vector;

using gudev::Client;
using gudev::Device;


gboolean
handle_sigint(Glib::MainLoop* loop)
{
    cout << "\nCaught Ctrl-C." << endl;
    loop->quit();
    return false;
}


struct MyClient : Client {

    using Client::Client;

    void
    on_uevent(const string& action,
              Device& device)
        override
    {
        cout << setw(6) << action;

        if (auto subsystem = device.subsystem())
            cout << " | " << *subsystem;

        if (auto name = device.name())
            cout << " | " << *name;

        if (auto file = device.device_file())
            cout << " | " << file->string();
        else if (auto sysfs = device.sysfs())
            cout << " | " << sysfs->string();

        if (auto seqnum = device.seqnum())
            cout << " | " << *seqnum;

        cout << endl;
    }

};


int
main(int argc,
     char* argv[])
{
    Glib::init();

    vector<string> filters;
    for (int i=1; i<argc; ++i)
        filters.push_back(argv[i]);

    auto main_loop = Glib::MainLoop::create();

    g_unix_signal_add(SIGINT,
                      G_SOURCE_FUNC(handle_sigint),
                      main_loop.get());


    MyClient client{filters};

    main_loop->run();

    cout << "exiting" << endl;
}
