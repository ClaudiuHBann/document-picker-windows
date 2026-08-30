import NativeDocumentPickerWindows from './spec/NativeDocumentPickerWindows';

/**
 * Converts a canonical UTF-8 `file://` URI returned by this library to a Windows file-system path.
 *
 * Returns `null` for malformed URIs, unsupported URI forms, and URIs that cannot be converted to a Windows path.
 *
 * @group DocumentPicker
 */
export function uriToPath(uri: string): string | null {
  return NativeDocumentPickerWindows.uriToPath(uri);
}
