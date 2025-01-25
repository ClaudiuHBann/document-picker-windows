#include "pch.h"

#include "ReactPackageProvider.h"
#if __has_include("ReactPackageProvider.g.cpp")
#include "ReactPackageProvider.g.cpp"
#endif

#include "Picker.h"

namespace winrt::Picker::implementation
{
using namespace Microsoft::ReactNative;

void ReactPackageProvider::CreatePackage(IReactPackageBuilder const &aPackageBuilder) noexcept
{
    AddAttributedModules(aPackageBuilder, true);
}
} // namespace winrt::Picker::implementation
