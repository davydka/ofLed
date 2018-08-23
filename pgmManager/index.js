const config = require('dotenv').config()
const midi = require('midi');

if (config.error) {
	throw config.error;
}

const {
	INDEX
} = process.env;
console.log('pifull #:', INDEX);


// Set up a new input.
const input = new midi.input();

let openIndex = 1;
for ( let i = 0; i < input.getPortCount(); i += 1 ) {
  if(input.getPortName(i) == 'QmidiNet 128:0') {
	  openIndex = i;
  }
  console.log(i, input.getPortName(i));
}
input.openPort(openIndex); //QmidiNet 128:0

// Configure a callback.
input.on('message', function(deltaTime, message) {
  // The message is an array of numbers corresponding to the MIDI bytes:
  //   [status, data1, data2]
  // https://www.cs.cf.ac.uk/Dave/Multimedia/node158.html has some helpful
  // information interpreting the messages.
  console.log('m:' + message + ' d:' + deltaTime);
});


// Sysex, timing, and active sensing messages are ignored
// by default. To enable these message types, pass false for
// the appropriate type in the function below.
// Order: (Sysex, Timing, Active Sensing)
// For example if you want to receive only MIDI Clock beats
// you should use
// input.ignoreTypes(true, false, true)
input.ignoreTypes(false, false, true);

// ... receive MIDI messages ...

// Close the port when done.
// input.closePort();
