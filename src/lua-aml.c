#include <lua.h>
#include <lauxlib.h>

#include <aml.h>

struct l_aml_obj {
	void* obj;
	struct lua_State* L;
	int self_ref;
	int cb_ref;
};

struct l_aml_handler {
	struct aml_handler* handler;
	struct lua_State* L;
	int self_ref;
	int cb_ref;
};

struct l_aml_timer {
	struct aml_timer* timer;
	struct lua_State* L;
	int self_ref;
	int cb_ref;
};

struct l_aml_ticker {
	struct aml_ticker* ticker;
	struct lua_State* L;
	int self_ref;
	int cb_ref;
};

static int l_aml_new(struct lua_State* L)
{
	struct aml* aml = aml_new();
	if (!aml)
		return 0;

	struct aml** ud = lua_newuserdata(L, sizeof(*ud));
	*ud = aml;

	luaL_getmetatable(L, "meta_aml");
	lua_setmetatable(L, -2);

	return 1;
}

static int l_aml_gc(struct lua_State* L)
{
	struct l_aml_handler* ud = luaL_checkudata(L, 1, "meta_aml");
	aml_unref(ud->handler);
	return 0;
}

static int l_aml_set_default(struct lua_State* L)
{
	struct aml** ud = luaL_checkudata(L, 1, "meta_aml");
	aml_set_default(*ud);
	return 0;
}

static int l_aml_get_default(struct lua_State* L)
{
	struct aml* aml = aml_get_default();
	if (!aml)
		return 0;

	struct aml** ud = lua_newuserdata(L, sizeof(*ud));
	*ud = aml;

	luaL_getmetatable(L, "meta_aml");
	lua_setmetatable(L, -2);

	return 1;
}

static void l_aml_handler_callback(void* obj)
{
	 struct l_aml_handler* ud = aml_get_userdata(obj);

	 lua_rawgeti(ud->L, LUA_REGISTRYINDEX, ud->cb_ref);
	 lua_rawgeti(ud->L, LUA_REGISTRYINDEX, ud->self_ref);

	 lua_call(ud->L, 1, 0);
}

static void l_aml_handler_free(void* userdata)
{
	struct l_aml_handler* ud = userdata;
	luaL_unref(ud->L, LUA_REGISTRYINDEX, ud->cb_ref);
	luaL_unref(ud->L, LUA_REGISTRYINDEX, ud->self_ref);
}

static int l_aml_handler_new(struct lua_State* L)
{
	int fd = luaL_checkinteger(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);

	struct l_aml_handler* ud = lua_newuserdata(L, sizeof(*ud));
	ud->L = L;

	ud->handler = aml_handler_new(fd, l_aml_handler_callback, ud,
			l_aml_handler_free);

	lua_pushvalue(L, -1);
	ud->self_ref = luaL_ref(L, LUA_REGISTRYINDEX);

	lua_pushvalue(L, 2);
	ud->cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);

	luaL_getmetatable(L, "meta_aml_handler");
	lua_setmetatable(L, -2);

	return 1;
}

static int l_aml_handler_gc(struct lua_State* L)
{
	struct l_aml_handler* ud = luaL_checkudata(L, 1, "meta_aml_handler");
	aml_unref(ud->handler);
	return 0;
}

static void l_aml_timer_callback(void* obj)
{
	 struct l_aml_timer* ud = aml_get_userdata(obj);

	 lua_rawgeti(ud->L, LUA_REGISTRYINDEX, ud->cb_ref);
	 lua_rawgeti(ud->L, LUA_REGISTRYINDEX, ud->self_ref);

	 lua_call(ud->L, 1, 0);
}

static void l_aml_timer_free(void* userdata)
{
	struct l_aml_timer* ud = userdata;
	luaL_unref(ud->L, LUA_REGISTRYINDEX, ud->cb_ref);
	luaL_unref(ud->L, LUA_REGISTRYINDEX, ud->self_ref);
}

static int l_aml_timer_new(struct lua_State* L)
{
	int timeout = luaL_checkinteger(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);

	struct l_aml_timer* ud = lua_newuserdata(L, sizeof(*ud));
	ud->L = L;

	ud->timer = aml_timer_new(timeout, l_aml_timer_callback, ud,
			l_aml_timer_free);

	lua_pushvalue(L, -1);
	ud->self_ref = luaL_ref(L, LUA_REGISTRYINDEX);

	lua_pushvalue(L, 2);
	ud->cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);

	luaL_getmetatable(L, "meta_aml_timer");
	lua_setmetatable(L, -2);

	return 1;
}

static int l_aml_timer_gc(struct lua_State* L)
{
	struct l_aml_timer* ud = luaL_checkudata(L, 1, "meta_aml_timer");
	aml_unref(ud->timer);
	return 0;
}

static void l_aml_ticker_callback(void* obj)
{
	 struct l_aml_ticker* ud = aml_get_userdata(obj);

	 lua_rawgeti(ud->L, LUA_REGISTRYINDEX, ud->cb_ref);
	 lua_rawgeti(ud->L, LUA_REGISTRYINDEX, ud->self_ref);

	 lua_call(ud->L, 1, 0);
}

static void l_aml_ticker_free(void* userdata)
{
	struct l_aml_ticker* ud = userdata;
	luaL_unref(ud->L, LUA_REGISTRYINDEX, ud->cb_ref);
	luaL_unref(ud->L, LUA_REGISTRYINDEX, ud->self_ref);
}

static int l_aml_ticker_new(struct lua_State* L)
{
	int period = luaL_checkinteger(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);

	struct l_aml_ticker* ud = lua_newuserdata(L, sizeof(*ud));
	ud->L = L;

	ud->ticker = aml_ticker_new(period, l_aml_ticker_callback, ud,
			l_aml_ticker_free);

	lua_pushvalue(L, -1);
	ud->self_ref = luaL_ref(L, LUA_REGISTRYINDEX);

	lua_pushvalue(L, 2);
	ud->cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);

	luaL_getmetatable(L, "meta_aml_ticker");
	lua_setmetatable(L, -2);

	return 1;
}

static int l_aml_ticker_gc(struct lua_State* L)
{
	struct l_aml_ticker* ud = luaL_checkudata(L, 1, "meta_aml_ticker");
	aml_unref(ud->ticker);
	return 0;
}

static int l_aml_poll(struct lua_State* L)
{
	struct aml** ud = luaL_checkudata(L, 1, "meta_aml");
	int timeout = luaL_checkint(L, 2);

	int rc = aml_poll(*ud, timeout);

	lua_pushinteger(L, rc);
	return 1;
}

static int l_aml_run(struct lua_State* L)
{
	struct aml** ud = luaL_checkudata(L, 1, "meta_aml");

	int rc = aml_run(*ud);

	lua_pushinteger(L, rc);
	return 1;
}

static int l_aml_exit(struct lua_State* L)
{
	struct aml** ud = luaL_checkudata(L, 1, "meta_aml");
	aml_exit(*ud);
	return 0;
}

static int l_aml_dispatch(struct lua_State* L)
{
	struct aml** ud = luaL_checkudata(L, 1, "meta_aml");
	aml_dispatch(*ud);
	return 0;
}

static int l_aml_interrupt(struct lua_State* L)
{
	struct aml** ud = luaL_checkudata(L, 1, "meta_aml");
	aml_interrupt(*ud);
	return 0;
}

static int l_aml_start(struct lua_State* L)
{
	struct aml** ud = luaL_checkudata(L, 1, "meta_aml");

	// TODO: Make sure this is actually what we expect
	struct l_aml_obj* obj_ud = lua_touserdata(L, 2);

	int rc = aml_start(*ud, obj_ud->obj);

	lua_pushboolean(L, rc == 0);
	return 1;
}

static int l_aml_stop(struct lua_State* L)
{
	struct aml** ud = luaL_checkudata(L, 1, "meta_aml");

	// TODO: Make sure this is actually what we expect
	struct l_aml_obj* obj_ud = lua_touserdata(L, 2);

	int rc = aml_stop(*ud, obj_ud->obj);

	lua_pushboolean(L, rc == 0);
	return 1;
}

static int l_aml_is_started(struct lua_State* L)
{
	struct aml** ud = luaL_checkudata(L, 1, "meta_aml");

	// TODO: Make sure this is actually what we expect
	struct l_aml_obj* obj_ud = lua_touserdata(L, 2);

	bool rc = aml_is_started(*ud, obj_ud->obj);

	lua_pushboolean(L, rc);
	return 1;
}

__attribute__((visibility("default")))
int luaopen_aml(struct lua_State* L)
{
	static const struct luaL_Reg l_aml_functions[] = {
		{ "__gc", l_aml_gc },
		{ "poll", l_aml_poll },
		{ "run", l_aml_run },
		{ "exit", l_aml_exit },
		{ "dispatch", l_aml_dispatch },
		{ "interrupt", l_aml_interrupt },
		{ "start", l_aml_start },
		{ "stop", l_aml_stop },
		{ "is_started", l_aml_is_started },
		{ NULL, NULL }
	};

	luaL_newmetatable(L, "meta_aml");
	luaL_register(L, NULL, l_aml_functions);
	lua_setfield(L, -1, "__index");

	static const struct luaL_Reg l_aml_handler_functions[] = {
		{ "__gc", l_aml_handler_gc },
// TODO:
//		{ "get_fd", l_aml_handler_get_fd },
//		{ "get_revents", l_aml_handler_get_revents },
		{ NULL, NULL }
	};

	luaL_newmetatable(L, "meta_aml_handler");
	luaL_register(L, NULL, l_aml_handler_functions);
	lua_setfield(L, -1, "__index");

	static const struct luaL_Reg l_aml_timer_functions[] = {
		{ "__gc", l_aml_timer_gc },
// TODO:
//		{ "set_duration", l_aml_timer_set_duration },
		{ NULL, NULL }
	};

	luaL_newmetatable(L, "meta_aml_timer");
	luaL_register(L, NULL, l_aml_timer_functions);
	lua_setfield(L, -1, "__index");

	static const struct luaL_Reg l_aml_ticker_functions[] = {
		{ "__gc", l_aml_ticker_gc },
// TODO:
//		{ "set_duration", l_aml_timer_set_duration },
		{ NULL, NULL }
	};

	luaL_newmetatable(L, "meta_aml_ticker");
	luaL_register(L, NULL, l_aml_ticker_functions);
	lua_setfield(L, -1, "__index");

	static const struct luaL_Reg l_base_functions[] = {
		{ "new", l_aml_new },
		{ "set_default", l_aml_set_default },
		{ "get_default", l_aml_get_default },
		{ "handler_new", l_aml_handler_new },
		{ "timer_new", l_aml_timer_new },
		{ "ticker_new", l_aml_ticker_new },
// TODO:
//		{ "signal_new", l_aml_signal_new },
//		{ "idle_new", l_aml_idle_new },
		{ NULL, NULL }
	};

	luaL_register(L, "aml", l_base_functions);
	return 1;
}
