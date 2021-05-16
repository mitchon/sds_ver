# sds_ver
Simple digital signature verifier based on standart GOST 34.10-2018  
This program is used to verify 256bit digital signature to the end of the file based on given parameters

[Automatic DS verifier](https://github.com/mitchon/sds_ver_auto)  
[DS venerator](https://github.com/mitchon/sds_gen)  

Tested in Ubuntu 20.04 LTS

## Installation
<code>make</code>  
<code>make install</code>

## Uninstall
<code>make uninstall</code>

## Start
To start a program use <code>sds_ver</code>. This executable is stored in `/usr/local/bin`.  
To get help type <code>sds_ver -h</code>

## Configuration files
Configs are in `/usr/local/etc/sds`  
ds_params contains digital signature parameters  
public_accounts contains user login and his public keys  
