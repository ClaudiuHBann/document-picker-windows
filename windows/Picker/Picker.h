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
    void keepLocalCopy(::React::JSValue &&, ::React::ReactPromise<::React::JSValueArray> &&aResult) noexcept;

    REACT_SYNC_METHOD(isKnownType)
    ::React::JSValue isKnownType(std::string aKind, std::string value) noexcept;

    REACT_METHOD(releaseSecureAccess)
    void releaseSecureAccess(std::vector<std::string> const &,
                             ::React::ReactPromise<::React::JSValue> &&aResult) noexcept;

    REACT_METHOD(releaseLongTermAccess)
    void releaseLongTermAccess(std::vector<std::string> const &,
                               ::React::ReactPromise<::React::JSValue> &&aResult) noexcept;

  private:
    Windows::Storage::Pickers::FileOpenPicker CreateFileOpenPicker(const ::React::JSValue &aOptions);
    Windows::Storage::Pickers::FileSavePicker CreateFileSavePicker(const ::React::JSValue &aOptions);
    Windows::Storage::Pickers::FolderPicker CreateFolderPicker(const ::React::JSValue &aOptions);

    Windows::Foundation::IAsyncAction pickInternal(::React::JSValue &&aOptions,
                                                   ::React::ReactPromise<::React::JSValueArray> &&aResult) noexcept;
    Windows::Foundation::IAsyncAction saveDocumentInternal(::React::JSValue &&aOptions,
                                                           ::React::ReactPromise<::React::JSValue> &&aResult) noexcept;
    Windows::Foundation::IAsyncAction pickDirectoryInternal(::React::JSValue &&aOptions,
                                                            ::React::ReactPromise<::React::JSValue> &&aResult) noexcept;

    template <typename Type>
    void AsyncActionCompletedHandler(const Windows::Foundation::IAsyncAction &aAction,
                                     const Windows::Foundation::AsyncStatus &aStatus,
                                     ::React::ReactPromise<Type> &&aPromise) noexcept
    {
        if (aStatus != Windows::Foundation::AsyncStatus::Error)
        {
            return;
        }

        const auto errorCode = aAction.ErrorCode().value;
        const auto errorMessage = std::system_category().message(errorCode);

        Microsoft::ReactNative::ReactError reactError{
            .Message = std::format("HRESULT 0x{:}: {}", errorCode, errorMessage),
        };

        aPromise.Reject(std::move(reactError));
    }

    ::React::JSValueObject MakeDocumentPickerResponse(const Windows::Storage::StorageFile& aStorageFile);

  private:
    React::ReactContext mContext;
};
} // namespace winrt::Picker
