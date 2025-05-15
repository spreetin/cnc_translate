#include "cli_options.h"
#include "brands/brands.h"
#include <stdexcept>

cli_options::cli_options(int argc, char *argv[])
{
    auto parse_manu = [this](const std::string_view sv, bool input) -> bool{
        if (sv == "Traub"){
            (input) ? m_i_manufacturer : m_o_manufacturer = NCParser::Traub;
        } else if (sv == "Okuma"){
            (input) ? m_i_manufacturer : m_o_manufacturer = NCParser::Okuma;
        } else {
            return false;
        }
        return true;
    };

    auto parse_files = [this](std::string_view sv) -> std::vector<std::string_view>{
        std::vector<std::string_view> items;
        while (auto pos = sv.find(',')){
            items.push_back(sv.substr(0, pos));
            sv = sv.substr(pos+1);
        }
        items.push_back(sv);
        return items;
    };

    bool get_manu = false, get_machine = false, get_output = false,
        get_omanu = false, get_omachine = false;
    const std::vector<std::string_view> args(argv + 1, argv + argc);
    for (const auto &arg : args){
        if (get_manu){
            if (!parse_manu(arg, true)){
                throw std::runtime_error("Unknown manufacturer name");
            }
            get_manu = false;
        } else if (get_machine){
            m_i_machine = arg;
            get_machine = false;
        } else if (get_omanu){
            if (!parse_manu(arg, false)){
                throw std::runtime_error("Unknown manufacturer name");
            }
            get_omanu = false;
        } else if (get_omachine){
            m_o_machine = arg;
            get_omachine = false;
        } else if (get_output){
            m_output_files = parse_files(arg);
            get_output = false;
        } else if (arg == "-v" || arg == "--verbose"){
            m_verbose = true;
        } else if (arg == "-h" || arg == "--help"){
            m_help = true;
        } else if (arg == "-in" || arg.starts_with("--input-manufacturer")){
            auto has_equals = arg.find('=');
            if (has_equals != std::string_view::npos){
                auto man = arg.substr(has_equals+1);
                if (!parse_manu(man, true)){
                    throw std::runtime_error("Unknown manufacturer name");
                }
            } else {
                get_manu = true;
            }
        } else if (arg == "-om" || arg.starts_with("--output-machine")){
            auto has_equals = arg.find('=');
            if (has_equals != std::string_view::npos){
                m_o_machine = arg.substr(has_equals+1);
            } else {
                get_omachine = true;
            }
        } else if (arg == "-on" || arg.starts_with("--output-manufacturer")){
            auto has_equals = arg.find('=');
            if (has_equals != std::string_view::npos){
                auto man = arg.substr(has_equals+1);
                if (!parse_manu(man, false)){
                    throw std::runtime_error("Unknown manufacturer name");
                }
            } else {
                get_omanu = true;
            }
        } else if (arg == "-im" || arg.starts_with("--input-machine")){
            auto has_equals = arg.find('=');
            if (has_equals != std::string_view::npos){
                m_i_machine = arg.substr(has_equals+1);
            } else {
                get_machine = true;
            }
        } else if (arg == "-o" || arg.starts_with("--output")){
            auto has_equals = arg.find('=');
            if (has_equals != std::string_view::npos){
                auto man = arg.substr(has_equals+1);
                m_output_files = parse_files(man);
            } else {
                get_output = true;
            }
        } else {
            m_input_files = parse_files(arg);
        }
    }
}
