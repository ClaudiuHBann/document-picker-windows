#include "pch.h"

#include "MimeTypesHelper.h"
#include "resource.h"

namespace
{
constexpr auto kUrlMimeTypes =
    "https://raw.githubusercontent.com/patrickmccallum/mimetype-io/refs/heads/master/src/mimeData.json"sv;

constexpr auto kEmptyMimeDataJson = L"[]"sv;

constexpr auto kMimeTypeName = L"name"sv;
constexpr auto kMimeTypeFileTypes = L"fileTypes"sv;
} // namespace

namespace winrt::Picker
{
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

MimeTypesHelper::MimeTypesHelper() : mDefaultFileTypes(single_threaded_vector<hstring>({L"*"}))
{
}

/* static */ hstring MimeTypesHelper::LoadMimeDataFallback()
{
    HMODULE module{};
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            reinterpret_cast<LPCWSTR>(&LoadMimeDataFallback), &module))
    {
        return kEmptyMimeDataJson.data();
    }

    const auto resource = FindResourceW(module, MAKEINTRESOURCEW(IDR_MIME_DATA_FALLBACK), RT_RCDATA);
    if (!resource)
    {
        return kEmptyMimeDataJson.data();
    }

    const auto dataHandle = LoadResource(module, resource);
    if (!dataHandle)
    {
        return kEmptyMimeDataJson.data();
    }

    const auto data = LockResource(dataHandle);
    if (!data)
    {
        return kEmptyMimeDataJson.data();
    }

    const auto size = SizeofResource(module, resource);
    if (!size)
    {
        return kEmptyMimeDataJson.data();
    }

    return winrt::to_hstring(std::string_view(static_cast<const char *>(data), size));
}

IAsyncAction MimeTypesHelper::Initialize()
{
    if (mMimeTypesToFileTypes.size() || mFileTypeToMimeType.size())
    {
        co_return;
    }

    const Uri uri(to_hstring(kUrlMimeTypes));
    hstring jsonString;
    try
    {
        jsonString = co_await mHttpClient.GetStringAsync(uri);
    }
    catch (...)
    {
        jsonString = LoadMimeDataFallback();
    }

    const auto mimeTypes = JsonArray::Parse(jsonString);
    for (const auto &iMimeType : mimeTypes)
    {
        const auto &mimeType = iMimeType.GetObject();
        const auto mimeTypeName = mimeType.GetNamedString(kMimeTypeName);

        const auto &iFileTypes = mimeType.GetNamedArray(kMimeTypeFileTypes);
        for (const auto &iFileType : iFileTypes.GetArray())
        {
            const auto fileType(iFileType.GetString());

            if (!mMimeTypesToFileTypes.contains(mimeTypeName))
            {
                mMimeTypesToFileTypes[mimeTypeName] = single_threaded_vector<hstring>();
            }
            mMimeTypesToFileTypes[mimeTypeName].Append(fileType);

            mFileTypeToMimeType[fileType] = mimeTypeName;
        }
    }
}

IAsyncOperation<IVector<hstring>> MimeTypesHelper::MimeTypeToFileTypes(const hstring &aMimeType)
{
    co_await Initialize();

    if (!mMimeTypesToFileTypes.contains(aMimeType))
    {
        co_return mDefaultFileTypes;
    }

    co_return mMimeTypesToFileTypes.at(aMimeType);
}

IAsyncOperation<hstring> MimeTypesHelper::FileTypeToMimeType(const hstring &aFile)
{
    co_await Initialize();

    const std::filesystem::path path(aFile.data());
    const hstring fileType(path.extension().native());

    if (!mFileTypeToMimeType.contains(fileType))
    {
        co_return mDefaultMimeType;
    }

    co_return mFileTypeToMimeType.at(fileType);
}
} // namespace winrt::Picker
