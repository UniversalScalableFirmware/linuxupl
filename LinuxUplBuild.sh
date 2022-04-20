#!/bin/bash
ToolChain="GCC5"
BuildTarget="DEBUG"
BzImagePath=""
InitramfsPath=""
while getopts "hK:I:b:t:" arg; do
  case $arg in
    h)
      echo "usage:"
      echo "  -h: help"
      echo "  -K: Linux Kernel path"
      echo "  -I: optional, initramfs path"
      echo "  -b: build target, default is DEBUG"
      echo "  -t: tool chain, default is GCC5"
      exit 0
      ;;
    K)
      BzImagePath=$OPTARG
      ;;
    I)
      InitramfsPath=$OPTARG
      ;;
    b)
      ToolChain=$OPTARG
      ;;
    t)
      BuildTarget=$OPTARG
      ;;
  esac
done

if [ $BzImagePath = "" ]; then
  exit 1
fi

export WORKSPACE=$(cd `dirname $0`; pwd)
export PACKAGES_PATH="$WORKSPACE:$WORKSPACE/../edk2"

while [ $# -gt 0 ]; do
  shift
done

cd $WORKSPACE/../edk2
source ./edksetup.sh
make -C ./BaseTools
cd $WORKSPACE


if [ $InitramfsPath = "" ]; then
  python LinuxUplPkg/LinuxUplBuild.py -t $ToolChain -b $BuildTarget --KernelPath $BzImagePath --InitramfsPath $InitramfsPath
else
  python LinuxUplPkg/LinuxUplBuild.py -t $ToolChain -b $BuildTarget --KernelPath $BzImagePath
fi
