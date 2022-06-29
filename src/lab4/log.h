#ifndef OSLABS_LOG_H
#define OSLABS_LOG_H
#include <iostream>

#define LOG_WARN_S(stream, message) \
  Logger(stream, __FILE__, __LINE__, __FUNCTION__).warn() << message

#define LOG_WARN(message) LOG_WARN_S(std::cerr, message)

namespace lab4 {
class Logger {
 public:
  Logger(std::ostream &out, const char *file, size_t line, const char *function)
      : _out(out), _file(file), _line(line), _function(function) {}

  ~Logger() noexcept { _out << '\n'; }

  Logger(const Logger &) = delete;
  Logger(Logger &&) = delete;
  Logger &operator=(const Logger &) = delete;
  Logger &operator=(Logger &&) = delete;

  std::ostream &warn() {
    return _out << (_file ? _file : "unknown") << ":" << _line << " at "
                << (_function ? _function : "unknown") << " [WARNING] ";
  }

 private:
  std::ostream &_out;
  const char *_file, *_function;
  size_t _line;
};
}  // namespace lab4

#endif  // OSLABS_LOG_H
