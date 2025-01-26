#pragma once

namespace winrt::Picker
{
class MimeTypesHelper final
{
  public:
    explicit MimeTypesHelper();

    Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVector<hstring>> MimeTypeToFileTypes(
        const hstring &aMimeType);
    Windows::Foundation::IAsyncOperation<hstring> FileTypeToMimeType(const hstring &aFile);

  private:
    Windows::Foundation::IAsyncAction Initialize();

  private:
    Windows::Web::Http::HttpClient mHttpClient;

    std::map<hstring, Windows::Foundation::Collections::IVector<hstring>> mMimeTypesToFileTypes;
    std::map<hstring, hstring> mFileTypeToMimeType;

    const hstring mDefaultMimeType{L"*/*"};
    const Windows::Foundation::Collections::IVector<hstring> mDefaultFileTypes;
};
} // namespace winrt::Picker
