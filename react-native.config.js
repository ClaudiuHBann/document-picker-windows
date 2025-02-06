/**
 * @type {import('@react-native-community/cli-types').UserDependencyConfig}
 */
module.exports = {
  dependency: {
    platforms: {
      windows: {
        sourceDir: 'windows',
        solutionFile: 'Picker.sln',
        projects: [
          {
            projectFile: 'Picker\\Picker.vcxproj',
            directDependency: true,
          },
        ],
      },
    },
  },
};
