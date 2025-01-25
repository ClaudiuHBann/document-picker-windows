#pragma once

#if __has_include("codegen/NativeRNDocumentPickerWindowsDataTypes.g.h")
#include "codegen/NativeRNDocumentPickerWindowsDataTypes.g.h"
#endif
#include "codegen/NativeRNDocumentPickerWindowsSpec.g.h"

#include "NativeModules.h"

namespace winrt::Picker
{

REACT_MODULE(Picker, L"RNDocumentPickerWindows")
struct Picker
{
    using ModuleSpec = PickerCodegen::RNDocumentPickerWindowsSpec;

    REACT_INIT(Initialize)
    void Initialize(React::ReactContext const &aReactContext) noexcept;

    REACT_SYNC_METHOD(multiply)
    double multiply(double a, double b) noexcept;

  private:
    React::ReactContext mContext;
};

} // namespace winrt::Picker
