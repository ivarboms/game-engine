#pragma once

#include <string>
#include <vector>
#include <functional>
#include <boost/noncopyable.hpp>
#include <Windows.h>


/*  This class monitors a directory for file changes such as file creation, deletion, modification
    and rename.
    Callbacks are fired when any of these changes occur. The callbacks include the name of the file
    that was changed, where the file name is relative to the watched directory.
*/
class FileSystemWatcher : boost::noncopyable
{
public:
  //Construct watcher and watch the specified directory.
	FileSystemWatcher(const std::wstring& directory);

	~FileSystemWatcher();

  /*  Asynchronously read changes in the watched directory.
      The changes will be sent to the application whenever the operation is completed and the calling
      thread is in an alertable wait state (i.e. SleepEx or WaitForSingleObject).
  */
  void readChangesAsync();

  /*  Block execution until a change in the directory occurs, or until the timeout has elapsed.
  */
  void waitForChanges(unsigned long timeout = INFINITE);


  //The directory being watched.
  const std::wstring directory;
  //The handle of the directory. It is possible to use functions like WaitForSingleObject on this handle.
  const HANDLE directoryHandle;

  //Function taking a wstring, which is the name of the file that was changed.
  typedef std::function<void(const std::wstring&)> FileChangedCallback;
  //Function taking two wstrings, the first one being the old file name, the second being the new file name.
  typedef std::function<void(const std::wstring&, const std::wstring&)> FileRenamedCallback;

  //Invoked when a file has been added.
  std::vector<FileChangedCallback> fileAddedCallbacks;
  //Invoked when a file has been removed.
  std::vector<FileChangedCallback> fileRemovedCallbacks;
  //Invoked when a file has been modified.
  std::vector<FileChangedCallback> fileModifiedCallbacks;
  //Invoked when a file has been renamed.
  std::vector<FileRenamedCallback> fileRenamedCallbacks;

private:
  //Called by Windows when ReadDirectoryChangesW finishes.
  static void WINAPI readDirectoryChangesCompleted(DWORD errorCode, DWORD numberOfBytesTransfered,
    OVERLAPPED* overlapped);

  OVERLAPPED overlapped;
  std::vector<DWORD> changesBuffer; //DWORDs are used to make sure alignment is correct.
};