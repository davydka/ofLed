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
let restart = fs.readFileSync('RESTART').toString();

if(restart == 0 && INDEX == 0) {
  console.log('restart');
  setTimeout(function() {
    fs.writeFileSync('RESTART', 1);
    pm2.restart('midish');
  }, 5000);
}

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

  // listening for Channel 5, NoteOn from Synthstrom (can't send pgmOut)
  input.on('noteon', function (msg) {

    // 0 index midi channels, so channel 5 on device is channel 4 here
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

  let startProcess = pgm.toString().padStart(3, '0');

  //currentPgm = pgm;
  fs.writeFileSync('PGM', pgm);


  pm2.list(function(err, items) {
    if(err){
      console.log(err);
    }

    items.map((item) => {
      if(item.name === 'ofLed') {
        if(item.pm2_env.status === 'online') {
          pm2.stop('ofLed', function(err) {
            if(err) {
              console.log(err);
            }
          });
        }
      }

      if (item.name.match(/^\d/) && item.name != startProcess) {
        console.log(item.name);
        if(item.pm2_env.status === 'online') {
          pm2.stop(item.name, function(err) {
            if(err) {
              console.log(err);
            }
          });
        }
      }
    });
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
