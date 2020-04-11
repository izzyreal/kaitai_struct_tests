var assert = require('assert');
var testHelper = require('testHelper');

testHelper('ImportsAbsRel', 'src/fixed_struct.bin', function(r, ImportsAbsRel) {

  assert.strictEqual(r.one, 80);
  assert.strictEqual(r.two.one, 65);
  assert.strictEqual(r.two.two.one, 67);
});
