Microcoap example
============

This is a small microcoap example application. It provides a server which only 
answers GET requests to the resource /foo/bar. 

You may use the Copper Firefox Plugin to send those requests and use 
[marz](https://github.com/sgso/marz) to tunnel them to the RIOT native thread.
When you've installed the Plugin and set up marz, visit

    coap://[your:RIOT::IP]:5683/foo/bar

You can find out your RIOT instance's IP with the ``ifconfig`` shell command.