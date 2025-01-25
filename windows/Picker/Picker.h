#pragma once

#if __has_include("codegen/NativeRNDocumentPickerWindowsDataTypes.g.h")
#include "codegen/NativeRNDocumentPickerWindowsDataTypes.g.h"
#endif
#include "codegen/NativeDocumentPickerWindowsSpec.g.h"

#include "NativeModules.h"

namespace winrt::Picker
{
REACT_MODULE(Picker, L"RNDocumentPickerWindows")
struct Picker
{
  public:
    using ModuleSpec = PickerCodegen::DocumentPickerWindowsSpec;

  public:
    REACT_INIT(Initialize)
    void Initialize(::React::ReactContext const &aReactContext) noexcept;

    REACT_METHOD(pick)
    void pick(::React::JSValue &&aOptions, ::React::ReactPromise<::React::JSValueArray> &&aResult) noexcept;

    REACT_METHOD(saveDocument)
    void saveDocument(::React::JSValue &&aOptions, ::React::ReactPromise<::React::JSValue> &&aResult) noexcept;

    REACT_METHOD(writeDocuments)
    void writeDocuments(::React::JSValue &&aOptions,
                        ::React::ReactPromise<std::vector<::React::JSValue>> &&aResult) noexcept;

    REACT_METHOD(pickDirectory)
    void pickDirectory(::React::JSValue &&aOptions, ::React::ReactPromise<::React::JSValue> &&aResult) noexcept;

    REACT_METHOD(keepLocalCopy)
    void keepLocalCopy(::React::JSValue &&aOptions, ::React::ReactPromise<::React::JSValueArray> &&aResult) noexcept;

    REACT_SYNC_METHOD(isKnownType)
    ::React::JSValue isKnownType(std::string aKind, std::string value) noexcept;

    REACT_METHOD(releaseSecureAccess)
    void releaseSecureAccess(std::vector<std::string> const &aURIs,
                             ::React::ReactPromise<::React::JSValue> &&aResult) noexcept;

    REACT_METHOD(releaseLongTermAccess)
    void releaseLongTermAccess(std::vector<std::string> const &aURIs,
                               ::React::ReactPromise<::React::JSValue> &&aResult) noexcept;

  private:
    React::ReactContext mContext;
};
} // namespace winrt::Picker
