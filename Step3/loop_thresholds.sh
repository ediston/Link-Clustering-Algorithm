#!/usr/bin/env bash

# loop_thresholds.sh
# Vivek Dhiman 

# these need to be updated by you:
EXEC=./cluster
NET=networkEdgeIdMap.csv
JACC=newnetwork.jaccs
OUTDIR=clusters
# $EXEC network.pairs network.jaccs network.clusters network.cluster_stats threshold

for thr in 0.9 0.8 0.7 0.6 0.5 0.4 0.3 0.25 0.2 0.15 0.1
do
    echo $thr
    $EXEC $NET $JACC $OUTDIR/network_$thr.cluster $OUTDIR/network_$thr.cluster_stats $thr > $OUTDIR/runTime_$thr
done
