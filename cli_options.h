#ifndef CLI_OPTIONS_H
#define CLI_OPTIONS_H

#include <vector>
#include <string_view>

class cli_options
{
public:
    cli_options(int argc, char *argv[]);

    int input_manufacturer() const{
        return m_i_manufacturer;
    }
    std::string_view input_machine() const{
        return m_i_machine;
    }
    int output_manufacturer() const{
        return m_i_manufacturer;
    }
    std::string_view output_machine() const{
        return m_i_machine;
    }
    const std::vector<std::string_view> input_files() const{
        return m_input_files;
    }
    const std::vector<std::string_view> output_files() const{
        return m_output_files;
    }
    bool verbose() const{
        return m_verbose;
    }
    bool help() const{
        return m_help;
    }

private:
    int m_i_manufacturer = -1;
    std::string_view m_i_machine;
    int m_o_manufacturer = -1;
    std::string_view m_o_machine;
    std::vector<std::string_view> m_input_files;
    std::vector<std::string_view> m_output_files;
    bool m_verbose = false;
    bool m_help = false;
};

#endif // CLI_OPTIONS_H
