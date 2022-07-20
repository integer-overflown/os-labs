#ifndef OSLABS_OUTPUT_H
#define OSLABS_OUTPUT_H
#include <iostream>

namespace lab7 {
#ifdef UNICODE
inline std::wostream &out = std::wcout;
inline std::wostream &err = std::wcerr;
#else
inline std::ostream &out = std::cout;
inline std::ostream &err = std::cerr;
#endif
}

#endif  // OSLABS_OUTPUT_H
