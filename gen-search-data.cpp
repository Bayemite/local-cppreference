#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

std::string substitute(std::string str, const std::string &toReplace, const std::string &replacement)
{
    size_t index = str.find(toReplace);
    while (index != std::string::npos)
    {
        str.replace(index, toReplace.size(), replacement);
        index += replacement.size();
        index = str.find(toReplace, index);
    }

    return str;
}

std::string convert_html_escapes(std::string str)
{
    str = substitute(str, "&lt;", "<");
    str = substitute(str, "&gt;", ">");
    str = substitute(str, "&le;", "≤");
    str = substitute(str, "&ge;", "≥");
    return str;
}

std::string scrape_title(std::string buf)
{
    int titleStart = buf.find("<title>") + 7;
    int titleEnd = buf.find(" - cppreference.com</title>", titleStart);
    if (titleEnd == std::string::npos)
        titleEnd = buf.find("</title>", titleStart);

    auto title = buf.substr(titleStart, titleEnd - titleStart);
    title = convert_html_escapes(title);
    title = substitute(title, "\"", "\\\"");
    // Remove a strange artifact in the HTML
    title = substitute(title, "Â ", "");

    return title;
}

std::string scrape_href_keyword(std::string path)
{
    int keywordStart = path.find_last_of("/") + 1;
    int keywordEnd = path.find(".html");

    auto keyword = path.substr(keywordStart, keywordEnd - keywordStart);
    keyword = convert_html_escapes(keyword);

    return keyword;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Provide single argument for root directory path to recursively scan from.\n";
        return -1;
    }

    for (auto const &dir_entry : fs::recursive_directory_iterator(argv[1]))
    {
        if (dir_entry.is_regular_file())
        {
            auto filepath = dir_entry.path();
            std::ifstream file{filepath};
            if (!file)
                std::cout << "Can't open " + dir_entry.path().string() << '\n';

            const auto file_size = fs::file_size(filepath);
            std::string buf;
            buf.resize(file_size);
            file.read(buf.data(), file_size);

            auto path = substitute(filepath.string(), "\\", "/");
            std::string keyword = scrape_href_keyword(path);
            std::string title = scrape_title(buf);

            std::cout << "{keyword: \"" << keyword << "\","
                      << " title: \"" << title << "\","
                      << " href: \"" << path << "\"},\n";
        }
    }
}