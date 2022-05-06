/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#ifndef __ZORA_INDICATOR_H
#define __ZORA_INDICATOR_H

#include <libayatana-appindicator/app-indicator.h>
#include <indicator/zora-control.h>
#include <indicator/zora-dock.h>

typedef struct ZoraIndicator       ZoraIndicator;
typedef struct ZoraIndicatorClass  ZoraIndicatorClass;

struct ZoraIndicatorClass {
	AppIndicatorClass parent;
};

struct ZoraIndicator {
	AppIndicator parent;
	ZoraControl *control;
	GtkStatusIcon *icon;
	GtkMenu *popup;
	GtkWidget *dock;
	int dock_shown;
};

#define ZORA_INDICATOR_TYPE  (zora_indicator_get_type())
#define ZORA_INDICATOR(o)    (G_TYPE_CHECK_INSTANCE_CAST((o), \
			      ZORA_INDICATOR_TYPE, ZoraIndicator))

ZoraIndicator *zora_indicator_new();
void zora_indicator_free(ZoraIndicator *indicator);

#endif /* __ZORA_INDICATOR_H */
