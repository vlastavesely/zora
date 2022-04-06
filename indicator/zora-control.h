/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#ifndef __ZORA_CONTROL_H
#define __ZORA_CONTROL_H

#include <gtk/gtk.h>
#include "../zora/randr.h"

typedef struct ZoraControl       ZoraControl;
typedef struct ZoraControlClass  ZoraControlClass;

struct ZoraControlClass {
	GObjectClass parent;
};

struct ZoraControl {
	GObject parent;
	struct zora_randr_state state;
	GSettings *settings;
	unsigned int temperature;
	int enabled;
};

#define ZORA_CONTROL_TYPE  (zora_control_get_type())
#define ZORA_CONTROL(o)    (G_TYPE_CHECK_INSTANCE_CAST((o), \
			    ZORA_CONTROL_TYPE, ZoraControl))

GType zora_control_get_type(void);

ZoraControl *zora_control_new();
void zora_control_free(ZoraControl *control);

unsigned int zora_control_get_temperature(ZoraControl *control);
int zora_control_get_enabled(ZoraControl *control);

void zora_control_set_temperature(ZoraControl *control, unsigned int temperature);
void zora_control_set_enabled(ZoraControl *control, int enabled);

#endif /* __ZORA_CONTROL_H */
