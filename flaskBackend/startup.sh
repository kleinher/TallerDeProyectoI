#!/bin/bash
rm /root/TallerDeProyectoI/flaskBackend/instance/dispositivos.sqlite3 
screen -S test -d -m python3 -m flask run --host=0.0.0.0
