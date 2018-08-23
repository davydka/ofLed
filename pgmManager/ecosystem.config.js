const config = require('dotenv').config();

if(config.error) {
	throw config.error;
}

const {
	INDEX
} = process.env;

const apps = [];

apps.push({
	name: 'ofLed',
	script: '../bin/ofLed',
	watch: false,
	interpreter: 'none',
	args: '',
	autorestart: true
});

apps.push({
	name: 'qmidinet',
	script: 'qmidinet',
	watch: false,
	interpreter: 'none',
	args: '-g',
	autorestart: true
});

if(INDEX === '0') {
	apps.push({
		name: 'midish',
		script: 'midish',
		watch: false,
		interpreter: 'none',
		args: '',
		autorestart: true
	});
}

module.exports = {
	apps
};
