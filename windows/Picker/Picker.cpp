#include "pch.h"

#include "Picker.h"

namespace winrt::Picker
{
using namespace React;
using namespace Windows::Foundation;
using namespace Windows::Storage::Pickers;

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

FolderPicker Picker::CreateFolderPicker(const JSValue &aOptions)
{
    FolderPicker picker;

    const auto hwnd = ReactCoreInjection::GetTopLevelWindowId(mContext.Properties().Handle());
    picker.as<IInitializeWithWindow>()->Initialize(reinterpret_cast<HWND>(hwnd));

    // TODO: use type(s) property from aOptions to filter the file types
    // for (const auto &filter : aOptions)
    // {
    //     picker.FileTypeFilter().Append(to_hstring(filter));
    // }

    picker.SuggestedStartLocation(PickerLocationId::Downloads);
    picker.ViewMode(PickerViewMode::List);

    return picker;
}

IAsyncAction Picker::pickDirectoryInternal(JSValue &&aOptions, ReactPromise<JSValue> &&aResult) noexcept
{
    // keep promise alive
    auto capturedPromise = aResult;

    auto picker(CreateFolderPicker(aOptions));
    JSValueObject folders;

    auto storageFolder(co_await picker.PickSingleFolderAsync());
    if (storageFolder)
    {
        folders["uri"] = to_string(storageFolder.Path());
    }

    capturedPromise.Resolve(std::move(folders));
}

void Picker::pickDirectory(JSValue &&aOptions, ReactPromise<JSValue> &&aResult) noexcept
{
    mContext.UIDispatcher().Post([this, aOptions = std::move(aOptions), aResult = std::move(aResult)]() mutable {
        pickDirectoryInternal(std::move(aOptions), std::move(aResult))
            .Completed([aResult](const auto &aAction, const auto aStatus) {
                if (aStatus != AsyncStatus::Error)
                {
                    return;
                }

                const auto errorCode = aAction.ErrorCode().value;
                const auto errorMessage = std::system_category().message(errorCode);

                ReactError reactError{
                    .Message = std::format("HRESULT 0x{:}: {}", errorCode, errorMessage),
                };

                aPromise.Reject(std::move(reactError));
            });
    });
}

void Picker::keepLocalCopy(JSValue &&, ReactPromise<JSValueArray> &&aResult) noexcept
{
    aResult.Resolve({});
}

JSValue Picker::isKnownType(std::string, std::string) noexcept
{
    // TODO: implement me
    return nullptr;
}

void Picker::releaseSecureAccess(std::vector<std::string> const &aURIs, ReactPromise<JSValue> &&aResult) noexcept
{
    aResult.Resolve(nullptr);
}

void Picker::releaseLongTermAccess(std::vector<std::string> const &, ReactPromise<JSValue> &&aResult) noexcept
{
    aResult.Resolve(nullptr);
}
} // namespace winrt::Picker
