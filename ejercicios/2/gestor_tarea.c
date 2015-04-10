#include "gestor_tarea.h"

struct gestor_tarea{
	struct tarea *tareas;
	uint16_t num_tareas;

	uint16_t flags;
};

static int search_pos(struct gestor_tarea *g, int prio)
{
	int i;
	int res = g->num_tareas;
	for (i = 0; i < g->num_tareas; i++) {
		struct tarea *t = &(g->tareas[i]);
		if (tarea_attr_get_u32(t, TAREA_ATTR_PRIORIDAD) <= prio) {
			res = i;
			break;
		}
	}
	return res;
}

struct gestor_tarea *gestor_tarea_alloc(void)
{
	struct gestor_tarea *g =
		(struct gestor_tarea *) calloc(1, sizeof(struct gestor_tarea));
	g->tareas = (struct tarea *) calloc(10, sizeof(struct tarea));
	return g;
}

void gestor_tarea_free(struct gestor_tarea *g)
{
	xfree(g->tareas);
	xfree(g);
}


bool gestor_tarea_attr_is_set(const struct gestor_tarea *g, uint16_t attr)
{
	return g->flags & (1 << attr);
}

void gestor_tarea_attr_unset_tarea(struct gestor_tarea *g, uint16_t n)
{
	if (!(g->flags & (1 << GESTOR_TAREA_ATTR_NUM_TAREAS)) || g->num_tareas <= n)
		return;
	int i = n;
	for(i = g->num_tareas; i >= n; i--)
		g->tareas[i] = g->tareas[i-1];
	g->num_tareas--;
	if (g->num_tareas == 0)
		g->flags = 0;
}

void gestor_tarea_attr_set_tarea(struct gestor_tarea *g, const struct tarea *t)
{
	if (t == NULL)
		return;

	if (!(g->flags & (1 << GESTOR_TAREA_ATTR_NUM_TAREAS))) {
		g->flags = 3; //set num_tareas y tareas
		g->num_tareas = 0;
	}

	int pos = search_pos(g, tarea_attr_get_u32(t, TAREA_ATTR_PRIORIDAD));
	int i = g->num_tareas;
	if (g->num_tareas >= 10)
		gestor_tarea_attr_unset_tarea(g, 9);
	for(i = g->num_tareas; i > pos; i--)
		g->tareas[i] = g->tareas[i-1];
	g->tareas[pos] = *t;
	g->num_tareas++;
}

const struct tarea *gestor_tarea_attr_get_tarea(struct gestor_tarea *g, uint16_t num)
{
	if (num >= g->num_tareas || !(g->flags & (1 << GESTOR_TAREA_ATTR_NUM_TAREAS)))
		return NULL;
	return &(g->tareas[num]);
}

const uint16_t gestor_tarea_attr_get_u16(struct gestor_tarea *g, uint16_t attr)
{
	if (!(g->flags & (1 << GESTOR_TAREA_ATTR_NUM_TAREAS)) 
		 || attr != GESTOR_TAREA_ATTR_NUM_TAREAS)
		return 0;
	return g->num_tareas;
}

int gestor_tarea_snprintf(char *buf, size_t size, struct gestor_tarea *g, int n)
{
	int tmp = snprintf(buf, size, "Tarea numero: %d\n", n);
	if (n >= g->num_tareas || !(g->flags & (1 << GESTOR_TAREA_ATTR_NUM_TAREAS)))
		return 0;
	return tmp + tarea_snprintf(buf + tmp, size - tmp, g->tareas + n);
}
