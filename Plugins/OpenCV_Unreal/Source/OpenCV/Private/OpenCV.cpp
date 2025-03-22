// (c) 2019 Technical University of Munich
// Jakob Weiss <jakob.weiss@tum.de>

#include "OpenCV.h"
#include "Core.h"
#include "Interfaces/IPluginManager.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FOpenCVModule"

THIRD_PARTY_INCLUDES_START
#include <opencv2/opencv.hpp>
THIRD_PARTY_INCLUDES_END

DEFINE_LOG_CATEGORY(OpenCV);

void FOpenCVModule::StartupModule() {
  // Get the base directory of this plugin
  FString BaseDir = IPluginManager::Get().FindPlugin("OpenCV")->GetBaseDir();

  // Add on the relative location of the third party dll and load it
  FString LibraryPath;
#if PLATFORM_WINDOWS
  LibraryPath =
      FPaths::Combine(*BaseDir, TEXT("ThirdParty/opencv/x64/vc15/bin/opencv_world341.dll"));
#elif PLATFORM_MAC
  LibraryPath =
      FPaths::Combine(*BaseDir, TEXT("ThirdParty/opencv/Mac/bin/libopencv_world341.dylib"));
#else
  static_assert(false, "Other platforms are currently not supported!");
#endif

  OpenCVLibraryHandle =
      !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

  if (OpenCVLibraryHandle) {
    UE_LOG(OpenCV, Log, TEXT("Loaded OpenCV Version %d.%d.%d"), CV_MAJOR_VERSION, CV_MINOR_VERSION,
           CV_SUBMINOR_VERSION)
  } else {
    UE_LOG(OpenCV, Error,
           TEXT("Failed to load opencv library! Check your include/lib paths and make sure "
                "opencv_world341.dll is deployed!"))
  }
}

void FOpenCVModule::ShutdownModule() {
  // Free the dll handle
  FPlatformProcess::FreeDllHandle(OpenCVLibraryHandle);
  OpenCVLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FOpenCVModule, OpenCV)
