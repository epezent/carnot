#include <Common/Util.hpp>
#include <windows.h>

namespace carnot {

void openUrl(const std::string& url) {
    ShellExecuteA(0, 0, url.c_str(), 0, 0 , 5);
}

void openEmail(const std::string& address, const std::string& subject) {
    std::string str = "mailto:" + address;
    if (!subject.empty()) 
        str += "?subject=" + subject;
    ShellExecuteA(0, 0, str.c_str(), 0, 0 , 5);
}


} // namespace carnot