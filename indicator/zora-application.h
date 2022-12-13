/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#ifndef __ZORA_APPLICATION_H
#define __ZORA_APPLICATION_H

#include <gtk/gtk.h>
#include <indicator/zora-application.h>
#include <indicator/zora-indicator.h>

#define APPLICATION_ID "com.vlastavesely.zora"
#define APPLICATION_ICON "zora"

typedef struct ZoraApplication       ZoraApplication;
typedef struct ZoraApplicationClass  ZoraApplicationClass;

struct ZoraApplication {
	GtkApplication parent;
	ZoraIndicator *indicator;
};

struct ZoraApplicationClass {
	GtkApplicationClass parent;
};

#define ZORA_APPLICATION_TYPE  (zora_application_get_type())
#define ZORA_APPLICATION(o)    (G_TYPE_CHECK_INSTANCE_CAST((o), \
				ZORA_APPLICATION_TYPE, ZoraApplication))

GType zora_application_get_type(void);

ZoraApplication *zora_application_new(void);

#endif /* __ZORA_APPLICATION_H */
