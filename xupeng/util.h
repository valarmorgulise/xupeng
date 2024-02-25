#ifndef __PENG_UTIL_H__
#define __PENG_UTIL_H__

#include <stdint.h>
#include <string>
#include <sys/time.h>
#include <vector>

namespace PENG {

pid_t GetThreadId();

uint32_t GetFiberId();

/**
 * @brief 获取当前的调用栈
 * @param[in] bt 保存调用栈
 * @param[in] size 最多返回层数
 * @param[in] skip 跳过栈顶的层数
 *
 */
void Backtrace(std::vector<std::string> &bt, int size = 64, int skip = 1);

/**
 * @brief 获取当前栈信息的字符串
 * @param[in] size 栈的最大层数
 * @param[in] skip 跳过栈顶的层数
 * @param[in] prefix 栈信息前输出的内容
 *
 */
std::string BacktraceToString(int size = 64, int skip = 2,
                              const std::string &prefix = "");

uint64_t GetCurrentMS();
uint64_t GetCurrentUS();

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
