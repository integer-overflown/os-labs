#ifndef OSLABS_LOG_H
#define OSLABS_LOG_H
#define LOG_WARN_S(stream, message)                                 \
  (stream) << __FILE__ << ":" << __LINE__ << " at " << __FUNCTION__ \
           << " [WARNING] " << (message) << '\n'

#define LOG_WARN(message) LOG_WARN_S(std::cerr, message)

#endif  // OSLABS_LOG_H
