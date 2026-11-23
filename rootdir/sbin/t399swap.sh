#!/system/bin/sh
# T399: compressed swap. The 3.4 kernel can't build zram on ARM, but has
# zswap (frontswap). zswap keeps pages compressed in RAM and only writes to
# this backing file when its pool is full, so flash wear stays low.
SWAPFILE=/data/swapfile
SIZE_MB=256

if [ ! -f $SWAPFILE ]; then
    dd if=/dev/zero of=$SWAPFILE bs=1048576 count=$SIZE_MB 2>/dev/null || exit 1
    chmod 600 $SWAPFILE
    mkswap $SWAPFILE >/dev/null || { rm -f $SWAPFILE; exit 1; }
fi
swapon $SWAPFILE
echo 25 > /sys/module/zswap/parameters/max_pool_percent
echo 100 > /proc/sys/vm/swappiness
