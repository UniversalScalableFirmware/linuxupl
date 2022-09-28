#!/bin/bash
ToolChain="CLANGDWARF"
BuildTarget="DEBUG"
BzImagePath=""
InitramfsPath=""
LinuxbootConfig=""
UrootCommit="c45c826bfd6636aef82077dfbcb0ac96235fb46b"
LinuxbootCommit="8fe31e0995f048d16b378b90926793a0aa4af1e5"
while getopts "hK:c:I:b:t:w:l:u:" arg; do
  case $arg in
    h)
      echo "usage:"
      echo "  -h: help"
      echo "  -K: Linux Kernel path, default is linuxboot_bzImage_intel"
      echo "  -I: optional, initramfs path"
      echo "  -b: build target, default is DEBUG"
      echo "  -t: tool chain, default is GCC5"
      echo "  -c: Linuxboot build config, default is x86_64_defconfig_intel"
      echo "  -l: Set Linuxboot commit version, default is 8fe31e0995f048d16b378b90926793a0aa4af1e5"
      echo "  -u: Set Uroot commit version, default is c45c826bfd6636aef82077dfbcb0ac96235fb46b"
      exit 0
      ;;
    K)
      BzImagePath=$OPTARG
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
    l)
      LinuxbootCommit=$OPTARG
      ;;
    u)
      UrootCommit=$OPTARG
      ;;
  esac
done

export WORKSPACE=$(cd `dirname $0`; pwd)
while [ $# -gt 0 ]; do
  shift
done

if [[ $BzImagePath = "" ]]; then
  if [ ! -d "$WORKSPACE/ImgBuild" ]; then
    mkdir ImgBuild
  fi
  # Set Linuxboot self build bzImage path and config path
  BzImagePath="$WORKSPACE/ImgBuild/linuxboot_bzImage_intel"
  if [[ $LinuxbootConfig = "" ]]; then
    LinuxbootConfig="$WORKSPACE/Configs/x86_64_defconfig_intel"
  fi

  if [ ! -f $BzImagePath ]; then
    echo "Build linuxboot_bzImage_intel ..."
    if ! command -v go &> /dev/null
    then
      echo "Check go ... Fail"
      echo "Please install go!"
      exit 1
    fi

    if [ ! -f $LinuxbootConfig ]; then
      echo "Linuxboot Config not found!"
      exit 1
    fi

    #　Prepare linuxboot kernel source code
    if [ ! -d "$WORKSPACE/ImgBuild/lb_kernel" ]; then
      git clone https://github.com/linuxboot/linux ./ImgBuild/lb_kernel
    fi
    # Set the PATH for build environment
    export GOPATH="$WORKSPACE/ImgBuild/go"
    export PATH="$WORKSPACE/ImgBuild/go/bin:$PATH"
    export PATH="/usr/local/go/bin:$PATH"
    # Download and install u-root
    GO111MODULE=off go get github.com/u-root/u-root
    GO111MODULE=off go get github.com/nsf/godit
    # Go to the u-root source code folder and build
    cd $WORKSPACE/ImgBuild/go/src/github.com/u-root/u-root/
    ## Use a steady version or customer
    git checkout $UrootCommit
    git rev-parse HEAD
    u-root -build=bb -uinitcmd=systemboot core cmds/exp/{page,ipmidump,dumpebda} github.com/u-root/u-root/cmds/boot/{systemboot,localboot}
    cd $WORKSPACE
    # Build
    cd ./ImgBuild/lb_kernel
    ## Use a steady version or customer
    git checkout $LinuxbootCommit
    git rev-parse HEAD
    ## Copy u-root output to linuxboot source folder
    cp -f /tmp/initramfs.linux_amd64.cpio ./
    ## Copy coreboot config to linuxboot source
    cp -f $LinuxbootConfig ./arch/x86/configs/x86_64_defconfig
    make mrproper
    make defconfig
    make
    # Copy linuxboot output to coreboot payload folder and rename it to "linuxboot_bzImage_intel"
    cd $WORKSPACE
    cp -f ./ImgBuild/lb_kernel/arch/x86/boot/bzImage ./ImgBuild/linuxboot_bzImage_intel
  fi
fi

echo "Build LinuxUplPkg ..."
rm -rf $WORKSPACE/Build
cd $WORKSPACE/LinuxUplPkg
make
cd $WORKSPACE
if [[ $InitramfsPath = "" ]]; then
  python LinuxUplBuild.py -t $ToolChain -b $BuildTarget --KernelPath $BzImagePath
else
  python LinuxUplBuild.py -t $ToolChain -b $BuildTarget --KernelPath $BzImagePath --InitramfsPath $InitramfsPath
fi
