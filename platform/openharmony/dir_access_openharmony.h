#pragma once

#include "core/io/dir_access.h"
#include "drivers/unix/dir_access_unix.h"

#include <rawfile/raw_file_manager.h>
#include <rawfile/raw_dir.h>


class DirAccessOpenHarmony : public DirAccessUnix {
	static NativeResourceManager *resource_manager;
	RawDir *_rawdir = nullptr;
	int _rawdir_counter = 0;
	int _rawfile_count = 0;
	bool _is_rawdir = false;
	String _cpath = "";

protected:
	String get_absolute_path(String p_path);
	bool is_in_bundle(String p_path);

public:
	static void setup(NativeResourceManager *p_resource_manager);

	virtual Error list_dir_begin() override;
	virtual String get_next() override;
	virtual bool current_is_dir() const override;
	virtual bool current_is_hidden() const override;
	virtual void list_dir_end() override;

	virtual Error change_dir(String p_dir) override;
	virtual String get_current_dir(bool p_include_drive = true) const override;
	virtual Error make_dir(String p_dir) override;

	virtual bool file_exists(String p_file) override;
	virtual bool dir_exists(String p_dir) override;
	virtual bool is_readable(String p_dir) override;
	virtual bool is_writable(String p_dir) override;

	virtual uint64_t get_modified_time(String p_file) override;
	virtual Error rename(String p_path, String p_new_path) override;
	virtual Error remove(String p_path) override;

	virtual bool is_link(String p_file) override;
	virtual String read_link(String p_file) override;
	virtual Error create_link(String p_source, String p_target) override;

	virtual bool is_case_sensitive(const String &p_path) const override;
	virtual uint64_t get_space_left() override;
	virtual String get_filesystem_type() const override;

	DirAccessOpenHarmony();
	~DirAccessOpenHarmony();
};