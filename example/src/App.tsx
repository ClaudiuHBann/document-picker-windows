import { SafeAreaView, ScrollView, StyleSheet } from 'react-native';
import { ImportExamples } from './Import';
import { OpenExamples } from './Open';
import { SaveAsExamples } from './SaveAs';

export default function App() {
  return (
    <SafeAreaView style={{ flex: 1, backgroundColor: 'white' }}>
      <ScrollView
        style={styles.container}
        contentContainerStyle={{ gap: 20, padding: 10 }}
        testID={'screenContainer'}
      >
        <ImportExamples />
        <OpenExamples />
        <SaveAsExamples />
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
