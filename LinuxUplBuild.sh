#!/bin/bash
ToolChain="CLANGDWARF"
BuildTarget="DEBUG"
BzImagePath=""
InitramfsPath=""
WithoutEdk2="TRUE"
LinuxbootConfig="INTEL"
while getopts "hc:I:b:t:w" arg; do
  case $arg in
    h)
      echo "usage:"
      echo "  -h: help"
      echo "  -I: optional, initramfs path"
      echo "  -b: build target, default is DEBUG"
      echo "  -t: tool chain, default is GCC5"
      echo "  -w: WithoutEdk2, default is TRUE"
      echo "  -c: Linuxboot Config [INTEL|OSF], default is INTEL"
      exit 0
      ;;
    I)
      InitramfsPath=$OPTARG
      ;;
    t)
      ToolChain=$OPTARG
      ;;
    b)
      BuildTarget=$OPTARG
      ;;
    c)
      LinuxbootConfig=$OPTARG
      ;;
  esac
done

export WORKSPACE=$(cd `dirname $0`; pwd)
if [[ $WithoutEdk2 = "FLASE" ]]; then
  export PACKAGES_PATH="$WORKSPACE:$WORKSPACE/../Edk2"
fi
while [ $# -gt 0 ]; do
  shift
done

if [[ $LinuxbootConfig = "OSF" ]]; then
  BzImagePath="$WORKSPACE/Bzimage/linuxboot_bzImage_osf"
elif [[ $LinuxbootConfig = "INTEL" ]]; then
  BzImagePath="$WORKSPACE/Bzimage/linuxboot_bzImage_intel"
  if [ ! -f $BzImagePath ]; then
    echo "Build linuxboot_bzImage_intel ..."
    if ! command -v go &> /dev/null
    then
      echo "Check go ... Fail"
      echo "Please install go!"
      exit
    fi

    #　Prepare linuxboot kernel source code
    if [ ! -d "$WORKSPACE/lb_kernel" ]; then
	  git clone https://github.com/linuxboot/linux lb_kernel
	fi
    # Set the PATH for build environment
    export GOPATH="$WORKSPACE/go"
    export PATH="$WORKSPACE/go/bin:$PATH"
    export PATH="/usr/local/go/bin:$PATH"
    # Download and install u-root
    GO111MODULE=off go get github.com/u-root/u-root
    GO111MODULE=off go get github.com/nsf/godit
    # Go to the u-root source code folder and build
    cd $WORKSPACE/go/src/github.com/u-root/u-root/
    u-root -build=bb -uinitcmd=systemboot core cmds/exp/{page,ipmidump,dumpebda} github.com/u-root/u-root/cmds/boot/{systemboot,localboot}
    cd $WORKSPACE
    # Copy u-root output to linuxboot source folder
    cp -f /tmp/initramfs.linux_amd64.cpio ./lb_kernel/
    # Copy coreboot config to linuxboot source
    cp -f ./Configs/x86_64_defconfig_intel ./lb_kernel/arch/x86/configs/x86_64_defconfig
    # Build
    cd ./lb_kernel
    make mrproper
    make defconfig
    make
    # Copy linuxboot output to coreboot payload folder and rename it to "linuxboot_bzImage_intel"
    cd $WORKSPACE
    cp -f ./lb_kernel/arch/x86/boot/bzImage ./Bzimage/linuxboot_bzImage_intel
  fi
else
  echo "Please set Linuxboot Config [INTEL|OSF]"
  exit 1
fi

if [[ $WithoutEdk2 = "FALSE" ]]; then
  cd $WORKSPACE/../Edk2
  source ./edksetup.sh
  make -C ./BaseTools
  cd $WORKSPACE
  if [[ $InitramfsPath = "" ]]; then
    python LinuxUplPkg/LinuxUplBuild.py -t $ToolChain -b $BuildTarget --KernelPath $BzImagePath
  else
    python LinuxUplPkg/LinuxUplBuild.py -t $ToolChain -b $BuildTarget --KernelPath $BzImagePath --InitramfsPath $InitramfsPath
  fi
else
  rm -rf $WORKSPACE/Build
  cd $WORKSPACE/LinuxUplPkg
  make
  cd $WORKSPACE
  if [[ $InitramfsPath = "" ]]; then
    python LinuxUplBuild.py -t $ToolChain -b $BuildTarget --KernelPath $BzImagePath
  else
    python LinuxUplBuild.py -t $ToolChain -b $BuildTarget --KernelPath $BzImagePath --InitramfsPath $InitramfsPath
  fi
fi
