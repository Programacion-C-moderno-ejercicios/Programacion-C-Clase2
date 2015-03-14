#include "tarea.h"
#include <assert.h>

int main(void)
{
	struct tarea *t;
	char info[250];

	t = tarea_alloc();

	tarea_attr_set_str(t, TAREA_ATTR_NOMBRE, "Tarea");
	assert(!(strcmp(tarea_attr_get_str(t, TAREA_ATTR_NOMBRE), "Tarea")));
	tarea_attr_set_str(t, TAREA_ATTR_DESCRIPCION, "Descripcion tarea");
	assert(!(strcmp(tarea_attr_get_str(t, TAREA_ATTR_DESCRIPCION), "Descripcion tarea")));
	tarea_attr_set_u32(t, TAREA_ATTR_ID, 1);
	assert(tarea_attr_get_u32(t, TAREA_ATTR_ID) == 1);
	tarea_attr_set_str(t, TAREA_ATTR_USUARIO, "Pablo");
	assert(!(strcmp(tarea_attr_get_str(t, TAREA_ATTR_USUARIO), "Pablo")));
	tarea_attr_set_u32(t, TAREA_ATTR_PRIORIDAD, 0);
	assert(tarea_attr_get_u32(t, TAREA_ATTR_PRIORIDAD) == 0);

	tarea_snprintf(info, sizeof(info), t);
	printf("%s", info);

	tarea_free(t);
	return 0;
}
