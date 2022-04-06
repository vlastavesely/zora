/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#ifndef __ZORA_DOCK_H
#define __ZORA_DOCK_H

#include <gtk/gtk.h>
#include "zora-control.h"

typedef struct ZoraDock       ZoraDock;
typedef struct ZoraDockClass  ZoraDockClass;

struct ZoraDock {
	GtkWindow parent;
	GtkAdjustment *adjustment;
	GtkWidget *scale;
	ZoraControl *control;
};

struct ZoraDockClass {
	GtkWindowClass parent;
};

#define ZORA_DOCK_TYPE  (zora_dock_get_type())
#define ZORA_DOCK(o)    (G_TYPE_CHECK_INSTANCE_CAST((o), \
			 ZORA_DOCK_TYPE, ZoraDock))

GType zora_dock_get_type(void);

ZoraDock *zora_dock_new(ZoraControl *control);//, unsigned int value);
void zora_dock_free(ZoraDock *dock);

#endif /* __ZORA_DOCK_H */
