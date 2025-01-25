#include "pch.h"

#include "Picker.h"

namespace winrt::Picker
{
using namespace React;

void Picker::Initialize(ReactContext const &aReactContext) noexcept
{
    mContext = aReactContext;
}

void Picker::pick(JSValue &&aOptions, ReactPromise<JSValueArray> &&aResult) noexcept
{
    UNREFERENCED_PARAMETER(aOptions);
    UNREFERENCED_PARAMETER(aResult);
}

void Picker::saveDocument(JSValue &&aOptions, ReactPromise<JSValue> &&aResult) noexcept
{
    UNREFERENCED_PARAMETER(aOptions);
    UNREFERENCED_PARAMETER(aResult);
}

void Picker::writeDocuments(JSValue &&aOptions, ReactPromise<std::vector<JSValue>> &&aResult) noexcept
{
    UNREFERENCED_PARAMETER(aOptions);
    UNREFERENCED_PARAMETER(aResult);
}

void Picker::pickDirectory(JSValue &&aOptions, ReactPromise<JSValue> &&aResult) noexcept
{
    UNREFERENCED_PARAMETER(aOptions);
    UNREFERENCED_PARAMETER(aResult);
}

void Picker::keepLocalCopy(JSValue &&aOptions, ReactPromise<JSValueArray> &&aResult) noexcept
{
    UNREFERENCED_PARAMETER(aOptions);
    UNREFERENCED_PARAMETER(aResult);
}

JSValue Picker::isKnownType(std::string aKind, std::string value) noexcept
{
    UNREFERENCED_PARAMETER(aKind);
    UNREFERENCED_PARAMETER(value);
    return nullptr;
}

void Picker::releaseSecureAccess(std::vector<std::string> const &aURIs, ReactPromise<JSValue> &&aResult) noexcept
{
    UNREFERENCED_PARAMETER(aURIs);
    UNREFERENCED_PARAMETER(aResult);
}

void Picker::releaseLongTermAccess(std::vector<std::string> const &aURIs, ReactPromise<JSValue> &&aResult) noexcept
{
    UNREFERENCED_PARAMETER(aURIs);
    UNREFERENCED_PARAMETER(aResult);
}
} // namespace winrt::Picker
