#!/bin/bash
sudo make clean
sudo phpize
sudo ./configure --with-php-config=/usr/bin/php-config
sudo make && make install