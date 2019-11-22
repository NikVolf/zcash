package=crate_cc
$(package)_crate_name=cc
$(package)_version=1.0.45
$(package)_download_path=https://static.crates.io/crates/$($(package)_crate_name)
$(package)_file_name=$($(package)_crate_name)-$($(package)_version).crate
$(package)_sha256_hash=4fc9a35e1f4290eb9e5fc54ba6cf40671ed2a2514c3eeb2b2a908dda2ea5a1be
$(package)_crate_versioned_name=$($(package)_crate_name)

define $(package)_preprocess_cmds
  $(call generate_crate_checksum,$(package))
endef

define $(package)_stage_cmds
  $(call vendor_crate_source,$(package))
endef
