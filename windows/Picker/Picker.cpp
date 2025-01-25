#include "pch.h"

#include "Picker.h"

namespace winrt::Picker
{
void Picker::Initialize(React::ReactContext const &reactContext) noexcept
{
    mContext = reactContext;
}

double Picker::multiply(double a, double b) noexcept
{
    return a * b;
}
} // namespace winrt::Picker
