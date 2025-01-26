const path = require('path');
const pkg = require('../package.json');

module.exports = {
  project: {},
  dependencies: {
    [pkg.name]: {
      root: path.join(__dirname, '..'),
      platforms: {},
    },
  },
};
