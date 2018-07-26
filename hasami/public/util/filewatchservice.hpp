#pragma once

#include <deque>
#include <map>
#include <tuple>
#include <string>
#include <thread>
#include <mutex>
#include <optional.hpp>
#include <FileWatcher/FileWatcher.h>

namespace hs
{

/// Watches a directory for file changes, e.g. FileWatchService::Instance().addWatch("C:\\some\\dir", this);
class FileWatchService
{
public:
  FileWatchService();
  ~FileWatchService();

  /// Listen thread messages
  enum class Task { AddWatch, RemoveWatch };

  /// This is a singleton instead of static so that its constructor runs on exit
  static FileWatchService& Instance() { static FileWatchService service; return service; }

  /// Add a watch for a filename
  void addWatch(const char* dirpath, FW::FileWatchListener* listener);

  /// Remove a watch
  void removeWatch(FW::FileWatchListener* listener);


private:
  /// Start the File watch thread
  void startFileWatchThread();

  /// Stop the File watch thread and join it
  void stopFileWatchThread();

  /// The File watch thread kernel
  void fileWatchThread();

  /// Tasks
  std::deque<std::tuple<Task, FW::FileWatchListener*, std::optional<std::string>>> m_tasks;

  /// Whether the File watch thread should be running
  bool m_runThread;

  /// The file watch thread
  std::thread m_fileWatchThread;

  /// File watch mutex
  std::mutex m_fileWatchMutex;

  /// File watch system
  FW::FileWatcher m_fileWatcher;

  /// Watcher ids
  std::map<FW::FileWatchListener*, FW::WatchID> m_watches;
};

}