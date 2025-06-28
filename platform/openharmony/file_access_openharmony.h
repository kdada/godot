#pragma once

#include "core/io/file_access.h"
#include "drivers/unix/file_access_unix.h"

#include <rawfile/raw_file_manager.h>
#include <rawfile/raw_dir.h>

class FileAccessOpenHarmony : public FileAccessUnix {
	static NativeResourceManager *resource_manager;
	RawFile64 *_rawfile = nullptr;
	bool _is_rawfile = false;
	String _cpath = "";

protected:
	bool is_in_bundle(String p_path);

public:
	static void setup(NativeResourceManager *p_resource_manager);

	virtual Error open_internal(const String &p_path, int p_mode_flags) override;
	virtual bool is_open() const override;

	virtual String get_path() const override;
	virtual String get_path_absolute() const override;

	virtual void seek(uint64_t p_position) override;
	virtual void seek_end(int64_t p_position = 0) override;
	virtual uint64_t get_position() const override;
	virtual uint64_t get_length() const override;

	virtual bool eof_reached() const override;
	virtual uint64_t get_buffer(uint8_t *p_dst, uint64_t p_length) const override;

	virtual Error get_error() const override;

	virtual Error resize(int64_t p_length) override;
	virtual void flush() override;
	virtual bool store_buffer(const uint8_t *p_src, uint64_t p_length) override;

	virtual bool file_exists(const String &p_path) override;

	virtual uint64_t _get_modified_time(const String &p_file) override;
	virtual BitField<FileAccess::UnixPermissionFlags> _get_unix_permissions(const String &p_file) override;
	virtual Error _set_unix_permissions(const String &p_file, BitField<FileAccess::UnixPermissionFlags> p_permissions) override;

	virtual void close() override;

	FileAccessOpenHarmony();
	virtual ~FileAccessOpenHarmony();
};