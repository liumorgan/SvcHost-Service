# SvcHost-Service
An windows service that runs in svchost.exe.

# Service Project
`Service` project is a SvcHost-based service framework, you only need add your custom code to `SvcHostService::Run()` function:
```c++
void SvcHostService::Run() {
  OutputDebugStringW(L"[svcs] Run\n");

  while (true) {
    if (IsStopped())
      break;

    // Add your custom code to here!
    //
    // ......
  }

  if (hHasStoppedEvent_) {
    SetEvent(hHasStoppedEvent_);
  }
}
```

# Install Project
`Install` project is a sample about how to install/uninstall/start/stop SvcHost-based service.

The core function provide by `ServiceInstaller` class.

![ProcessExplorer.png](Screenshot/ProcessExplorer.png)
![Service.png](Screenshot/Service.png)