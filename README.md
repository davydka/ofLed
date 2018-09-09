## TODO: ditch midish and use node easymidi for routing, currently using midish on pifull0 and basic node routing on pifulls1-4


* addon -> https://github.com/davydka/ofxTeensyOcto

`(pidof -c /home/pi/of/apps/Sites/ofLed/bin/ofLed > /dev/null || /home/pi/of/apps/Sites/ofLed/bin/ofLed &> /home/pi/output.log &)`

* `ansible all -m ping -i ansible/hosts --ask-pass`
* `ansible all -a "/bin/echo hello" -i ansible/hosts --ask-pass`
* `ansible-playbook ansible/HelloWorld.yml -i ansible/hosts --ask-pass`

---

* `npm install` (maybe `npm install --python=python2.7`)
* note: `/sendmidi/sendmidi-rpi-jessie dev "port 0" cc 20 5`

---

* `cd of/apps/Sites/ofLed/pgmManager/`
* `cp .env.sample .env`
* `npm install`
* `npm install -g pm2`
* `pm2 install pm2-logrotate@2.6.0`
* pifull0: `sudo apt install midish`
* pifull0: `vim ~/.midish`
* `sudo vim /etc/systemd/system/pifull.service`
* `sudo systemctl daemon-reload`
* `sudo systemctl enable pifull.service`
* `sudo systemctl start pifull.service`
* reboot and check qmidinet is running: `pidof qmidinet`


```
[Unit]
Description=pifull pm2 services
Wants=network-online.target
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

```
dnew 0 "Midi Through" rw
dnew 1 "mio" rw
dnew 2 "QmidiNet" rw

# define filter "devicemap"
fnew devicemap
#route device 1 -> device 2
fmap {any 1} {any 2}

i #important: run midish in idle mode, run this routing
```
