#ifndef __PENG_UTIL_H__
#define __PENG_UTIL_H__

#include <stdint.h>
#include <string>
#include <vector>

namespace PENG {

pid_t GetThreadId();

uint32_t GetFiberId();

class FSUtil {
public:
  static void ListAllFile(std::vector<std::string> &files,
                          const std::string &path, const std::string &subfix);
  static void Mkdir(const std::string &dirname);
  static void isRunningPidfile(const std::string &pidfile);
  static bool Rm(const std::string &path);
  static bool Mv(const std::string &from, const std::string &to);
  static bool RealPath(const std::string &path, const std::string &rpath);
  static bool Symlink(const std::string &frm, const std::string &to);
  static bool Unlink(const std::string &filename, bool exist = false);
  static std::string Dirname(const std::string &filename);
  static std::string Basename(const std::string &filename);
  // static bool OpenForRead(std::ifstream &ifs, const std::string &filename,
  // std::ios_base::openmode mode);
  // static bool OpenForWrite(std::ofstream &ofs, const
  // std::string &filename,
  // std::ios_base::openmode mode);
};
} // namespace PENG

#endif // !__PENG_UTIL_H__
