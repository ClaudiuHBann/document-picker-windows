import { ScrollView, StyleSheet, View } from 'react-native';
import { ImportExamples } from './Import';
import { OpenExamples } from './Open';
import { SaveAsExamples } from './SaveAs';

export default function App() {
  return (
    <View style={styles.screen}>
      <ScrollView
        style={styles.container}
        contentContainerStyle={styles.content}
        testID={'screenContainer'}
      >
        <ImportExamples />
        <OpenExamples />
        <SaveAsExamples />
      </ScrollView>
    </View>
  );
}

const styles = StyleSheet.create({
  screen: {
    backgroundColor: '#202020',
    flex: 1,
  },
  container: {
    flex: 1,
  },
  content: {
    gap: 20,
    padding: 10,
  },
});
