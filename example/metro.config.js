const { getDefaultConfig } = require('@react-native/metro-config');
const { withMetroConfig } = require('react-native-monorepo-config');
const fs = require('fs');
const path = require('node:path');

const root = path.resolve(__dirname, '..');
const rnwPath = fs.realpathSync(
  path.resolve(require.resolve('react-native-windows/package.json'), '..')
);

/**
 * Metro configuration
 * https://facebook.github.io/metro/docs/configuration
 *
 * @type {import('metro-config').MetroConfig}
 */
const config = withMetroConfig(getDefaultConfig(__dirname), {
  root,
  dirname: __dirname,
});

module.exports = {
  ...config,
  resolver: {
    ...config.resolver,
    blockList: [
      ...[config.resolver.blockList].flat().filter(Boolean),
      // This stops "npx @react-native-community/cli run-windows" from causing the metro server to crash if it's already running.
      new RegExp(
        `${path.resolve(__dirname, 'windows').replace(/[/\\]/g, '/')}.*`
      ),
      // This prevents MSBuild output from causing EBUSY errors while Metro is running.
      new RegExp(`${rnwPath}/build/.*`),
      new RegExp(`${rnwPath}/target/.*`),
      /.*\.ProjectImports\.zip/,
    ],
  },
};
