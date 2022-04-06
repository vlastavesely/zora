/*
 * Copyright © 2022, Vlasta Vesely
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */
#include <indicator/zora-control.h>

#define SCHEME "com.vlastavesely.zora"

enum {
	PROP_TEMPERATURE = 1,
	PROP_ENABLED,
	N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL};

G_DEFINE_TYPE(ZoraControl, zora_control, G_TYPE_OBJECT);

static void update_temperature(ZoraControl *control)
{
	unsigned int temperature;

	temperature = control->enabled ? control->temperature : 6500;
	zora_randr_set_colour_temperature(&control->state, temperature);
}

static void set_property(GObject *object, unsigned int prop_id,
			 const GValue *value, GParamSpec *pspec)
{
	ZoraControl *control = ZORA_CONTROL(object);

	switch (prop_id) {
	case PROP_TEMPERATURE:
		if (control->temperature != g_value_get_uint(value)) {
			control->temperature = g_value_get_uint(value);
			update_temperature(control);
		}
		break;

	case PROP_ENABLED:
		if (control->enabled != g_value_get_boolean(value)) {
			control->enabled = g_value_get_boolean(value);
			update_temperature(control);
		}
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

static void get_property(GObject *object, unsigned int prop_id, GValue *value,
			 GParamSpec *pspec)
{
	ZoraControl *control = ZORA_CONTROL(object);

	switch (prop_id) {
	case PROP_TEMPERATURE:
		g_value_set_uint(value, control->temperature);
		break;

	case PROP_ENABLED:
		g_value_set_boolean(value, control->enabled);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

static void finalise(GObject *object)
{
	ZoraControl *control = ZORA_CONTROL(object);

	zora_randr_finalise(&control->state);
	g_object_unref(control->settings);

	G_OBJECT_CLASS(zora_control_parent_class)->finalize(object);
}

static void zora_control_class_init(ZoraControlClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	object_class->get_property = get_property;
	object_class->set_property = set_property;
	object_class->finalize = finalise;

	obj_properties[PROP_TEMPERATURE] = g_param_spec_uint("temperature",
			"Temperature", "White colour temperature in Kelvins",
			1000, 13000, 6500, G_PARAM_READWRITE);

	obj_properties[PROP_ENABLED] = g_param_spec_boolean("enabled",
			"Enabled", "Whether the colour shift should be enabled",
			TRUE, G_PARAM_READWRITE);

	g_object_class_install_properties(object_class, N_PROPERTIES,
					  obj_properties);
}

static void zora_control_init(ZoraControl *self)
{
	GSettings *settings;

	zora_randr_initialise(&self->state);

	settings = g_settings_new(SCHEME);
	self->temperature = g_settings_get_uint(settings, "temperature");
	self->enabled = g_settings_get_boolean(settings, "enabled");

	g_settings_bind(settings, "temperature", self, "temperature",
			G_SETTINGS_BIND_DEFAULT);
	g_settings_bind(settings, "enabled", self, "enabled",
			G_SETTINGS_BIND_DEFAULT);

	/* Restore the last state on start */
	update_temperature(self);

	self->settings = settings;
}

ZoraControl *zora_control_new()
{
	return g_object_new(ZORA_CONTROL_TYPE, NULL);
}

void zora_control_free(ZoraControl *control)
{
	g_object_unref(control);
}

unsigned int zora_control_get_temperature(ZoraControl *control)
{
	return control->temperature;
}

int zora_control_get_enabled(ZoraControl *control)
{
	return control->enabled;
}

void zora_control_set_temperature(ZoraControl *control, unsigned int temperature)
{
	g_object_set(G_OBJECT(control), "temperature", temperature, NULL);
}

void zora_control_set_enabled(ZoraControl *control, int enabled)
{
	g_object_set(G_OBJECT(control), "enabled", enabled, NULL);
}
