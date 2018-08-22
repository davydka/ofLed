
* addon -> https://github.com/davydka/ofxTeensyOcto

`(pidof -c /home/pi/of/apps/Sites/ofLed/bin/ofLed > /dev/null || /home/pi/of/apps/Sites/ofLed/bin/ofLed &> /home/pi/output.log &)`

* `ansible all -m ping -i ansible/hosts --ask-pass`
* `ansible all -a "/bin/echo hello" -i ansible/hosts --ask-pass`
* `ansible-playbook ansible/HelloWorld.yml -i ansible/hosts --ask-pass`

---

* `cd pgmManager`
* `npm install` (maybe `npm install --python=python2.7`)
* `npm run start`
* todo: `qmidinet -g`
* todo: `1 'QmidiNet 128:0'`
* other todo: `/sendmidi/sendmidi-rpi-jessie dev "port 0" cc 20 5`

---

* `cd of/apps/Sites/ofLed/pgmManager/`
* `npm install`
* `npm install -g pm2`
* `sudo vim /etc/systemd/system/pifull.service`
* `sudo systemctl daemon-reload`
* `npm install -g pm2`
* `sudo systemctl enable pifull.service`
* `sudo systemctl start pifull.service`
* reboot and check qmidinet is running: `pidof qmidinet`


```
[Unit]
Description=pifull pm2 services
After=network.target network-online.target

[Service]
Type=forking
ExecStart=/home/pi/.npm-global/bin/pm2 start ecosystem.config.js
WorkingDirectory=/home/pi/of/apps/Sites/ofLed/pgmManager
StandardOutput=inherit
StandardError=inherit
User=pi

[Install]
WantedBy=multi-user.target
```
