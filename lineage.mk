# Inherit some common Lineage stuff.
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)
$(call inherit-product, device/samsung/gardaltetmo/full_gardaltetmo.mk)

PRODUCT_NAME := lineage_gardaltetmo
