// Autogenerated from KST: please remove this line if doing any edits by hand!

var assert = require('assert');
var testHelper = require('testHelper');

testHelper('ExprToITrailing', 'src/term_strz.bin', function(r, ExprToITrailing) {

  assert.throws(
    function() {
      r.toIR10;
    },
    {name: 'KaitaiStream.ConversionError'}
  )
  assert.strictEqual(r.toIR13, 44020937);
  assert.throws(
    function() {
      r.toIGarbage;
    },
    {name: 'KaitaiStream.ConversionError'}
  )
});
