#include "pch.h"

#include "MimeTypesHelper.h"

namespace
{
constexpr auto kUrlMimeTypes =
    "https://raw.githubusercontent.com/patrickmccallum/mimetype-io/refs/heads/master/src/mimeData.json"sv;

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

IAsyncAction MimeTypesHelper::Initialize()
{
    if (mMimeTypesToFileTypes.size() || mFileTypeToMimeType.size())
    {
        co_return;
    }

    const Uri uri(to_hstring(kUrlMimeTypes));
    const auto jsonString(co_await mHttpClient.GetStringAsync(uri));

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
