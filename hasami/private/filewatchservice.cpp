#include "filewatchservice.hpp"

namespace hs
{

FileWatchService::FileWatchService()
  : m_runThread(false)
{
}

FileWatchService::~FileWatchService()
{
  stopFileWatchThread();
}

void FileWatchService::addWatch(const char* dir, FW::FileWatchListener* listener)
{
  if (!m_fileWatchThread.joinable())
    startFileWatchThread();
  std::lock_guard<std::mutex> lock(m_fileWatchMutex);
  m_tasks.push_back({Task::AddWatch, listener, std::optional<std::string>(dir)});
}

void FileWatchService::removeWatch(FW::FileWatchListener* listener)
{
  std::lock_guard<std::mutex> lock(m_fileWatchMutex);
  m_tasks.push_back({Task::RemoveWatch, listener, std::optional<std::string>()});
}

void FileWatchService::startFileWatchThread()
{
  std::lock_guard<std::mutex> lock(m_fileWatchMutex);
  if (m_runThread)
    return;

  m_runThread = true;
  m_fileWatchThread = std::thread(&FileWatchService::fileWatchThread, this);
}

void FileWatchService::stopFileWatchThread()
{
  if (m_fileWatchThread.joinable()) {
    m_fileWatchMutex.lock();
    m_runThread = false;
    m_fileWatchMutex.unlock();
    m_fileWatchThread.join();
  }
}

void FileWatchService::fileWatchThread()
{
  while (true) {
    {
      std::lock_guard<std::mutex> lock(m_fileWatchMutex);
      if (!m_runThread)
        break;
      for (auto& task : m_tasks) {
        switch (std::get<0>(task)) {
          case Task::AddWatch: {
            m_watches[std::get<1>(task)] = m_fileWatcher.addWatch(std::get<2>(task).value(), std::get<1>(task));
            break;
          }
          case Task::RemoveWatch: {
            auto it = m_watches.find(std::get<1>(task));
            if (it != m_watches.end()) {
              m_fileWatcher.removeWatch(it->second);
              m_watches.erase(it);
            }
            break;
          }
          default: assert(false); break;
        }
      }
      m_tasks.clear();
    }
    m_fileWatcher.update();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

}