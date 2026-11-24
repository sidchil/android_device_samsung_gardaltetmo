# patches

these patches go on top of LineageOS 15.1. from the root of your source tree, apply each folder in its matching repo, for example:

```
cd frameworks/av
git am ../../device/samsung/gardaltetmo/patches/frameworks_av/*.patch
```

| folder | repo |
| --- | --- |
| frameworks_av | frameworks/av |
| frameworks_base | frameworks/base |
| frameworks_native | frameworks/native |
| hardware_interfaces | hardware/interfaces |
| hardware_lineage_interfaces | hardware/lineage/interfaces |
| system_nfc | system/nfc |
