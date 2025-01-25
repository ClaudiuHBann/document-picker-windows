export {
  isKnownType,
  type IsKnownTypeOptions,
  type IsKnownTypeResponse,
} from './isKnownType';
export {
  keepLocalCopy,
  type FileToCopy,
  type KeepLocalCopyOptions,
  type KeepLocalCopyResponse,
} from './keepLocalCopy';

// TODO expose from /fileTypes?
export { errorCodes, isErrorWithCode } from './errors';
export { types, type PredefinedFileTypes } from './fileTypes';
export { pick, type DocumentPickerOptionsBase } from './pick';
export type {
  DocumentPickerOptions,
  DocumentPickerOptionsImport,
  DocumentPickerOptionsOpenLongTerm,
  DocumentPickerOptionsOpenOnce,
  DocumentPickerResponseOpenLongTerm,
} from './pick';
export { pickDirectory } from './pickDirectory';
export type {
  DirectoryPickerOptions,
  DirectoryPickerOptionsBase,
  DirectoryPickerOptionsLongTerm,
  DirectoryPickerResponse,
  DirectoryPickerResponseLongTerm,
  PickDirectoryResponse,
} from './pickDirectory';
export {
  releaseLongTermAccess,
  releaseSecureAccess,
  type ReleaseLongTermAccessResult,
} from './release';
export {
  saveDocuments,
  type SaveDocumentsOptions,
  type SaveDocumentsResponse,
} from './saveDocuments';
export type { LocalCopyResponse } from './spec/NativeDocumentPickerWindows';
export type {
  BookmarkingResponse,
  DocumentPickerResponse,
  NonEmptyArray,
  PresentationStyle,
  TransitionStyle,
  VirtualFileMeta,
} from './types';
