#pragma once

#include "pch.h"
#include "resource.h"

#if __has_include("codegen/NativePickerDataTypes.g.h")
  #include "codegen/NativePickerDataTypes.g.h"
#endif
#include "codegen/NativePickerSpec.g.h"

#include "NativeModules.h"

namespace winrt::Picker
{

REACT_MODULE(Picker)
struct Picker
{
  using ModuleSpec = PickerCodegen::PickerSpec;

  REACT_INIT(Initialize)
  void Initialize(React::ReactContext const &reactContext) noexcept;

  REACT_SYNC_METHOD(multiply)
  double multiply(double a, double b) noexcept;

private:
  React::ReactContext m_context;
};

} // namespace winrt::Picker