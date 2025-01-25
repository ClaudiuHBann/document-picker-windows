#include "pch.h"

#include "Picker.h"

namespace winrt::Picker
{

// See https://microsoft.github.io/react-native-windows/docs/native-modules for details on writing native modules

void Picker::Initialize(React::ReactContext const &reactContext) noexcept {
  m_context = reactContext;
}

double Picker::multiply(double a, double b) noexcept {
  return a * b;
}

} // namespace winrt::Picker