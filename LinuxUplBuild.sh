#!/bin/bash
ToolChain="CLANGDWARF"
BuildTarget="DEBUG"
BzImagePath=""
InitramfsPath=""
WithoutEdk2="TRUE"
while getopts "hK:I:b:t:w:" arg; do
  case $arg in
    h)
      echo "usage:"
      echo "  -h: help"
      echo "  -K: Linux Kernel path"
      echo "  -I: optional, initramfs path"
      echo "  -b: build target, default is DEBUG"
      echo "  -t: tool chain, default is GCC5"
      echo "  -w: WithoutEdk2, default is TRUE"
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
  esac
done

if [[ $BzImagePath = "" ]]; then
  exit 1
fi

export WORKSPACE=$(cd `dirname $0`; pwd)
if [[ $WithoutEdk2 = "FLASE" ]]; then
  export PACKAGES_PATH="$WORKSPACE:$WORKSPACE/../Edk2"
fi
while [ $# -gt 0 ]; do
  shift
done

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
