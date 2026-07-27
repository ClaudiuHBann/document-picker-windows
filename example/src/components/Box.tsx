import * as React from 'react';
import { StyleSheet, Text, View } from 'react-native';

export const Box = ({
  children,
  label,
}: React.PropsWithChildren & { label: string }) => {
  return (
    <View style={styles.container}>
      <Text style={styles.label}>{label}</Text>
      {children}
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    borderColor: '#9A9A9A',
    borderRadius: 10,
    borderWidth: 2,
    padding: 10,
    rowGap: 10,
  },
  label: {
    fontWeight: 'bold',
  },
});
