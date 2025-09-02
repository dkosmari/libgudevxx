/*
 * Test program to print device info.
 */

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <locale>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

#include <glibmm.h>

#include <gudevxx/Client.hpp>
#include <gudevxx/Device.hpp>


using std::cerr;
using std::cout;
using std::endl;
using std::filesystem::path;
using std::optional;
using std::setw;
using std::string;

using namespace std::literals;

using Glib::OptionContext;
using Glib::OptionGroup;
using Glib::OptionEntry;
using gudev::Client;
using gudev::Device;


template<typename Rep,
         typename Ratio>
string
human_time(std::chrono::duration<Rep, Ratio> t)
{
    using std::to_string;
    using namespace std::chrono;

    if (t < 5000us)
        return to_string(duration_cast<microseconds>(t).count()) + " µs";
    if (t < 5000ms)
        return to_string(duration_cast<milliseconds>(t).count()) + " ms";
    if (t < 120s)
        return to_string(duration_cast<seconds>(t).count()) + " s";
    if (t < 120min)
        return to_string(duration_cast<minutes>(t).count()) + " min";
    if (t < 48h)
        return to_string(duration_cast<hours>(t).count()) + " h";
    return to_string(duration_cast<days>(t).count()) + " d";
}


std::vector<std::string>
split(const std::string& input,
      const std::string& delimiter)
{
    auto res = g_strsplit(input.data(), delimiter.data(), -1);
    std::vector<std::string> result;
    try {
        for (gchar** strp = res; *strp; ++strp)
            result.push_back(*strp);
        g_strfreev(res);
    }
    catch (...) {
        g_strfreev(res);
        throw;
    }
    return result;
}


void
print_device(const Device& dev)
{
    const string indent(20, ' ');
    const string indent2(40, ' ');

    using std::to_string;
    using gudev::to_string;

    auto aprint = [&indent](const string& left, const string& right = "")
    {
        if (indent.size() > left.size()) {
            size_t pad_size = indent.size() - left.size();
            string padding = indent.substr(0, pad_size);
            cout << padding;
        }
        cout << left << right << "\n";
    };
    auto print_opt = [&aprint](const string& label, const auto& val)
    {
        if (val) {
            using T = std::remove_reference_t<
                std::remove_cv_t<decltype(val.value())>>;

            if constexpr(std::is_convertible_v<T, std::string>)
                aprint(label, *val);
            else
                aprint(label, to_string(*val));
        }
    };

    auto aprint2 = [&indent2](const string& left, const string& right = "")
    {
        if (indent2.size() > left.size()) {
            size_t pad_size = indent2.size() - left.size();
            string padding = indent2.substr(0, pad_size);
            cout << padding;
        }
        cout << left;
        if (!left.empty() && !right.empty())
            cout << " = ";
        else
            cout << "   ";
        cout << right << "\n";
    };

    cout << "Device:\n";
    print_opt("Subsystem: ", dev.subsystem());
    print_opt("Devtype: ", dev.devtype());
    print_opt("Name: ", dev.name());
    print_opt("Number: ", dev.number());
    print_opt("SysFS: ", dev.sysfs());
    print_opt("Driver: ", dev.driver());
    print_opt("Action: ", dev.action());
    print_opt("Seqnum: ", dev.seqnum());
    print_opt("Device type: ", optional{dev.type()});
    print_opt("Device number: ", dev.device_number());

    if (dev.is_initialized())
        aprint("Initialized: ",
               human_time(dev.usec_since_initialized()) + " ago");

    print_opt("Device file: ", dev.device_file());

    if (auto symlinks = dev.device_symlinks(); !symlinks.empty()) {
        aprint("Symlinks: ", "");
        for (const auto& s : symlinks)
            aprint("", s.string());
    }

    if (auto tags = dev.tags(); !tags.empty()) {
        aprint("Tags: ");
        for (const auto& t : tags)
            aprint("", t);
    }

    // TODO: print with the same syntax as udev
    if (auto pkeys = dev.property_keys(); !pkeys.empty()) {
        aprint("Properties: ");
        for (const auto& k : pkeys)
            aprint2(k, dev.property_as<string>(k));
    }

    // TODO: print with the same syntax as udev
    if (auto akeys = dev.sysfs_attr_keys(); !akeys.empty()) {
        aprint("Sysfs attrs: ");
        for (const auto& k : akeys) {
            auto val = dev.sysfs_attr(k);
            if (val) {
                if (val->find('\n') == string::npos)
                    aprint2(k, *val);
                else {
                    aprint2(k, "{");
                    auto entries = split(*val, "\n");
                    for (auto& entry : entries)
                        aprint2("", "    " + entry);
                    aprint2("", "}");
                }
            } else {
                // only they key
                aprint2(k);
            }
        }
    }
}


int
main(int argc,
     char* argv[])
try {
    std::locale::global(std::locale(""));

    Glib::init();

    Glib::ustring subsystem;
    Glib::ustring file;
    bool show_parents = false;

    OptionContext opt_ctx;
    opt_ctx.set_ignore_unknown_options(false);

    OptionGroup grp{"options", "Main options"};

    OptionEntry sub_opt;
    sub_opt.set_long_name("subsystem");
    sub_opt.set_short_name('s');
    sub_opt.set_arg_description("SUBSYSTEM");
    sub_opt.set_description("Only list from selected subsystem.");
    grp.add_entry(sub_opt, subsystem);

    OptionEntry dev_opt;
    dev_opt.set_long_name("device");
    dev_opt.set_short_name('d');
    dev_opt.set_arg_description("FILE");
    dev_opt.set_description("Print device info from specified FILE.");
    grp.add_entry(dev_opt, file);

    OptionEntry par_opt;
    par_opt.set_long_name("parents");
    par_opt.set_short_name('p');
    par_opt.set_description("Show parent devices.");
    grp.add_entry(par_opt, show_parents);

    opt_ctx.set_main_group(grp);


    if (!opt_ctx.parse(argc, argv)) {
        cerr << "Unable to parse options." << endl;
        return -1;
    }

    if (argc > 1) {
        // Leftover arguments, treat as an error.
        for (int i = 1; i < argc; ++i)
            cout << "Unknown argument: " << argv[i] << "\n";
        cout << opt_ctx.get_help() << endl;
        return -1;
    }

    Client client;

    if (!file.empty()) {
        path dev_path = file.raw();
        auto dev_printer = [show_parents](const Device& d)
        {
            print_device(d);
            if (show_parents)
                for (auto p = d.parent(); p; p = p->parent()) {
                    cout << "\nParent:" << endl;
                    print_device(*p);
                }
        };

        if (auto d = client.get(dev_path))
            dev_printer(*d);
        else if (auto d = client.get_sysfs(dev_path))
            dev_printer(*d);
        else {
            cerr << "Could not access device " << dev_path << endl;
            return -1;
        }

        return 0;
    } else {
        // no remaining arguments, so just list all devices
        auto devices = client.query(subsystem);
        for (auto& d : devices)
            cout << d.subsystem().value_or("")
                 << " : "
                 << setw(5) << d.device_number().value_or(0)
                 << " : "
                 << d.device_file().value_or("").string()
                 << " : "
                 << d.sysfs().value_or("").string()
                 << endl;
        return 0;
    }
}
#if !GLIBMM_CHECK_VERSION(2, 68, 0)
catch (Glib::Exception& e) {
    cerr << e.what() << endl;
    return -2;
}
#endif
catch (std::exception& e) {
    cerr << e.what() << endl;
    return -1;
}
