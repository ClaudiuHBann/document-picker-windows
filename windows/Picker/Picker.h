#pragma once

#if __has_include("codegen/NativeRNDocumentPickerWindowsDataTypes.g.h")
#include "codegen/NativeRNDocumentPickerWindowsDataTypes.g.h"
#endif
#include "codegen/NativeDocumentPickerWindowsSpec.g.h"

#include "MimeTypesHelper.h"
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
    void keepLocalCopy(::React::JSValue &&, ::React::ReactPromise<std::vector<::React::JSValue>> &&aResult) noexcept;

    REACT_SYNC_METHOD(isKnownType)
    ::React::JSValue isKnownType(std::string aKind, std::string aValue) noexcept;

    REACT_METHOD(releaseSecureAccess)
    void releaseSecureAccess(std::vector<std::string> const &, ::React::ReactPromise<void> &&aResult) noexcept;

    REACT_METHOD(releaseLongTermAccess)
    void releaseLongTermAccess(std::vector<std::string> const &,
                               ::React::ReactPromise<::React::JSValue> &&aResult) noexcept;

  private:
    static std::optional<std::string> UriFromPath(const hstring &aPath);

  private:
    Windows::Foundation::IAsyncOperation<Windows::Storage::Pickers::FileOpenPicker> CreateFileOpenPicker(
        const std::shared_ptr<::React::JSValue> aOptions);
    Windows::Storage::Pickers::FileSavePicker CreateFileSavePicker(const ::React::JSValue &aOptions);
    Windows::Foundation::IAsyncOperation<Windows::Storage::Pickers::FolderPicker> CreateFolderPicker(
        const std::shared_ptr<::React::JSValue> aOptions);

    Windows::Foundation::IAsyncAction pickInternal(const std::shared_ptr<::React::JSValue> aOptions,
                                                   ::React::ReactPromise<::React::JSValueArray> aResult) noexcept;
    Windows::Foundation::IAsyncAction saveDocumentInternal(const std::shared_ptr<::React::JSValue> aOptions,
                                                           ::React::ReactPromise<::React::JSValue> aResult) noexcept;
    Windows::Foundation::IAsyncAction pickDirectoryInternal(const std::shared_ptr<::React::JSValue> aOptions,
                                                            ::React::ReactPromise<::React::JSValue> aResult) noexcept;

    template <typename Promise>
        requires std::derived_from<Promise, ::React::ReactPromiseBase>
    void AsyncActionCompletedHandler(const Windows::Foundation::IAsyncAction &aAction,
                                     const Windows::Foundation::AsyncStatus &aStatus, Promise aPromise) noexcept
    {
        if (aStatus == Windows::Foundation::AsyncStatus::Completed)
        {
            return;
        }

        if (aStatus == Windows::Foundation::AsyncStatus::Canceled)
        {
            aPromise.Reject(Microsoft::ReactNative::ReactError{
                .Code = "OPERATION_CANCELED",
                .Message = "The picker operation was canceled.",
            });
            return;
        }

        const auto errorCode = aAction.ErrorCode();
        const auto errorCodeValue = static_cast<unsigned int>(errorCode.value);
        Microsoft::ReactNative::ReactError reactError{
            .Code = std::format("0x{:08X}", errorCodeValue),
            .Message = std::format("HRESULT 0x{:08X}: {}", errorCodeValue,
                                   to_string(winrt::hresult_error(errorCode).message())),
        };

        aPromise.Reject(std::move(reactError));
    }

    Windows::Foundation::IAsyncAction PopulateDocumentPickerResponse(::React::JSValueObject &aResponse,
                                                                     const Windows::Storage::StorageFile &aStorageFile);

  private:
    React::ReactContext mContext;
    MimeTypesHelper mMimeTypesHelper;
};
} // namespace winrt::Picker
