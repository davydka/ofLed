const config = require('dotenv').config()
var easymidi = require('easymidi');

if (config.error) {
	throw config.error;
}

const {
	INDEX
} = process.env;
console.log('pifull #:', INDEX);


easymidi.getInputs().forEach(function(inputName){
  var input = new easymidi.Input(inputName);
  /*input.on('noteon', function (msg) {
    console.log(inputName+": "+msg);
  });
  input.on('cc', function (msg) {
    console.log(inputName+": "+msg);
  });*/
  input.on('program', function (msg) {
    console.log(inputName+": "+msg);
  });
});
