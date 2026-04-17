#!/bin/bash

SUDO_PASSWORD="2387"

sshpass -p "$SUDO_PASSWORD" sudo scp src/cat/s21_cat titandea2@172.24.116.8:/usr/local/bin/
sshpass -p "$SUDO_PASSWORD" sudo scp src/grep/s21_grep titandea2@172.24.116.8:/usr/local/bin/