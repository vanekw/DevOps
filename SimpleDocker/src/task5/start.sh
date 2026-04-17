#!/bin/bash

service nginx start
spawn-fcgi -p 8080 ./webserver
nginx -s reload
tail -f /dev/null