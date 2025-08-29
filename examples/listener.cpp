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


#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <signal.h>

#include <glib.h>
#include <glib-unix.h>
#include <glibmm.h>

#include <gudevxx/Client.hpp>


using std::chrono::duration_cast;
using std::chrono::hours;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::minutes;
using std::chrono::seconds;
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


template<typename Rep,
         typename Ratio>
string
human_time(std::chrono::duration<Rep, Ratio> t)
{
    string result = std::to_string(t.count());

    if (auto u = duration_cast<microseconds>(t); u.count() <= 5000)
        result += " µs";
    else if (auto m = duration_cast<milliseconds>(t); m.count() < 5000)
        result += " ms";
    else if (auto s = duration_cast<seconds>(t); s.count() < 120)
        result += " s";
    else if (auto m = duration_cast<minutes>(t); m.count() < 120)
        result += " m";
    else
        result += " h";
    return result;
}


string
to_string(const Device& device)
{
    string result;

    if (auto subsystem = device.subsystem())
        result += *subsystem + " | ";

    if (auto name = device.name())
        result += *name + " | ";

    if (auto file = device.device_file())
        result += file->string() + " | ";
    else if (auto sysfs = device.sysfs())
        result += sysfs->string() + " | ";

    if (auto seqnum = device.seqnum())
        result += *seqnum;

    return result;
}


struct MyClient : Client {

    using Client::Client;

    void
    on_uevent(const string& action,
              Device& device)
        override
    {
        cout << setw(6) << action
             << " | "
             << to_string(device)
             << endl;
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
