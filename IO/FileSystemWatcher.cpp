#include "stdafx.h"
#include "FileSystemWatcher.h"
#include <WindowsUtility.h>

#include <Windows.h>
#include <assert.h>


FileSystemWatcher::FileSystemWatcher(const std::wstring& directory)
  : directory(directory)
  , directoryHandle(CreateFileW(directory.c_str(), FILE_LIST_DIRECTORY,
  FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING,
  FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, nullptr))
{
  changesBuffer.resize(16380);//Needs to be less than 16384 to make monitoring network drives work.
}

FileSystemWatcher::~FileSystemWatcher()
{
  //Cancel all I/O requests, from all threads.
  CancelIoEx(directoryHandle, nullptr);
  CloseHandle(directoryHandle);
}

void FileSystemWatcher::readChangesAsync()
{
  const DWORD notifyFilter = FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION
    | FILE_NOTIFY_CHANGE_FILE_NAME;

  DWORD bytes = 0;
  memset(&overlapped, 0, sizeof(overlapped));
  //hEvent is not used by ReadDirectoryChangesW, so it's safe to store custom data in it.
  overlapped.hEvent = this;

  bool success = ReadDirectoryChangesW(directoryHandle, changesBuffer.data(), changesBuffer.size(),
    true, notifyFilter, &bytes, &overlapped, readDirectoryChangesCompleted) != 0;
  assert(success);
}

void FileSystemWatcher::waitForChanges(unsigned long timeout /*= INFINITE*/)
{
  WaitForSingleObjectEx(directoryHandle, timeout, true);
}

template<typename T>
void invokeAllCallbacks(const std::vector<T>& callbacks, const std::wstring& fileName)
{
  for (unsigned int i = 0; i < callbacks.size(); ++i)
  {
    callbacks[i](fileName);
  }
}

template<typename T>
void invokeAllCallbacks(const std::vector<T>& callbacks, const std::wstring& oldFileName,
  const std::wstring& newFileName)
{
  for (unsigned int i = 0; i < callbacks.size(); ++i)
  {
    callbacks[i](oldFileName, newFileName);
  }
}

void WINAPI FileSystemWatcher::readDirectoryChangesCompleted(DWORD errorCode,
  DWORD numberOfBytesTransfered, OVERLAPPED* overlapped)
{
  if (errorCode != 0)
  {
    std::wstring error = winApiErrorCodeToString(GetLastError());
    wprintf(L"ReadDirectoryChangesW error %s\n", error.c_str());
    return;
  }
  if (numberOfBytesTransfered == 0)
  {
    printf("No data transfered\n");
    return;
  }

  FileSystemWatcher& watcher = *static_cast<FileSystemWatcher*>(overlapped->hEvent);

  //Extract all the file changes from the buffer.

  char* buffer = (char*)watcher.changesBuffer.data();
  
  //Events about renamed files are sent as 2 messages, one for old name and one for new time.
  //Because of this, the old name is cached to make it easier to invoke callbacks with both old and
  //new file names.
  //This assumes that the old file name event is sent before the new file event, which is not well
  //documented, but seems to be the case.
  std::wstring renamedOldName;


  unsigned int offset = 0;
  for (;;)
  {
    const FILE_NOTIFY_INFORMATION& notifyInfo = *(const FILE_NOTIFY_INFORMATION*)(buffer + offset);

    std::wstring fileName;
    fileName.assign(notifyInfo.FileName, notifyInfo.FileNameLength / sizeof(wchar_t));

    switch (notifyInfo.Action)
    {
    case FILE_ACTION_ADDED:
      invokeAllCallbacks(watcher.fileAddedCallbacks, fileName);
      break;

    case FILE_ACTION_REMOVED:
      invokeAllCallbacks(watcher.fileRemovedCallbacks, fileName);
      break;

    case FILE_ACTION_MODIFIED:
      invokeAllCallbacks(watcher.fileModifiedCallbacks, fileName);
      break;

    case FILE_ACTION_RENAMED_OLD_NAME:
      renamedOldName.assign(notifyInfo.FileName, notifyInfo.FileNameLength / sizeof(wchar_t));
      break;

    case FILE_ACTION_RENAMED_NEW_NAME:
      invokeAllCallbacks(watcher.fileRenamedCallbacks, renamedOldName, fileName);
      break;
    }

    offset += notifyInfo.NextEntryOffset;
    if (notifyInfo.NextEntryOffset == 0)
    {
      break;
    }
  }
}
