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

typedef struct ZoraDock       ZoraDock;
typedef struct ZoraDockClass  ZoraDockClass;

struct ZoraDock {
	GtkWindow parent;
};

struct ZoraDockClass {
	GtkWindowClass parent;
};

#define ZORA_DOCK_TYPE  (zora_dock_get_type())

ZoraDock *zora_dock_new();
void zora_dock_free(ZoraDock *dock);

#endif /* __ZORA_DOCK_H */
