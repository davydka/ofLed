const apps = [];

apps.push({
	name: 'qmidinet process',
	script: 'qmidinet',
	watch: false,
	interpreter: 'none',
	args: '-g',
	autorestart: true
});

module.exports = {
	apps
};
