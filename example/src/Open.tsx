import * as React from 'react';

import {
  type DirectoryPickerResponse,
  type DirectoryPickerResponseLongTerm,
  type DocumentPickerResponse,
  errorCodes,
  isErrorWithCode,
  pick,
  pickDirectory,
  releaseLongTermAccess,
  releaseSecureAccess,
  types,
} from '@react-native-documents-windows/picker';
import { useEffect } from 'react';
import {
  Button,
  Platform,
  SafeAreaView,
  ScrollView,
  StyleSheet,
  Text,
  View,
} from 'react-native';
import { Box } from './components/Box';

export function OpenExamples() {
  const [results, _setResults] = React.useState<
    Array<
      | DocumentPickerResponse[]
      | DirectoryPickerResponse
      | DirectoryPickerResponseLongTerm[]
    >
  >([]);
  const [bookmark, setBookmark] = React.useState<
    { fileName: string; bookmark: string } | undefined
  >();

  useEffect(() => {
    console.log('AsyncStorage');
  }, []);

  const addResult = (
    newResult:
      | DocumentPickerResponse[]
      | DirectoryPickerResponse
      | DirectoryPickerResponseLongTerm[]
  ) => {
    _setResults((prevResult) => {
      if (prevResult) {
        return [newResult, ...prevResult].slice(0, 4);
      } else {
        return [newResult];
      }
    });
    return newResult;
  };

  useEffect(() => {
    console.log(JSON.stringify(results, null, 2));
  }, [results]);

  const handleError = (err: unknown) => {
    if (isErrorWithCode(err)) {
      switch (err.code) {
        case errorCodes.IN_PROGRESS:
          console.warn(
            'user attempted to present a picker, but a previous one was already presented'
          );
          break;
        case errorCodes.UNABLE_TO_OPEN_FILE_TYPE:
          console.warn('unable to open file type');
          break;
        case errorCodes.OPERATION_CANCELED:
          console.log('cancelled');
          break;
        default:
          console.error(err);
      }
    } else {
      console.warn(err);
    }
  };

  return (
    <SafeAreaView style={{ flex: 1, backgroundColor: 'white' }}>
      <ScrollView
        style={styles.container}
        contentContainerStyle={{ gap: 20, padding: 10 }}
      >
        <Text>
          Stored bookmark from open mode:{' '}
          {bookmark ? bookmark.fileName : 'none'}
        </Text>

        <Box label={'Open mode'}>
          <Button
            title="open file"
            onPress={async () => {
              try {
                const [result] = await pick({
                  mode: 'open',
                });
                addResult([result]);
              } catch (err) {
                handleError(err);
              }
            }}
          />
          <Button
            title="open directory"
            onPress={async () => {
              try {
                const { uri } = await pickDirectory({
                  requestLongTermAccess: false,
                });
                addResult({ uri });
              } catch (err) {
                handleError(err);
              }
            }}
          />

          <Button
            title="open pdf file with requestLongTermAccess: true"
            onPress={async () => {
              try {
                const [result] = await pick({
                  mode: 'open',
                  requestLongTermAccess: true,
                  type: [types.pdf],
                });
                if (result.bookmarkStatus === 'success') {
                  const bookmarkToStore = {
                    fileName: result.name ?? 'unknown name',
                    bookmark: result.bookmark,
                  };
                  setBookmark(bookmarkToStore);
                  console.log('AsyncStorage');

                  const responseWithoutBookmark = {
                    ...result,
                    bookmark: 'bookmark is present',
                  };
                  addResult([responseWithoutBookmark]);
                } else {
                  console.warn(result);
                }
              } catch (err) {
                handleError(err);
              }
            }}
          />

          <Button
            title="open directory picker with requestLongTermAccess: true"
            onPress={async () => {
              try {
                const result = await pickDirectory({
                  requestLongTermAccess: true,
                });
                addResult(result);
                if (result.bookmarkStatus === 'success') {
                  const bookmarkToStore = {
                    fileName: 'directory',
                    bookmark: result.bookmark,
                  };
                  setBookmark(bookmarkToStore);
                  console.log('AsyncStorage');
                }
              } catch (err) {
                handleError(err);
              }
            }}
          />
        </Box>
        <Button
          title="view file that was previously opened (also works after app restart)"
          onPress={() => {
            const lastResults = results[0];
            if (
              lastResults &&
              Array.isArray(lastResults) &&
              lastResults.length > 0 &&
              lastResults[0]
            ) {
              const uriToOpen: string = lastResults[0].uri;
              console.log('viewDocument(' + uriToOpen + ')');
            } else if (bookmark) {
              console.log('viewDocument(' + bookmark + ')');
            } else {
              console.warn('no uri found', lastResults);
            }
          }}
        />
        <Button
          title="release LongTermAccess (android) / stopAccessingSecurityScopedResource (iOS)"
          onPress={() => {
            const lastResults = results[0];
            if (
              lastResults &&
              Array.isArray(lastResults) &&
              lastResults.length > 0 &&
              lastResults[0]
            ) {
              const uriToRelease: string = lastResults[0].uri;

              if (Platform.OS === 'android') {
                releaseLongTermAccess([uriToRelease])
                  .then(console.log)
                  .catch(handleError);
              } else {
                releaseSecureAccess([uriToRelease])
                  .then(console.log)
                  .catch(handleError);
              }
            } else {
              console.warn('no suitable uri found', lastResults);
            }
          }}
        />

        <Text selectable>Results</Text>
        <View>
          {results.flat().map((result, index) => {
            return (
              <View key={index}>
                <Text>{JSON.stringify(result)}</Text>
              </View>
            );
          })}
        </View>
      </ScrollView>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
