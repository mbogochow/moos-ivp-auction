#!/bin/bash

EXTEND_DIR=~/workspace/moos-ivp-extend
MOOSIVP_DIR=~/workspace/moos-ivp

FILES=(pAuctioneer pBidder)
NUM=${#FILES[@]}

for ((i=0; i < $NUM; i++)); do
  cp -v "$EXTEND_DIR/bin/${FILES[$i]}" "$MOOSIVP_DIR/bin/"
done


# cp "$EXTEND_DIR/bin/pAuctioneer" "$MOOSIVP_DIR/bin"
# cp "$EXTEND_DIR/bin/pBidder" "$MOOSIVP_DIR/bin"