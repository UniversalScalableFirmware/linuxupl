# upl.payload.linuxboot
A payload help bootloader to boot to Linux operation system.

## Build Requirements
- make (GNU Make 4.2.1)  
- gcc (gcc version 9.4.0)  
- gcc-ar (GNU ar (GNU Binutils for Ubuntu) 2.34)  
- nasm (NASM version 2.15.05)  
- python (Python 2.7.18)
- go (go version go1.13.5 linux/amd64)

## How to build LinuxUpl
Create a new directory <workspace>, and using the below command  
```  
cd <workspace>  
./LinuxUplBuild.sh 
```  

LinuxUniversalPayload.elf is generated in ```<workspace>/Build``` folder.  

