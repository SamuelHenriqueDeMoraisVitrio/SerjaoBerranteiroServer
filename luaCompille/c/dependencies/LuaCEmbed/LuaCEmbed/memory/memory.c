


static void *private_LuaCembed_custom_allocator(void *ud, void *ptr, size_t osize, size_t nsize) {
    int *used = (int *)ud;

    if (ptr == NULL) {
        osize = 0;
    }

    if (nsize == 0) {
        free(ptr);
        *used -= osize; /* subtract old size from used memory */
        return NULL;
    } else {
        long  custom_limit = (long)(private_lua_cembed_memory_limit * PRIVATE_LUA_CEMBED_ONE_MB);
        if (*used + (nsize - osize) > custom_limit) /* too much memory in use */
            return NULL;
        ptr = realloc(ptr, nsize);
        if (ptr) /* reallocation successful? */
            *used += (nsize - osize);
        return ptr;
    }
}

