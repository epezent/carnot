#include <Utility/Util.hpp>
#ifdef _WIN32
#include <windows.h>
#endif

namespace carnot {

void openUrl(const std::string& url) {
#ifdef _WIN32
    ShellExecuteA(0, 0, url.c_str(), 0, 0 , 5);
#endif
}

void openEmail(const std::string& address, const std::string& subject) {
    std::string str = "mailto:" + address;
    if (!subject.empty()) 
        str += "?subject=" + subject;
#ifdef _WIN32
    ShellExecuteA(0, 0, str.c_str(), 0, 0 , 5);
#endif
}


} // namespace carnot