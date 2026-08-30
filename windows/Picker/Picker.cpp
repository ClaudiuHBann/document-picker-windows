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
constexpr auto kType = "type"sv;                               // array of strings of mime types
constexpr auto kNativeType = "nativeType"sv;                   // array of strings of types

constexpr auto kKindUTType = "UTType"sv; // apple specific
constexpr auto kKindMimeType = "mimeType"sv;
constexpr auto kKindExtension = "extension"sv;

constexpr auto kIsKnown = "isKnown"sv;                                       // boolean
constexpr auto kPreferredFilenameExtension = "preferredFilenameExtension"sv; // string
constexpr auto kMimeType = "mimeType"sv;                                     // string
constexpr auto kUTType = "UTType"sv;                                         // string

constexpr auto kAsyncStatusCanceledString = "OPERATION_CANCELED"sv;

constexpr auto kFileUriPrefixA = "file://"sv;        // UNC/network paths
constexpr auto kFileUriPrefixW = L"file://"sv;       // UNC/network paths
constexpr auto kLocalFileUriPrefixA = "file:///"sv;  // local drive paths
constexpr auto kLocalFileUriPrefixW = L"file:///"sv; // local drive paths
constexpr auto kUriPathSeparatorA = '/';
constexpr auto kUriPathSeparatorW = L'/';

constexpr auto kUncPathPrefix = LR"(\\)"sv;
constexpr auto kExtendedPathPrefix = LR"(\\?\)"sv;
constexpr auto kDevicePathPrefix = LR"(\\.\)"sv;
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

/* static */ std::string Picker::UriFromPath(const hstring &aPath)
{
    const std::filesystem::path path(aPath.c_str());
    const auto &pathNative = path.native();

    if (pathNative.empty() || !path.is_absolute() || pathNative.starts_with(kExtendedPathPrefix) ||
        pathNative.starts_with(kDevicePathPrefix))
    {
        throw hresult_invalid_argument(L"Relative, extended and device paths are not supported yet.");
    }

    const auto escapeComponent = [](const std::wstring &aComponent) {
        if (aComponent.empty() || aComponent == L"."sv || aComponent == L".."sv)
        {
            throw hresult_invalid_argument(L"Relative paths are not supported yet.");
        }

        std::array<wchar_t, INTERNET_MAX_URL_LENGTH> escaped{};
        DWORD escapedLength = static_cast<DWORD>(escaped.size());

        constexpr DWORD escapeFlags = URL_ESCAPE_URI_COMPONENT | URL_ESCAPE_SEGMENT_ONLY;
        check_hresult(UrlEscapeW(aComponent.c_str(), escaped.data(), &escapedLength, escapeFlags));

        return std::wstring(escaped.data(), escapedLength);
    };

    std::wstring uri;
    const auto rootName = path.root_name().native();

    if (pathNative.starts_with(kUncPathPrefix))
    {
        uri = kFileUriPrefixW;
        uri += escapeComponent(rootName.substr(kUncPathPrefix.size()));
        uri += kUriPathSeparatorW;
    }
    else
    {
        uri = kLocalFileUriPrefixW;
        uri += rootName; // drive, e.g.: "C:"
        uri += kUriPathSeparatorW;
    }

    bool firstComponent = true;
    for (const auto &component : path.relative_path())
    {
        if (!firstComponent)
        {
            uri += kUriPathSeparatorW;
        }

        uri += escapeComponent(component.native());
        firstComponent = false;
    }

    if (pathNative.starts_with(kUncPathPrefix) && firstComponent)
    {
        throw hresult_invalid_argument(L"The UNC path does not contain a share name.");
    }

    return to_string(uri);
}

/* static */ std::optional<std::string> Picker::PathFromUri(const std::string &aUri)
{
    if (aUri.empty())
    {
        return std::nullopt;
    }

    const auto unescapeComponent = [](const std::string &aComponent) -> std::optional<std::wstring> {
        if (aComponent.empty())
        {
            return std::nullopt;
        }

        std::wstring component(aComponent.begin(), aComponent.end());
        std::vector<wchar_t> unescaped(component.size() + 1);
        DWORD unescapedLength = static_cast<DWORD>(unescaped.size());

        if (FAILED(UrlUnescapeW(component.data(), unescaped.data(), &unescapedLength, URL_UNESCAPE_AS_UTF8)))
        {
            return std::nullopt;
        }

        std::wstring result(unescaped.data(), unescapedLength);
        if (result.empty() || result == L"."sv || result == L".."sv)
        {
            return std::nullopt;
        }

        return result;
    };

    const auto appendPathComponents = [&unescapeComponent](const std::string &aEncodedPath,
                                                           std::filesystem::path &aPath) {
        if (aEncodedPath.empty())
        {
            return true;
        }

        size_t componentStart = 0;
        while (componentStart <= aEncodedPath.size())
        {
            const auto separator = aEncodedPath.find(kUriPathSeparatorA, componentStart);
            const auto componentEnd = separator == std::string_view::npos ? aEncodedPath.size() : separator;
            const auto decoded = unescapeComponent(aEncodedPath.substr(componentStart, componentEnd - componentStart));
            if (!decoded)
            {
                return false;
            }

            aPath /= *decoded;

            if (separator == std::string_view::npos)
            {
                break;
            }

            componentStart = separator + 1;
        }

        return true;
    };

    std::filesystem::path path;

    if (aUri.starts_with(kLocalFileUriPrefixA))
    {
        const auto localPath = aUri.substr(kLocalFileUriPrefixA.size());
        const std::wstring root{static_cast<wchar_t>(localPath[0]), L':', L'\\'};

        path = std::filesystem::path(root);
        if (!appendPathComponents(localPath.substr(3), path))
        {
            return std::nullopt;
        }
    }
    else if (aUri.starts_with(kFileUriPrefixA))
    {
        const auto uncUri = aUri.substr(kFileUriPrefixA.size());
        const auto authorityEnd = uncUri.find(kUriPathSeparatorA);
        if (authorityEnd == std::string::npos)
        {
            return std::nullopt;
        }

        const auto host = unescapeComponent(uncUri.substr(0, authorityEnd));
        if (!host)
        {
            return std::nullopt;
        }

        const auto encodedPath = uncUri.substr(authorityEnd + 1);
        if (encodedPath.empty())
        {
            return std::nullopt;
        }

        path = std::filesystem::path(kUncPathPrefix.data() + *host);
        if (!appendPathComponents(encodedPath, path))
        {
            return std::nullopt;
        }
    }
    else
    {
        return std::nullopt;
    }

    if (!path.is_absolute() || UriFromPath(hstring{path.native()}) != aUri)
    {
        return std::nullopt;
    }

    return to_string(path.native());
}

IAsyncOperation<FileOpenPicker> Picker::CreateFileOpenPicker(const std::shared_ptr<JSValue> aOptions)
{
    FileOpenPicker picker;

    const auto hwnd = ReactCoreInjection::GetTopLevelWindowId(mContext.Properties().Handle());
    picker.as<IInitializeWithWindow>()->Initialize(reinterpret_cast<HWND>(hwnd));

    for (const auto &mimeType : (*aOptions)[kType].AsArray())
    {
        const auto mimeTypeStr(to_hstring(mimeType.AsString()));
        const auto fileTypes = co_await mMimeTypesHelper.MimeTypeToFileTypes(mimeTypeStr);

        for (const auto &fileType : fileTypes)
        {
            picker.FileTypeFilter().Append(fileType);
        }
    }

    picker.SuggestedStartLocation(PickerLocationId::Desktop);
    picker.ViewMode(PickerViewMode::List);

    co_return picker;
}

IAsyncAction Picker::PopulateDocumentPickerResponse(JSValueObject &aResponse, const StorageFile &aStorageFile)
{
    const auto path(aStorageFile.Path());
    const auto basicProperties(co_await aStorageFile.GetBasicPropertiesAsync());

    aResponse[kUri] = JSValue(UriFromPath(path));
    aResponse[kName] = to_string(aStorageFile.Name());
    aResponse[kSize] = basicProperties.Size();
    aResponse[kType] = to_string(co_await mMimeTypesHelper.FileTypeToMimeType(path));
    aResponse[kNativeType] = to_string(aStorageFile.FileType());
    aResponse[kIsVirtual] = false;
    aResponse[kHasRequestedType] = false;
}

IAsyncAction Picker::pickInternal(const std::shared_ptr<JSValue> aOptions, ReactPromise<JSValueArray> aResult) noexcept
{
    auto picker(co_await CreateFileOpenPicker(aOptions));
    JSValueArray files;

    if ((*aOptions)[kAllowMultiSelection].AsBoolean())
    {
        auto storageFiles(co_await picker.PickMultipleFilesAsync());
        for (const auto &storageFile : storageFiles)
        {
            JSValueObject response;
            co_await PopulateDocumentPickerResponse(response, storageFile);

            files.push_back(std::move(response));
        }
    }
    else
    {
        auto storageFile(co_await picker.PickSingleFileAsync());
        if (storageFile)
        {
            JSValueObject response;
            co_await PopulateDocumentPickerResponse(response, storageFile);

            files.push_back(std::move(response));
        }
    }

    if (files.empty())
    {
        aResult.Reject(ReactError{
            .Code = kAsyncStatusCanceledString.data(),
            .Message = "The document picker was canceled.",
        });
        co_return;
    }

    aResult.Resolve(std::move(files));
}

void Picker::pick(JSValue &&aOptions, ReactPromise<JSValueArray> &&aResult) noexcept
{
    mContext.UIDispatcher().Post([this, aOptions = std::move(aOptions), aResult]() mutable {
        pickInternal(std::make_shared<JSValue>(std::move(aOptions)), aResult)
            .Completed([this, aResult](const auto &aAction, const auto &aStatus) {
                AsyncActionCompletedHandler(aAction, aStatus, aResult);
            });
    });
}

FileSavePicker Picker::CreateFileSavePicker(const JSValue &aOptions)
{
    FileSavePicker picker;

    const auto hwnd = ReactCoreInjection::GetTopLevelWindowId(mContext.Properties().Handle());
    picker.as<IInitializeWithWindow>()->Initialize(reinterpret_cast<HWND>(hwnd));

    const auto fileName = aOptions[kFileName].TryGetString();
    if (fileName)
    {
        const std::filesystem::path path(*fileName);
        const hstring extension(path.filename().extension().native());

        picker.SuggestedFileName(path.filename().stem().native());
        if (!extension.empty())
        {
            picker.DefaultFileExtension(extension);
            picker.FileTypeChoices().Insert(L"File", single_threaded_vector<hstring>({extension}));
        }
    }

    picker.SuggestedStartLocation(PickerLocationId::Desktop);

    return picker;
}

IAsyncAction Picker::saveDocumentInternal(const std::shared_ptr<JSValue> aOptions,
                                          ReactPromise<JSValue> aResult) noexcept
{
    auto picker(CreateFileSavePicker(*aOptions));
    auto storageFile(co_await picker.PickSaveFileAsync());

    if (!storageFile)
    {
        aResult.Reject(ReactError{
            .Code = kAsyncStatusCanceledString.data(),
            .Message = "The save picker was canceled.",
        });
        co_return;
    }

    JSValueObject file;

    file[kUri] = JSValue(UriFromPath(storageFile.Path()));
    file[kName] = to_string(storageFile.Name());

    aResult.Resolve(std::move(file));
}

void Picker::saveDocument(JSValue &&aOptions, ReactPromise<JSValue> &&aResult) noexcept
{
    mContext.UIDispatcher().Post([this, aOptions = std::move(aOptions), aResult]() mutable {
        saveDocumentInternal(std::make_shared<JSValue>(std::move(aOptions)), aResult)
            .Completed([this, aResult](const auto &aAction, const auto &aStatus) {
                AsyncActionCompletedHandler(aAction, aStatus, aResult);
            });
    });
}

void Picker::writeDocuments(JSValue &&, ReactPromise<std::vector<JSValue>> &&aResult) noexcept
{
    aResult.Reject("Not implemented!");
}

IAsyncOperation<FolderPicker> Picker::CreateFolderPicker(const std::shared_ptr<JSValue> aOptions)
{
    FolderPicker picker;

    const auto hwnd = ReactCoreInjection::GetTopLevelWindowId(mContext.Properties().Handle());
    picker.as<IInitializeWithWindow>()->Initialize(reinterpret_cast<HWND>(hwnd));
    picker.FileTypeFilter().Append(L"*");
    picker.SuggestedStartLocation(PickerLocationId::Downloads);
    picker.ViewMode(PickerViewMode::List);

    co_return picker;
}

IAsyncAction Picker::pickDirectoryInternal(const std::shared_ptr<JSValue> aOptions,
                                           ReactPromise<JSValue> aResult) noexcept
{
    auto picker(co_await CreateFolderPicker(aOptions));
    JSValueObject folders;

    auto storageFolder(co_await picker.PickSingleFolderAsync());
    if (!storageFolder)
    {
        aResult.Reject(ReactError{
            .Code = kAsyncStatusCanceledString.data(),
            .Message = "The folder picker was canceled.",
        });
        co_return;
    }

    folders[kUri] = JSValue(UriFromPath(storageFolder.Path()));
    aResult.Resolve(std::move(folders));
}

void Picker::pickDirectory(JSValue &&aOptions, ReactPromise<JSValue> &&aResult) noexcept
{
    mContext.UIDispatcher().Post([this, aOptions = std::move(aOptions), aResult]() mutable {
        pickDirectoryInternal(std::make_shared<JSValue>(std::move(aOptions)), aResult)
            .Completed([this, aResult](const auto &aAction, const auto &aStatus) {
                AsyncActionCompletedHandler(aAction, aStatus, aResult);
            });
    });
}

void Picker::keepLocalCopy(JSValue &&, ReactPromise<JSValueArray> &&aResult) noexcept
{
    aResult.Resolve({});
}

JSValue Picker::isKnownType(std::string aKind, std::string aValue) noexcept
{
    JSValueObject response;
    response[kIsKnown] = false;
    response[kPreferredFilenameExtension] = nullptr;
    response[kMimeType] = nullptr;
    response[kUTType] = nullptr;

    try
    {
        if (aKind == kKindUTType)
        {
            return response;
        }

        if (aKind == kKindMimeType)
        {
            const auto fileTypes = mMimeTypesHelper.MimeTypeToFileTypes(to_hstring(aValue)).get();
            if (fileTypes.Size() == 1 && fileTypes.GetAt(0) == mMimeTypesHelper.GetDefaultFileType())
            {
                return response;
            }

            response[kIsKnown] = true;
            response[kMimeType] = aValue;
            response[kPreferredFilenameExtension] = to_string(fileTypes.GetAt(0)).substr(1);
        }
        else if (aKind == kKindExtension)
        {
            const auto mimeType = mMimeTypesHelper.FileTypeToMimeType(to_hstring(aValue), true).get();
            if (mimeType == mMimeTypesHelper.GetDefaultMimeType())
            {
                return response;
            }

            response[kIsKnown] = true;
            response[kPreferredFilenameExtension] = aValue;
            response[kMimeType] = to_string(mimeType);
        }
    }
    catch (...)
    {
    }

    return response;
}

std::optional<std::string> Picker::uriToPath(std::string aUri) noexcept
{
    try
    {
        return PathFromUri(aUri);
    }
    catch (...)
    {
        return std::nullopt;
    }
}

void Picker::releaseSecureAccess(std::vector<std::string> const &, ReactPromise<void> &&aResult) noexcept
{
    aResult.Resolve();
}

void Picker::releaseLongTermAccess(std::vector<std::string> const &, ReactPromise<JSValue> &&aResult) noexcept
{
    aResult.Resolve(nullptr);
}
} // namespace winrt::Picker

// TODO: make requests and responses structs instead on magic strings and objects
