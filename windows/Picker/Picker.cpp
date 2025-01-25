#include "pch.h"

#include "Picker.h"

namespace
{
constexpr auto kAllowMultiSelection = "allowMultiSelection"sv; // boolean
constexpr auto kUri = "uri"sv;                                 // file path
constexpr auto kName = "name"sv;                               // file name with extension
constexpr auto kSize = "size"sv;                               // file size in bytes
constexpr auto kIsVirtual = "isVirtual"sv;                     // always false
constexpr auto kHasRequestedType = "hasRequestedType"sv;       // always false
constexpr auto kSourceUris = "sourceUris"sv;                   // always file paths
constexpr auto kFileName = "fileName"sv;                       // file name with extension
} // namespace

namespace winrt::Picker
{
using namespace React;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;

void Picker::Initialize(ReactContext const &aReactContext) noexcept
{
    mContext = aReactContext;
}

FileOpenPicker Picker::CreateFileOpenPicker(const JSValue &)
{
    FileOpenPicker picker;

    const auto hwnd = ReactCoreInjection::GetTopLevelWindowId(mContext.Properties().Handle());
    picker.as<IInitializeWithWindow>()->Initialize(reinterpret_cast<HWND>(hwnd));

    // TODO: use type(s) property from aOptions to filter the file types
    // for (const auto &filter : aOptions.filters)
    // {
    //     picker.FileTypeFilter().Append(to_hstring(filter));
    // }
    picker.FileTypeFilter().Append(L"*");

    picker.SuggestedStartLocation(PickerLocationId::Desktop);
    picker.ViewMode(PickerViewMode::List);

    return picker;
}

JSValueObject Picker::MakeDocumentPickerResponse(const StorageFile &aStorageFile)
{
    const std::filesystem::path path(to_string(aStorageFile.Path()));

    JSValueObject documentPickerResponse;

    documentPickerResponse[kUri] = path.string();
    documentPickerResponse[kName] = path.filename().string();
    documentPickerResponse[kSize] = std::filesystem::file_size(path);
    documentPickerResponse[kIsVirtual] = false;
    documentPickerResponse[kHasRequestedType] = false;

    return documentPickerResponse;
}

IAsyncAction Picker::pickInternal(JSValue &&aOptions, ReactPromise<JSValueArray> &&aResult) noexcept
{
    auto picker(CreateFileOpenPicker(aOptions));
    JSValueArray files;

    if (aOptions[kAllowMultiSelection].AsBoolean())
    {
        auto storageFiles(co_await picker.PickMultipleFilesAsync());
        for (const auto &storageFile : storageFiles)
        {
            files.push_back(MakeDocumentPickerResponse(storageFile));
        }
    }
    else
    {
        auto storageFile(co_await picker.PickSingleFileAsync());
        if (storageFile)
        {
            files.push_back(MakeDocumentPickerResponse(storageFile));
        }
    }

    aResult.Resolve(std::move(files));
}

void Picker::pick(JSValue &&aOptions, ReactPromise<JSValueArray> &&aResult) noexcept
{
    mContext.UIDispatcher().Post([this, aOptions = std::move(aOptions), aResult = std::move(aResult)]() mutable {
        pickInternal(std::move(aOptions), std::move(aResult))
            .Completed([this, aResult](const auto &aAction, const auto &aStatus) {
                AsyncActionCompletedHandler(aAction, aStatus, std::move(aResult));
            });
    });
}

FileSavePicker Picker::CreateFileSavePicker(const ::React::JSValue &aOptions)
{
    FileSavePicker picker;

    const auto hwnd = ReactCoreInjection::GetTopLevelWindowId(mContext.Properties().Handle());
    picker.as<IInitializeWithWindow>()->Initialize(reinterpret_cast<HWND>(hwnd));

    const auto fileName = aOptions[kFileName].TryGetString();
    if (fileName)
    {
        const std::filesystem::path path(*fileName);

        picker.SuggestedFileName(path.filename().stem().native());
        picker.DefaultFileExtension(path.filename().extension().native());
    }

    picker.SuggestedStartLocation(PickerLocationId::Desktop);

    return picker;
}

IAsyncAction Picker::saveDocumentInternal(JSValue &&aOptions, ReactPromise<JSValue> &&aResult) noexcept
{
    auto picker(CreateFileSavePicker(aOptions));
    auto storageFile(co_await picker.PickSaveFileAsync());

    const std::filesystem::path path(to_string(storageFile.Path()));

    JSValueObject file;

    file[kUri] = path.string();
    file[kName] = path.filename().string();

    aResult.Resolve(std::move(file));
}

void Picker::saveDocument(JSValue &&aOptions, ReactPromise<JSValue> &&aResult) noexcept
{
    mContext.UIDispatcher().Post([this, aOptions = std::move(aOptions), aResult = std::move(aResult)]() mutable {
        saveDocumentInternal(std::move(aOptions), std::move(aResult))
            .Completed([this, aResult](const auto &aAction, const auto &aStatus) {
                AsyncActionCompletedHandler(aAction, aStatus, std::move(aResult));
            });
    });
}

void Picker::writeDocuments(JSValue &&, ReactPromise<std::vector<JSValue>> &&aResult) noexcept
{
    aResult.Reject("Not implemented!");
}

FolderPicker Picker::CreateFolderPicker(const JSValue &)
{
    FolderPicker picker;

    const auto hwnd = ReactCoreInjection::GetTopLevelWindowId(mContext.Properties().Handle());
    picker.as<IInitializeWithWindow>()->Initialize(reinterpret_cast<HWND>(hwnd));

    // TODO: use type(s) property from aOptions to filter the file types
    // for (const auto &filter : aOptions.filters)
    // {
    //     picker.FileTypeFilter().Append(to_hstring(filter));
    // }
    picker.FileTypeFilter().Append(L"*");

    picker.SuggestedStartLocation(PickerLocationId::Downloads);
    picker.ViewMode(PickerViewMode::List);

    return picker;
}

IAsyncAction Picker::pickDirectoryInternal(JSValue &&aOptions, ReactPromise<JSValue> &&aResult) noexcept
{
    auto picker(CreateFolderPicker(aOptions));
    JSValueObject folders;

    auto storageFolder(co_await picker.PickSingleFolderAsync());
    if (storageFolder)
    {
        folders[kUri] = to_string(storageFolder.Path());
    }

    aResult.Resolve(std::move(folders));
}

void Picker::pickDirectory(JSValue &&aOptions, ReactPromise<JSValue> &&aResult) noexcept
{
    mContext.UIDispatcher().Post([this, aOptions = std::move(aOptions), aResult = std::move(aResult)]() mutable {
        pickDirectoryInternal(std::move(aOptions), std::move(aResult))
            .Completed([this, aResult](const auto &aAction, const auto &aStatus) {
                AsyncActionCompletedHandler(aAction, aStatus, std::move(aResult));
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

void Picker::releaseSecureAccess(std::vector<std::string> const &, ReactPromise<JSValue> &&aResult) noexcept
{
    aResult.Resolve(nullptr);
}

void Picker::releaseLongTermAccess(std::vector<std::string> const &, ReactPromise<JSValue> &&aResult) noexcept
{
    aResult.Resolve(nullptr);
}
} // namespace winrt::Picker
