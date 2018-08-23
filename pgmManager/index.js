const config = require('dotenv').config();
const easymidi = require('easymidi');
const pm2 = require('pm2');
const fs = require('fs');

if (config.error) {
	throw config.error;
}

const {
  INDEX
} = process.env;
// console.log('pifull #:', INDEX);

let currentPgm = fs.readFileSync('PGM').toString();


easymidi.getInputs().forEach(function(inputName){
  var input = new easymidi.Input(inputName);
  input.on('program', function (msg) {

    // 0 index midi channels, so channel 6 on device is channel 5 here
    if(msg.channel === 5) {
      handlePgm(msg.number);
    }
    else {
      console.log(msg);
    }

  });
});

function handlePgm(pgm) {
  if(pgm === currentPgm) {
    console.log('samesies');
    return;
  }
  console.log(currentPgm);

  let startProcess = pgm.toString().padStart(3, '0');
  let stopProcess = "ofLed";
  if(currentPgm.toString().indexOf("ofLed") == -1) {
    console.log("setting to soemthing");
    stopProcess = currentPgm.toString().padStart(3, '0');
  }

  currentPgm = pgm;
  fs.writeFileSync('PGM', pgm);

  pm2.stop(stopProcess, function(err) {
    if(err) {
      // console.log(err);
    }

    console.log('stopped');
  });

  pm2.start({
    name: startProcess,
    script: `../${startProcess}/bin/${startProcess}`,
    watch: false,
    interpreter: 'none',
    args: '',
    autorestart: true
  }, function(err) {
    if(err) {
      console.log(err);
    }
  });
}
