
* addon -> https://github.com/davydka/ofxTeensyOcto

`(pidof -c /home/pi/of/apps/Sites/ofLed/bin/ofLed > /dev/null || /home/pi/of/apps/Sites/ofLed/bin/ofLed &> /home/pi/output.log &)`

* `ansible all -m ping -i ansible/hosts --ask-pass`
* `ansible all -a "/bin/echo hello" -i ansible/hosts --ask-pass`
* `ansible-playbook ansible/HelloWorld.yml -i ansible/hosts --ask-pass`
