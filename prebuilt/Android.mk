LOCAL_PATH := $(call my-dir)

# We use a prebuilt kernel, so Lineage's kernel.mk never defines this target,
# but some modules (ebtables) still depend on it.
.PHONY: INSTALLED_KERNEL_HEADERS
INSTALLED_KERNEL_HEADERS:
