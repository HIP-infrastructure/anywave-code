#include "libeep.h"
#include <stdlib.h>
#include "eep/eepio.h"
///////////////////////////////////////////////////////////////////////////////
#define SCALING_FACTOR 128

libeep::libeep()
{
	_entry_map = NULL;
	_entry_size = 0;
	_recinfo_map = NULL;
	_recinfo_size = 0;
	_channel_map = NULL;
	_channel_size = 0;
}


libeep::~libeep()
{
	// free entry map
	int i;
	for (i = 0; i < _entry_size; ++i) {
		if (_entry_map[i] != NULL) {
			free(_entry_map[i]); 
		}
	}
	if (_entry_map != NULL) {
		free(_entry_map);
	}
	// free recinfo map
	for (i = 0; i < _recinfo_size; ++i) {
		if (_recinfo_map[i] != NULL) {
			free(_recinfo_map[i]);
		}
	}
	if (_recinfo_map != NULL) {
		free(_recinfo_map);
	}
	// free channel map
	for (i = 0; i < _channel_size; ++i) {
		if (_channel_map[i] != NULL) {
			free(_channel_map[i]);
		}
	}
	if (_channel_map != NULL) {
		free(_channel_map);
	}
}


cntfile_t libeep::read(const char *filename)
{
	return read_delegate(filename, 0);
}

cntfile_t libeep::allocate()
{
	struct _libeep_entry **new_entry_map = NULL;
	new_entry_map = (struct _libeep_entry **)realloc(_entry_map, sizeof(struct _libeep_entry *) * (_entry_size + 1));
	if (new_entry_map == NULL) {
		return -1;
	}
	_entry_map = new_entry_map;
	_entry_map[_entry_size] = (struct _libeep_entry *)malloc(sizeof(struct _libeep_entry));
	if (_entry_map[_entry_size] == NULL) {
		return -1;
	}
	_entry_map[_entry_size]->open_mode = om_none;
	_entry_map[_entry_size]->data_type = dt_none;
	_entry_size += 1;
	return _entry_size - 1;
}

cntfile_t libeep::read_delegate(const char *filename, int external_triggers)
{
	int status, channel_id, channel_count;
	int handle = allocate();
	struct _libeep_entry * obj = get_object(handle, om_none);

}