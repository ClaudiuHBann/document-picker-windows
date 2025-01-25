#pragma once

#include "ReactPackageProvider.g.h"

namespace winrt::Picker::implementation
{
struct ReactPackageProvider : ReactPackageProviderT<ReactPackageProvider>
{
    void CreatePackage(Microsoft::ReactNative::IReactPackageBuilder const &aPackageBuilder) noexcept;
};
} // namespace winrt::Picker::implementation

namespace winrt::Picker::factory_implementation
{
struct ReactPackageProvider : ReactPackageProviderT<ReactPackageProvider, implementation::ReactPackageProvider>
{
};
} // namespace winrt::Picker::factory_implementation
